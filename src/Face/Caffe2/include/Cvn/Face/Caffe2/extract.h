
#pragma once

/*
walk throught img dir, extract feature, and save to nmslib object
*/

#include <functional>
#include <vector>
#include <string>
#include <sqlite_orm/sqlite_orm.h>

class Extracter {
public:
	typedef std::function< std::vector<float>(const std::string &)> feature_fun;

	//typedef std::function< similarity::Object * (const std::string &, int32_t, int32_t) > obj_fun;

	Extracter(feature_fun feature, size_t maxobjs = 0, size_t maxImgPerObj = 0);

	class ImgRec {
	public:
		int32_t id;
		int32_t label;
		std::string externalId;
		int32_t file_index_inFolder;
		std::vector<char> data;
		std::string path;

		static decltype(auto) make_table(void) {
			return sqlite_orm::make_table<ImgRec>("imgrec",
				sqlite_orm::make_column("id", &ImgRec::id), //, sqlite_orm::primary_key()),
				sqlite_orm::make_column("label", &ImgRec::label),
				sqlite_orm::make_column("externalId", &ImgRec::externalId),
				sqlite_orm::make_column("file_index_inFolder", &ImgRec::file_index_inFolder),
				sqlite_orm::make_column("data", &ImgRec::data),
				sqlite_orm::make_column("path", &ImgRec::path));
		}
	};

	class LabelRec {
	public:
		int32_t label;
		std::string path; //folder path

		static decltype(auto) make_table(void) {
			return sqlite_orm::make_table<LabelRec>("labelrec",
				sqlite_orm::make_column("label", &LabelRec::label), //, sqlite_orm::primary_key()),
				sqlite_orm::make_column("path", &LabelRec::path));
		}
	};
	/*
	遍历一个目录，得到以下文件, 更好的处理办法是把结构化数据放在数据库中
	idxLogFile, list<tuple<file_index_global, folder_index=label, file_index_in_folder, filepath>
	labelFile,  list<tuple<folder_index=label, folder_path>
	dataSetFile, dataset<tuple<file_index_global=index, folder_index=label, feature>>

	inputdir
	*/
	bool extract(const std::string & inputdir, const std::string & dbFile, const std::string & dateSetFile, bool mkdb=false) const;

	//bool extract(const std::string & inputdir, std::vector<ImgRec> & idxLog, std::vector<LabelRec> & labels, similarity::ObjectVector & dataset) const;

private:

	size_t _maxOjbs;
	size_t _maxImgPerObj;
	feature_fun _feature;
};
