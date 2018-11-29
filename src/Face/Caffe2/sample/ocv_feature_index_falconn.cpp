
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

#include "falconn/eigen_wrapper.h"
#include "falconn/lsh_nn_table.h"

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
		std::vector<falconn::DenseVector<float>> falconn_data;
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

			falconn_data.push_back(v);
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
		auto printResults = [] (similarity::KNNQuery<float>* qobj) -> std::vector<int32_t> {
			std::vector<int32_t>  resvec;
			similarity::KNNQueue<float>* res = qobj->Result()->Clone();
			while (!res->Empty()) {
				resvec.push_back(res->TopObject()->id());
				res->Pop();
			}
			delete res;
			return std::move(resvec);
		};

		auto reduce_by_distance = [&features, feature_size, K](const std::vector<int32_t> & idxCandidate, const std::vector<float> & feat ) -> std::vector<int32_t> {
			std::vector<std::pair<float, size_t> > dists_idxs;
			auto q = Eigen::VectorXf::Map(&feat[0], feature_size);
			for (int i = 0; i < idxCandidate.size(); i++) {
				size_t row = idxCandidate[i];
				auto p = Eigen::VectorXf::Map(&features[row][0], feature_size);
				float tmp_cosine_dist = q.dot(p);
				dists_idxs.push_back(std::make_pair(tmp_cosine_dist, row));
			}
			std::sort(dists_idxs.begin(), dists_idxs.end(), [](const std::pair<float, size_t> & left, const std::pair<float, size_t> & right) {
				return left.first > right.first;
			});
			//std::reverse(dists_idxs.begin(), dists_idxs.end());

			std::vector<int32_t> result;
			for (int i = 0; i < K; i++) {
				result.push_back((int32_t)dists_idxs[i].second);
			}
			return result;
		};

		auto idx2label = [&labels](const std::vector<int32_t> & ids) -> std::vector<int32_t> {
			std::vector<int32_t> labs;
			for (int i = 0; i < ids.size(); i++) {
				labs.push_back(labels[ids[i]]);
			}
			return labs;
		};

		hsize_t rowout = 0;
		int32_t start = 0;

		for (const similarity::Object * queryObj : ov) {
			similarity::KNNQuery<float>   knnQ(space, queryObj, 50);
			indexSmallWorld->Search(&knnQ, start);
			result = printResults(&knnQ);
			result = reduce_by_distance(result, features[rowout]);
			result = idx2label(result);
			poco_assert(result.size() == K);
			result.push_back(queryObj->label());
			labelset.select({ rowout, 0 }, { 1, K+1 }).write(result);
			rowout++;
		}



		HighFive::DataSet falconnset = query.createDataSet(std::string("falconn"), resspace, HighFive::AtomicType<int32_t>());
		{
			falconn::LSHConstructionParameters params_cp;
			unique_ptr<falconn::LSHNearestNeighborTable<falconn::DenseVector<float>>> cptable;
			// Common LSH parameters
			int num_tables = 8;
			int num_setup_threads = 0;
			uint64_t seed = 119417657;
			falconn::StorageHashTable storage_hash_table = falconn::StorageHashTable::FlatHashTable;
			falconn::DistanceFunction distance_function = falconn::DistanceFunction::NegativeInnerProduct;
			// Cross polytope hashing
			params_cp.dimension = feature_size;
			params_cp.lsh_family = falconn::LSHFamily::CrossPolytope;
			params_cp.distance_function = distance_function;
			params_cp.storage_hash_table = storage_hash_table;
			params_cp.k = 2; // 每个哈希表的哈希函数数目
			params_cp.l = num_tables; // 哈希表数目
			params_cp.last_cp_dimension = 2;
			params_cp.num_rotations = 2;
			params_cp.num_setup_threads = num_setup_threads;
			params_cp.seed = seed ^ 833840234;
			cptable = unique_ptr<falconn::LSHNearestNeighborTable<falconn::DenseVector<float>>>(std::move(falconn::construct_table<falconn::DenseVector<float>>(falconn_data, params_cp)));
			cptable->set_num_probes(896);

			rowout = 0;
			for (int32_t id = 0; id < labels.size(); id++) {
				try {
					auto q = Eigen::VectorXf::Map(&features[id][0], feature_size);
					//q.normalize(); // L2归一化
					std::vector<int32_t> idxCandidate;
					cptable->find_k_nearest_neighbors(q, 50, &idxCandidate); // LSH find the K nearest neighbors
					std::vector<int32_t> result = reduce_by_distance(idxCandidate, features[id]);
					result = idx2label(result);
					result.push_back(labels[id]);
					falconnset.select({ rowout++, 0 }, { 1, K + 1 }).write(result);
				}
				catch (...) {
					rowout++;
					logger().information("exception");
				}
			}
		}

		return 0;
	}
};

POCO_APP_MAIN(DetectorApp)
