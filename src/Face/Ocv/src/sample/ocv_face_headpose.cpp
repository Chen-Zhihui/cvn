// cpptmpl.cpp : Defines the entry point for the application.
//

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Cvn/Apputil/TmplApp.h>
#include <Cvn/Face/Base/Face.h>
#include <nlohmann/json.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/printf.h>
#include <memory>
#include <vector>

using namespace cv;
using namespace cv::face;
using namespace std;
using json = nlohmann::json;
using namespace Cvn::Apputil;



bool myDetector(InputArray image, OutputArray faces, CascadeClassifier *face_cascade)
{
	Mat gray;

	if (image.channels() > 1)
		cvtColor(image, gray, COLOR_BGR2GRAY);
	else
		gray = image.getMat().clone();

	equalizeHist(gray, gray);

	std::vector<Rect> faces_;
	face_cascade->detectMultiScale(gray, faces_, 1.4, 2, CASCADE_SCALE_IMAGE, Size(30, 30));
	Mat(faces_).copyTo(faces);
	return true;
}


void headpose(const cv::Mat & im, const cv::Rect & face, const std::vector<cv::Point2f> & landmarks) {
	// 2D image points. If you change the image, you need to change vector
	std::vector<cv::Point2d> image_points;

	cv::Point2d nose(landmarks.at(31 - 1).x, landmarks.at(31 - 1).y);
	image_points.push_back(nose);    // Nose tip, 31-1
	cv::Point2d chin(landmarks.at(9 - 1).x, landmarks.at(9 - 1).y);
	image_points.push_back(chin);    // Chin, 9-1
	cv::Point2d lefteye(landmarks.at(37 - 1).x, landmarks.at(37 - 1).y);
	image_points.push_back(lefteye);     // Left eye left corner 37-1
	cv::Point2d righteye(landmarks.at(46 - 1).x, landmarks.at(46 - 1).y);
	image_points.push_back(righteye);    // Right eye right corner 46-1
	cv::Point2d leftmouth(landmarks.at(49 - 1).x, landmarks.at(49 - 1).y);
	image_points.push_back(leftmouth);    // Left Mouth corner 49-1
	cv::Point2d rightmouth(landmarks.at(55 - 1).x, landmarks.at(55 - 1).y);
	image_points.push_back(rightmouth);    // Right mouth corner 55-1

	// 3D model points.
	std::vector<cv::Point3d> model_points;
	model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
	model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
	model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
	model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
	model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
	model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

	// Camera internals
	double focal_length = im.cols; // Approximate focal length.
	Point2d center = cv::Point2d(face.x + face.width/2, face.y + face.height/2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion

	cout << "Camera Matrix " << endl << camera_matrix << endl;
	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;

	// Solve for pose
	cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);


	// Project a 3D point (0, 0, 1000.0) onto the image plane.
	// We use this to draw a line sticking out of the nose

	vector<Point3d> nose_end_point3D;
	vector<Point2d> nose_end_point2D;
	nose_end_point3D.push_back(Point3d(0, 0, 1000.0));

	projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);


	for (int i = 0; i < image_points.size(); i++)
	{
		circle(im, image_points[i], 3, Scalar(0, 0, 255), -1);
	}

	cv::line(im, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);

	cout << "Rotation Vector " << endl << rotation_vector << endl;
	cout << "Translation Vector" << endl << translation_vector << endl;
}


class DetectorApp : public TmplApp {
public:
	virtual void dumpParamList(std::ostream& ostr) const {
		json j2 =
		{
		{"opencv_dir", "E:\\ws.model\\opencv"},
		{"in_dir", "E:\\testdata\\test-head"},
		{"out_dir", "E:\\testdata\\test-head-out-headpose2"},
		{"force_output", true},
		{"detector_model", "etc\\haarcascades\\haarcascade_frontalface_alt2.xml"},
		{"lbf_model", "E:\\ws.model\\GSOC2017\\data\\lbfmodel.yaml"}
		};
		ostr << j2.dump() << std::endl;
	}

	std::shared_ptr<cv::face::CParams> cparams;

	cv::CascadeClassifier face_cascade;

	int main(const Poco::Util::Application::ArgVec & args) {
		printConfig(this->configFile());

		Poco::Path face_cascade_model_path(configFile().getString("opencv_dir"));
		face_cascade_model_path.append(configFile().getString("detector_model"));

		cparams.reset(new cv::face::CParams(face_cascade_model_path.toString()));


		Poco::Path in_dir(configFile().getString("in_dir"));   in_dir = in_dir.makeDirectory();
		Poco::Path out_dir(configFile().getString("out_dir")); out_dir = out_dir.makeDirectory();

		Poco::File out(out_dir);
		if (out.exists()) {
			if (configFile().getBool("force_output") && out.isDirectory()) {
			}
			else {
				std::cerr << "out put directory already exists, failed" << std::endl;
				logger().information("out put directory already exists, failed");
				return -1;
			}
		}
		out.createDirectories();

		Poco::File in(in_dir);
		if (!in.exists() || in.isFile() ) {
			std::cerr << "input dir doesnot exists" << std::endl;
			logger().information("input dir donnot exists");
			return  -1;
		}

		std::vector<std::string> files;
		in.list(files);

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
		// Load landmark detector
		facemark->loadModel(lbf_path.toString());

		int count = 0;
		for (auto & in_file : files) {
			Poco::Path file_path(in_dir);
			file_path.append(in_file);
			auto dir = file_path.makeDirectory();
			auto file = file_path.makeFile();
			if (!Poco::File(file).exists())
				continue;

			std::vector<cv::Rect> faces;
			cv::Mat input = cv::imread(Poco::Path(in_dir).append(in_file).toString());
			Poco::Path out_file(out_dir);
			out_file.append(in_file);
			out_file.makeFile();
					   
			cv::face::getFaces(input, faces, cparams.get());
			auto out = input.clone();
			cvn::Face::Base::drawFaceRecs(out, faces, Scalar(0, 255, 0));

			auto msg = fmt::format("detected {:04d} faces in file {}", faces.size(), Poco::Path(in_dir).append(in_file).toString());
			logger().information(msg);
			std::cout << msg << endl;

			std::vector<std::vector<Point2f> > landmarks;
			if( !faces.empty())
				auto ret = facemark->fit(input, faces, landmarks);
			if (!landmarks.empty() ) {
				for (int j = 0; j < landmarks.size(); j++) {
				//	cvn::Face::Base::drawFacemark(out, landmarks[j], Scalar(0, 0, 255));
					headpose(out, faces.at(j), landmarks.at(j));
				}
			}
			else {
				auto msg = fmt::format("landmark faces failed in file {}", Poco::Path(in_dir).append(in_file).toString());
			}

			cv::imwrite(out_file.toString(), out);
			count++;
		}
		return 0;
	}
};


POCO_APP_MAIN(DetectorApp)
