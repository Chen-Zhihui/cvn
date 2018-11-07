

#include <nmslib/object.h>
#include <Cvn/Face/Caffe2/extract.h>
#include <nmslib/space.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include "Poco/DirectoryIterator.h"
#include <nmslib/space/space_vector.h>
#include <nmslib/space/space_vector_gen.h>
#include <space/space_word_embed.h>
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
	similarity::WordEmbedSpace<float> space(similarity::kEmbedDistL2);
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
	int32_t id = 0;
	int32_t label = 0;
	while (it != end) {
		Poco::Path folder(inputdir, it.name());
		Poco::DirectoryIterator fileIt(folder);
		Poco::DirectoryIterator fileEnd;
		if (fileIt == fileEnd) {
			label++;
			it++;
			continue;
		}

		int32_t files = 1;
		while (fileIt != fileEnd) {
			auto feat = _feature(fileIt.path().toString());
			if (feat.empty()) {
				id++;
				fileIt++;
				continue;
			}
			similarity::Object obj(id, label, sizeof(float)*feat.size(), &feat[0]);
			space.WriteNextObj(obj, fmt::format("{0}", it.name()), *output);
			if( mkdb ) {
				ImgRec rec{ id, label, it.name(), fileIt.name(), files, std::vector<char>(obj.data(), obj.data()+obj.datalength()), fileIt.path().toString() };
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


#include <H5Cpp.h>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>

const std::string DATASET_NAME_object("object");
const std::string DATASET_NAME_label("label");


bool Extracter::mkIndex(const std::string & inputdir, const std::string & indexfile, const std::string & rawDatafile) const {
	similarity::WordEmbedSpace<float> space(similarity::kEmbedDistL2);
	similarity::ObjectVector ov;

	Poco::File indexFile(indexfile);
	if (indexFile.exists())
		indexFile.remove();

	Poco::File hdf5File(rawDatafile);
	if (hdf5File.exists())
		hdf5File.remove();


	//
	size_t feature_size = 512;
	hsize_t row = 0;
	int32_t id = 0;
	int32_t label = 0;
	std::vector<float> feat(feature_size);
	similarity::Object obj(id, label, sizeof(float)*feature_size, &feat[0]);
	size_t buffer_size = obj.bufferlength();
	
	/*
	hsize_t dims[2] = { 1,buffer_size };	        // dataset dimensions at creation
	hsize_t maxdims[2] = { H5S_UNLIMITED, buffer_size };
	H5::H5File h5(rawDatafile, H5F_ACC_TRUNC);
	H5::DataSpace objds(2, dims, maxdims);
	auto dataset_ojb = h5.createDataSet(DATASET_NAME_object, H5::PredType::STD_I32BE, objds);

	hsize_t ldim[1] = { 1 };
	hsize_t lmaxdim[1] = { H5S_UNLIMITED };
	//H5::DataSpace labels(1, ldim, lmaxdim);
	//auto dataset_label = h5.createDataSet(DATASET_NAME_label,  ,labels);
	*/
	HighFive::File h5(rawDatafile, HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);
	HighFive::DataSpace dataspace({ 100, feature_size });
	HighFive::DataSet dataset = h5.createDataSet(DATASET_NAME_object, dataspace, HighFive::AtomicType<float>());

	Poco::DirectoryIterator it(inputdir);
	Poco::DirectoryIterator end;
	while (it != end) {
		Poco::Path folder(inputdir, it.name());
		Poco::DirectoryIterator fileIt(folder);
		Poco::DirectoryIterator fileEnd;
		if (fileIt == fileEnd) {
			label++;
			it++;
			continue;
		}

		int32_t files = 1;
		while (fileIt != fileEnd) {
			auto feat = _feature(fileIt.path().toString());
			if (feat.empty()) {
				id++;
				fileIt++;
				continue;
			}
			similarity::Object obj(id, label, sizeof(float)*feat.size(), &feat[0]);
			//dataset.resize({row+1,feature_size });
			dataset.select({row, 0 }, {1, feature_size }).write(&feat[0]);
			row++;
			//dataset_ojb.write(obj.buffer(), H5::PredType::STD_U32LE,)

			//dataset_label.write(it.name());
			id++;
			files++;
			fileIt++;
			if (files >= _maxImgPerObj)
				break;
		}


		label++;
		it++; //next folder 

		if (label >= _maxOjbs)
			break;
	}

	return true;
}