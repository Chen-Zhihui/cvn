// cpptmpl.cpp : Defines the entry point for the application.
//

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Cvn/Apputil/TmplApp.h>
#include <Cvn/Face/Base/Face.h>
#include <nlohmann/json.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <memory>
#include <Cvn/Face/Lmtcnn/mtcnn.h>

using namespace cv;
using namespace cv::face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;
using namespace fmt::literals;


vector<string> trackerTypes = { "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT" };

// create tracker by name
Ptr<Tracker> createTrackerByName(string trackerType)
{
	Ptr<Tracker> tracker;
	if (trackerType == trackerTypes[0])
		tracker = TrackerBoosting::create();
	else if (trackerType == trackerTypes[1])
		tracker = TrackerMIL::create();
	else if (trackerType == trackerTypes[2])
		tracker = TrackerKCF::create();
	else if (trackerType == trackerTypes[3])
		tracker = TrackerTLD::create();
	else if (trackerType == trackerTypes[4])
		tracker = TrackerMedianFlow::create();
	else if (trackerType == trackerTypes[5])
		tracker = TrackerGOTURN::create();
	else if (trackerType == trackerTypes[6])
		tracker = TrackerMOSSE::create();
	else if (trackerType == trackerTypes[7])
		tracker = TrackerCSRT::create();
	else {
		cout << "Incorrect tracker name" << endl;
		cout << "Available trackers are: " << endl;
		for (vector<string>::iterator it = trackerTypes.begin(); it != trackerTypes.end(); ++it)
			std::cout << " " << *it << endl;
	}
	return tracker;
}



class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 =
		{
		{"opencv_dir", "E:\\ws.model\\opencv"},
		{"in_file", "E:\\testdata\\yongzheng-01-10.mp4"},
		{"out_file", "E:\\testdata\\yongzheng-01_track-mtcnn.mp4"},
		{"force_output", true},
		{"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"},
		{"lmtcnn_dir", "E:\\ws.model\\Lmtcnn"},
		{"lbf_model", "E:\\ws.model\\GSOC2017\\data\\lbfmodel.yaml"},
		{"tracker", "KCF"},
		{"skip", 6}
		};
		ostr << j2.dump() << std::endl;
	}

	std::shared_ptr<cv::face::CParams> cparams;

	cv::CascadeClassifier face_cascade;

	std::shared_ptr<mtcnn> mtcnn_detector;

	int main(const Poco::Util::Application::ArgVec & args) {
		printConfig(this->configFile());

		//detector
		Poco::Path face_cascade_model_path(configFile().getString("opencv_dir"));
		face_cascade_model_path.append(configFile().getString("detector_model"));
		cparams.reset(new cv::face::CParams(face_cascade_model_path.toString()));

		Poco::Path lmtcnn_model_path(configFile().getString("lmtcnn_dir"));
		Poco::Path lmtcnn_pnet_model_path(lmtcnn_model_path); lmtcnn_pnet_model_path.append("Pnet.txt");
	    Poco::Path lmtcnn_onet_model_path(lmtcnn_model_path); lmtcnn_onet_model_path.append("Onet.txt");
	    Poco::Path lmtcnn_rnet_model_path(lmtcnn_model_path); lmtcnn_rnet_model_path.append("Rnet.txt");

		if(0) {
		// Create an instance of Facemark
		Ptr<Facemark> facemark = FacemarkLBF::create();
		Poco::Path lbf_path(configFile().getString("lbf_model"));
		Poco::File lbf_file(lbf_path);
		if (!lbf_file.exists() || !lbf_file.isFile()) {
			auto msg = fmt::format("lbf model Not found {}", lbf_path.toString());
			logger().information(msg);
			cerr << msg << endl;
			return -1;
		}
		facemark->loadModel(lbf_path.toString());
	    }

		//infile
		Poco::Path in_file_path(configFile().getString("in_file"));
		Poco::File in_file(in_file_path);
		if (!in_file.exists()) {
			auto msg = fmt::format("input file missing {}", in_file_path.toString());
			logger().information(msg);
			std::cerr << msg << endl;
			return -1;
		}

		//outfile
		Poco::Path out_file_path(configFile().getString("out_file"));
		Poco::File out_file(out_file_path);
		if ( out_file.exists() && !configFile().getBool("force_output") ) {
			auto msg = fmt::format("output file exists {}", out_file_path.toString());
			logger().information(msg);
			std::cerr << msg << endl;
			return -1;
		}

		cv::VideoCapture cap(in_file_path.toString());
		if (!cap.isOpened())
		{
			auto msg = fmt::format("Error opening video file {}", in_file_path.toString());
			logger().information(msg);
			std::cerr << msg << endl;
			return -1;
		}

		cv::Mat frame;
		cap >> frame;

		cv::VideoWriter writer(out_file_path.toString(), cv::VideoWriter::fourcc('F','M','P','4'), 25, frame.size());
		if (!writer.isOpened()) {
			auto msg = fmt::format("Error opening video file {}", out_file_path.toString());
			logger().information(msg);
			std::cerr << msg << endl;
			return -1;
		}

		
		int width = frame.cols;
		int height = frame.rows;
		mtcnn_detector.reset(new mtcnn(height, width, lmtcnn_pnet_model_path.toString(), 
			lmtcnn_rnet_model_path.toString(), lmtcnn_onet_model_path.toString()));


		//tracking 
		// Specify the tracker type
		string trackerType = configFile().getString("tracker", "KCF");
		// Create multitracker
		Ptr<MultiTracker> multiTracker = cv::MultiTracker::create();

		bool mtOk = false;
		long long frameCount = 0;
		long long detectCount = 0;
		long long trackCount = 0;

		int skip = configFile().getInt("skip", 8);

		double start = (double)getTickCount();
		
		std::vector<Rect2d> faces;
		while (cap.isOpened())
		{
			// get frame from the video
			cap >> frame;
			frameCount++;

			// Stop the program if reached end of video
			if (frame.empty()) 
				break;

			if (frameCount % skip == 0) {

				if (mtOk) {
					faces.clear();
					mtOk = multiTracker->update(frame, faces);
					if (mtOk) {
						//todo output time line and face rect
						//maybe failed partly
					}

					if (faces.empty())
						mtOk = false;
					else 
						cvn::Face::Base::drawFaceRecs(frame, faces, Scalar(0, 0, 255));
					trackCount++;
				}
				
				if(!mtOk) {
					// detect
					faces.clear();					
					//cv::face::getFaces(frame, faces, cparams.get());
					auto thirdBbox_ = mtcnn_detector->findFace(frame);
					for (vector<struct Bbox>::iterator it = thirdBbox_.begin(); it != thirdBbox_.end(); it++) {
						if ((*it).exist) {
							rectangle(frame, Point((*it).y1 , (*it).x1), Point((*it).y2, (*it).x2), Scalar(0, 0, 255), 2, 8, 0);
							//for (int num = 0; num < 5; num++)circle(fmat, Point((int)*(it->ppoint + num), (int)*(it->ppoint + num + 5)), 3, Scalar(0, 255, 255), -1);
						    faces.push_back(cv::Rect(it->x1, it->y1, it->x2-it->x1, it->y2-it->y1));
						}
					}

					auto msg_face = "face detected with count = {}"_format(faces.size());
					logger().information(msg_face);
					cout << msg_face << endl;

					// todo
					multiTracker.release();
					multiTracker = cv::MultiTracker::create();
					// Initialize multitracker
					for (int i = 0; i < faces.size(); i++)
						multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(faces[i]));

					auto msg = "multi tracker reinitialized at frame = {}"_format(frameCount);
					logger().information(msg);
					cout << msg << endl;

					mtOk = true; // multiTracker->update(frame);

					cvn::Face::Base::drawFaceRecs(frame, faces, Scalar(0, 255, 0));

					detectCount++;
				}
			}

			cvn::Face::Base::drawFaceRecs(frame, faces, Scalar(255, 0, 0));

			// Show frame
			//imshow("MultiTracker", frame);

			writer << frame;

			{
				if (frameCount % 25 == 0) {
					auto msg = "frame id={}, face count={}, detect index={}, track index={}"_format(frameCount, faces.size(), detectCount, trackCount);
					logger().information(msg);
					cout << msg << endl;

					double dur = ((double)getTickCount() - start) / getTickFrequency();
					msg = "frame rate by track = {} frame/sec"_format(frameCount / dur);
					logger().information(msg);
					cout << msg << endl;
				}
			}

			// quit on x button
			if (waitKey(1) == 27) break;

		}
		return 0;
	}
};


POCO_APP_MAIN(DetectorApp)
