
#pragma once

/*
walk throught img dir, extract feature, and save to nmslib object
*/

#include <nmslib/object.h>
#include <functional>
#include <vector>
#include <string>

class Extracter {
public:
	typedef std::function< std::vector<float>(const std::string &)> feature_fun;

	typedef std::function< similarity::Object * (const std::string &, int32_t, int32_t) > obj_fun;

	Extracter(feature_fun feature, size_t maxobjs = 0, size_t maxImgPerObj = 0);

	class ImgRec {
	public:
		int32_t label;
		int32_t file_index_inFolder;
		int32_t file_index_global;
		std::string filepath;
	};

	class LabelRec {
	public:
		int32_t label;
		std::string folderpath;
	};
	/*
	遍历一个目录，得到以下文件
	idxLogFile, list<tuple<folder_index=label, file_index_in_folder, file_index_global, filepath>
	labelFile,  list<tuple<folder_index=label, folder_path>
	dataSetFile, dataset<tuple<file_index_global=index, folder_index=label, feature>>

	inputdir
	*/
	bool extract(const std::string & inputdir, const std::string & idxLogFile, const std::string & labelFile, const std::string & dateSetFile) const;

	bool extract(const std::string & inputdir, std::vector<ImgRec> & idxLog, std::vector<LabelRec> & labels, similarity::ObjectVector & dataset) const;

private:

};
