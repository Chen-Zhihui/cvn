
// cpptmpl.cpp : Defines the entry point for the application.
//

#include <caffe2/core/logging.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Util/ConfigurationView.h>
#include <Cvn/Apputil/TmplApp.h>
#include <Cvn/Face/Base/Face.h>
#include <Cvn/Face/Caffe2/extract.h>
#include <Cvn/Face/Caffe2/sphereface.h>
#include <nlohmann/json.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/face.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <memory>
#include <nmslib/object.h>
#include <Cvn/Face/Caffe2/extract.h>
#include <nmslib/space.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include "Poco/DirectoryIterator.h"
#include <nmslib/space/space_vector.h>
#include <nmslib/space/space_vector_gen.h>
#include <nmslib/params.h>
#include <nmslib/init.h>
#include <nmslib/methodfactory.h>
#include <nmslib/spacefactory.h>
#include <space/space_word_embed.h>

#include <nmslib/rangequery.h>
#include <nmslib/knnquery.h>
#include <nmslib/knnqueue.h>

#include <H5Cpp.h>
#include <highfive/H5DataSet.hpp>
#include <highfive/H5DataSpace.hpp>
#include <highfive/H5File.hpp>
#include <Eigen/Eigen>

using namespace cv;
using namespace face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;

class DetectorApp : public TmplApp
{
  public:
	virtual void dumpParamList(std::ostream &ostr) const
	{
		json j2 =
		{ {"extractor" , {
			 {"input_dir", "E:\\testdata_inuse\\webfaces-imgs"},
			 {"output_dir", "E:\\ws.cvn"},
			 {"h5file", "webfaces-500by100.h5"},
			 {"h5query", "webfaces-500by100-query.h5"},
			 {"indexfile", "webfaces-500by100.index"},
			 {"maxFolder", 500},
			 {"maxImg", 100}},
			 },
		{"facemodel" , {
				 {"dir", "E:\\ws_face.model\\caffe2\\sphereface"},
				 {"predict_net", "sphereface_20_predict_net.pb"},
				 {"init_net", "sphereface_20_init_net.pb"},
				 {"input_names", {"data"}},
				 {"output_names", {"fc5"}}
			 }}
			};
		ostr << j2.dump() << std::endl;
	}

	int main(const Poco::Util::Application::ArgVec &args)
	{
		//printConfig(this->configFile());

		similarity::initLibrary(0, LIB_LOGNONE, NULL);

		auto extractorConfig = configFile().createView("extractor");

		printConfig(*extractorConfig);

		Poco::Path in_dir(extractorConfig->getString("input_dir"));
		in_dir = in_dir.makeDirectory();
		Poco::Path out_dir(extractorConfig->getString("output_dir"));
		out_dir = out_dir.makeDirectory();

		auto h5File = out_dir; h5File.append(extractorConfig->getString("h5file", "h5file.h5"));
		auto indexfile = out_dir; indexfile.append(extractorConfig->getString("indexfile", "indexfile.index"));
		auto queryFile = out_dir; queryFile.append(extractorConfig->getString("h5query", "h5query.h5"));

		auto maxFolder = extractorConfig->getInt64("maxFolder", 10);
		auto maxImg = extractorConfig->getInt64("maxImg", 10);

		Poco::File out(out_dir);
		out.createDirectories();

		Poco::File in(in_dir);
		if (!in.exists() || in.isFile())
		{
			std::cerr << "input dir doesnot exists" << std::endl;
			logger().information("input dir donnot exists");
			return -1;
		}

		//open hdf5, get data
		//dataset
		//

		similarity::WordEmbedSpace<float> space(similarity::kEmbedDistL2);
		similarity::ObjectVector ov;

		size_t feature_size = 512;
		size_t object_size = feature_size + 2;
		hsize_t row = 0;
		int32_t id = 0;
		int32_t label = 0;

		//std::vector<float> stat_data(10);
		//std::vector<float> feat_temp(feature_size);
		//similarity::Object obj(id, label, sizeof(float) * feature_size, &feat_temp[0]);
		///size_t buffer_size = obj.bufferlength();
		//similarity::ObjectVector objSet;


		HighFive::File h5(h5File.toString(), HighFive::File::ReadOnly );
		std::vector<std::vector<float>> features;
		std::vector<uint32_t> labels;

		const std::string DATASET_NAME_object("object");
		const std::string DATASET_NAME_rawdata("rawdata");
		const std::string DATASET_NAME_label("label");
		const std::string DATASET_NAME_stat("stat");

		auto ds_feat = h5.getDataSet(DATASET_NAME_rawdata);
		ds_feat.read(features);
		auto ds_label = h5.getDataSet(DATASET_NAME_label);
		ds_label.read(labels);

		poco_assert(features.size() == labels.size());
		for (int32_t id = 0; id < labels.size(); id++) {
			auto v = Eigen::VectorXf::Map(&features[id][0], feature_size);
			v.normalize(); // L2归一化
			similarity::Object * obj = new similarity::Object(id, labels.at(id), sizeof(float)*feature_size, &features[id][0]);
			ov.push_back(obj);
		}

	//index
		similarity::AnyParams IndexParams(
			{
			"lsh_family=cross_polytope",
			"storage_hash_table=flat_hash_table",
			"num_rotations=2",
			"norm_data=false",
			"num_hash_tables=4" /* para.l */
			});

		similarity::AnyParams QueryTimeParams(
			{
			"num_probes=896",
			}
		);

		similarity::Index<float>*   indexSmallWorld =
			similarity::MethodFactoryRegistry<float>::Instance().CreateMethod(true /* print progress */,
				"lsh_cauchy",
				"l1",
				space,
				ov
			);

		indexSmallWorld->CreateIndex(IndexParams);

		//indexSmallWorld->SaveIndex(indexfile.toString());

		indexSmallWorld->SetQueryTimeParams(QueryTimeParams);

		HighFive::File query(queryFile.toString(), HighFive::File::ReadWrite | HighFive::File::Create | HighFive::File::Truncate);
		unsigned K = 10; // 5-NN query
		std::vector<hsize_t> dim;
		dim.push_back(maxImg * maxFolder);
		dim.push_back(K+1);
		HighFive::DataSpace resspace(dim);
		dim.clear();
		HighFive::DataSet labelset = query.createDataSet(DATASET_NAME_label, resspace , HighFive::AtomicType<int32_t>()); // , props);


		std::vector<int32_t> result; result.reserve(K+1);
		auto printResults = [K, &labels] (similarity::KNNQuery<float>* qobj) -> std::vector<int32_t> {
			std::vector<int32_t>  resvec;
			resvec.reserve(K+1);
			similarity::KNNQueue<float>* res = qobj->Result()->Clone();
			while (!res->Empty()) {
				cout << res->TopObject()->id() << " : " << res->TopDistance() << endl;
				resvec.push_back(res->TopObject()->label());
				res->Pop();
			}
			delete res;
			resvec.push_back(qobj->QueryObject()->label());
			return std::move(resvec);
		};

		hsize_t rowout = 0;
		int32_t start = 0;
		for (const similarity::Object * queryObj : ov) {
			similarity::KNNQuery<float>   knnQ(space, queryObj, K);
			indexSmallWorld->Search(&knnQ, start);
			result = printResults(&knnQ);
			labelset.select({ rowout, 0 }, { 1, K+1 }).write(result);
			rowout++;
		}

		return 0;
	}
};

POCO_APP_MAIN(DetectorApp)
