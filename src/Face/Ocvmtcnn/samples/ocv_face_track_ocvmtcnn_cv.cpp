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
#include <algorithm>
#include <Cvn/Face/Ocvmtcnn/detector.h>

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

using rectPoints = std::pair<cv::Rect, std::vector<cv::Point>>;

static cv::Mat drawRectsAndPoints(const cv::Mat &img,
                                  const std::vector<rectPoints> data) {
  cv::Mat outImg;
  img.convertTo(outImg, CV_8UC3);

  for (auto &d : data) {
    cv::rectangle(outImg, d.first, cv::Scalar(0, 0, 255));
    auto pts = d.second;
    for (size_t i = 0; i < pts.size(); ++i) {
      cv::circle(outImg, pts[i], 3, cv::Scalar(0, 0, 255));
    }
  }
  return outImg;
}

class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 =
		{
		{"opencv_dir", "E:\\ws.model\\opencv"},
		{"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"},
		{"ocvmtcnn_dir", "E:\\ws.model\\opencv-mtcnn"},
		{"tracker", "KCF"},
		//{"in_file", "E:\\testdata\\clip.mp4"},
		//{"out_file", "E:\\testdata\\clip-ocvmtcnn-cv.mp4"},
		{"in_file", "E:\\testdata\\yongzheng-01-03.mp4"},
		{"out_file", "E:\\testdata\\yongzheng-01-03-ocvmtcnn-cv.mp4"},
		{"force_output", true},
		{"skip", 8}
		};
		ostr << j2.dump() << std::endl;
	}

	std::shared_ptr<cv::face::CParams> cparams;

	int main(const Poco::Util::Application::ArgVec & args) {
		printConfig(this->configFile());

		//detector
		Poco::Path face_cascade_model_path(configFile().getString("opencv_dir"));
		face_cascade_model_path.append(configFile().getString("detector_model"));
		cparams.reset(new cv::face::CParams(face_cascade_model_path.toString()));

		//detector
		Poco::Path lmtcnn_model_path(configFile().getString("ocvmtcnn_dir"));
		lmtcnn_model_path.makeDirectory();
		std::string modelDir = lmtcnn_model_path.toString();
		ProposalNetwork::Config pConfig;
		pConfig.caffeModel = modelDir + "/det1.caffemodel";
		pConfig.protoText = modelDir + "/det1.prototxt";
		pConfig.threshold = 0.6f;

		RefineNetwork::Config rConfig;
		rConfig.caffeModel = modelDir + "/det2.caffemodel";
		rConfig.protoText = modelDir + "/det2.prototxt";
		rConfig.threshold = 0.7f;

		OutputNetwork::Config oConfig;
		oConfig.caffeModel = modelDir + "/det3.caffemodel";
		oConfig.protoText = modelDir + "/det3.prototxt";
		oConfig.threshold = 0.7f;

  		MTCNNDetector detector(pConfig, rConfig, oConfig);

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
		//tracking 
		// Specify the tracker type
		string trackerType = configFile().getString("tracker", "KCF");
		// Create multitracker
		Ptr<MultiTracker> multiTracker = cv::MultiTracker::create();

		bool mtOk = false;
		long long frameCount = -1;
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

					auto casRec = faces;
					cv::face::getFaces(frame, casRec, cparams.get());	

					for(auto & r : casRec) {
						cv::Rect rr(r);
						int det = rr.width * 0.5;
						rr.x = std::max(0, rr.x - det);
						rr.y = std::max(0, rr.y - det);
						rr.width = std::min( rr.width * 2, frame.cols-rr.x);
						rr.height = std::min(rr.height* 2, frame.rows-rr.y);

						cv::Mat fMat(frame, rr);

						auto faces_mtcnn = detector.detect(fMat.clone(), 20.f, 0.709f);
			        	std::vector<rectPoints> data;
						// show the image with faces in it
						for (size_t i = 0; i < faces_mtcnn.size(); ++i) {
							std::vector<cv::Point> pts;
							for (int p = 0; p < NUM_PTS; ++p) {
								pts.push_back(
								cv::Point(faces_mtcnn[i].ptsCoords[2 * p], faces_mtcnn[i].ptsCoords[2 * p + 1]));
							}

							auto rect = faces_mtcnn[i].bbox.getRect();
							auto d = std::make_pair(rect, pts);
							data.push_back(d);

							faces.push_back(cv::Rect(rect.tl()+rr.tl(), cv::Size(rect.width, rect.height)));
						}
					}

			  		//auto resultImg = drawRectsAndPoints(frame, data);

					//auto msg_face = "face detected with count = {}"_format(faces.size());
					//logger().information(msg_face);
					//cout << msg_face << endl;

					// todo
					if (!faces.empty()) {
						multiTracker.release();
						multiTracker = cv::MultiTracker::create();
						// Initialize multitracker
						for (int i = 0; i < faces.size(); i++)
							multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(faces[i]));

						//auto msg = "multi tracker reinitialized at frame = {}"_format(frameCount);
						//logger().information(msg);
						//cout << msg << endl;

						mtOk = true; // multiTracker->update(frame);

						//cvn::Face::Base::drawFaceRecs(resultImg, faces, Scalar(0, 255, 0));
					}
					detectCount++;

					cvn::Face::Base::drawFaceRecs(frame, faces, Scalar(0, 255, 0));
				}
			}
			else {
				cvn::Face::Base::drawFaceRecs(frame, faces, Scalar(255, 0, 0));
			}


			// Show frame
			//imshow("MultiTracker", frame);

			writer << frame;

			{
				if (frameCount % 25 == 0) {
					double dur = ((double)getTickCount() - start) / getTickFrequency();
					auto msg = "frame id={}, face count={}, detect index={}, track index={}, frame rate = {}"_format(frameCount, faces.size(), detectCount, trackCount, frameCount / dur);
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
