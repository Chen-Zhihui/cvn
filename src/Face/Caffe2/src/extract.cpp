

#include <nmslib/object.h>
#include <Cvn/Face/Caffe2/extract.h>
#include <nmslib/space.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include "Poco/DirectoryIterator.h"
#include <nmslib/space/space_vector.h>
#include <nmslib/space/space_vector_gen.h>
#include <fmt/format.h>

using namespace similarity;

Extracter::Extracter(feature_fun feature, size_t maxobjs , size_t maxImgPerObj )
	:_feature(feature),_maxOjbs(maxobjs), _maxImgPerObj(maxImgPerObj){

}

struct DistL2 {
	/*
	 * Important: the function is const and arguments are const as well!!!
	 */
	float operator()(const float* x, const float* y, size_t qty) const {
		float res = 0;
		for (size_t i = 0; i < qty; ++i) res += (x[i] - y[i])*(x[i] - y[i]);
		return sqrt(res);
	}
};

bool Extracter::extract(const std::string & inputdir, const std::string & dbFile, const std::string & dateSetFile, bool mkdb) const {
	similarity::VectorSpaceGen<float, DistL2> space;
	similarity::ObjectVector ov;

	Poco::File dsFile(dateSetFile);
	if (dsFile.exists())
		dsFile.remove();

	auto output = space.OpenWriteFileHeader(ov, dateSetFile);
	auto storage = sqlite_orm::make_storage(dbFile, ImgRec::make_table(), LabelRec::make_table());
	if( mkdb ) {
	storage.sync_schema();
	storage.remove_all<ImgRec>();
	storage.remove_all<LabelRec>();
	}

	Poco::DirectoryIterator it(inputdir);
	Poco::DirectoryIterator end;
	uint32_t id = 0;
	uint32_t label = 0;
	while (it != end) {
		Poco::Path folder(inputdir, it.name());
		Poco::DirectoryIterator fileIt(folder);
		Poco::DirectoryIterator fileEnd;
		if (fileIt == fileEnd) {
			label++;
			it++;
			continue;
		}

		uint32_t files = 0;
		while (fileIt != fileEnd) {
			auto feat = _feature(fileIt.path().toString());
			if (feat.empty()) {
				id++;
				fileIt++;
				continue;
			}
			similarity::Object obj(id, label, sizeof(float)*feat.size(), &feat[0]);
			space.WriteNextObj(obj, fmt::format("{0}", label), *output);
			if( mkdb ) {
			ImgRec rec{id, label, files, fileIt.path().toString()};
			storage.insert(rec);
			}
			id++;
			files++;
			fileIt++;
			if (files >= _maxImgPerObj)
				break;
		}

		if(mkdb) {
		LabelRec rec{ label, it.path().toString() };
		storage.insert(rec);
		}

		label++;
		it++; //next folder 

		if (label >= _maxOjbs)
			break;
	}

	return true;
}