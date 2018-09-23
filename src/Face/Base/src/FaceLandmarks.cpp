
#include <Cvn/Face/Base/FaceLandmarks.h>
#include <Poco/SingletonHolder.h>

namespace Cvn {
    namespace Face {
        namespace Base {

            FaceLandmarks::FaceLandmarks(){
                // init reference facial points
                _ref_96_112_5pts.reserve(5);
                _ref_96_112_5pts.emplace_back(29.2946, 50.6963); // as c++ or python index start from 0
                _ref_96_112_5pts.emplace_back(64.5318, 50.5014); // w.r.t matlab index start from 1
                _ref_96_112_5pts.emplace_back(47.0252, 70.7366); // this will result a minus one in landmarks
                _ref_96_112_5pts.emplace_back(32.5493, 91.3655);
                _ref_96_112_5pts.emplace_back(61.7299, 91.2041);

                _ref_112_112_5pts.reserve(5);
                for (const auto & pt : _ref_96_112_5pts)
                    _ref_112_112_5pts.emplace_back(pt.x + 8.f, pt.y);
            }

            const FaceLandmarks & FaceLandmarks::instance()
            {
                static Poco::SingletonHolder<FaceLandmarks> sh;
                return *sh.get();
            }
        }
    }
}