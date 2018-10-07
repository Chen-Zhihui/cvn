
#include <Cvn/Face/Caffe2/sphereface.h>
#include <Cvn/Caffe2util/CPredictor.h>
#include <Poco/Bugcheck.h>
#include <opencv/cv.hpp>

SperefaceFeature::SperefaceFeature(){
    caffe2::TensorCPU input;
    input.Resize(std::vector<int>({bs, nc, input_height, input_width96}));
    input_vec["data"] = input;
}

bool SperefaceFeature::init(const std::string & configjs) {
    predictor = std::make_shared<CPredictor>();
    return predictor->load(configjs);
}

void set_input_buffer(std::vector<cv::Mat>& input_channels,
		float* input_data, const int height, const int width) 
{
	for (int i = 0; i < 3; ++i) {
		cv::Mat channel(height, width, CV_32FC1, input_data);
		input_channels.push_back(channels);
		input_data += width * height;
	}
}

std::vector<float> SperefaceFeature::feature(const cv::Mat & aligned ) const  {
    poco_assert(input_width == aligned.cols || input_width96 == aligned.cols );
    poco_assert(input_height== aligned.rows);
    poco_assert(nc == aligned.channels());

    cv::Mat3f copy = (aligned-127.5)/128.0;
    if( input_width == aligned.cols ) {
        cv::Rect roi(cv::Point(8,0), cv::Size(96,112));
        copy = cv::Mat(copy, roi);
    }
    
    std::vector<cv::Mat> channels;
    const caffe2::TensorCPU & input(input_vec["data"]);
    set_input_buffer(channels, input.mutable_data<float>(), input_height, input_width96);
    cv::split(copy, channels);

    caffe2::Predictor::TensorMap output_vec;
    (*predictor)(input_vec, &output_vec);
    caffe2::TensorCPU & outdata = output_vec["fc5"];

    poco_assert(outdata.ndim() == 2);
    auto dims = outdata.dims();
    poco_assert(dims[0] == bs);
    poco_assert(dims[1] == 512);

    return std::vector<float>(outdata.data<float>(), outdata.data<float>()+dims[1]);
}