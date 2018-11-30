


* build
** cmake

vcpkg install poco nlohmann-json tl-expected Catch2 fmt sqlite3 sqlite-orm asyncplusplus zlib bzip2 expat pcre eigen3 rxcpp gsl openblas hdf5[cpp] highfive protobuf nmslib

-DCMAKE_TOOLCHAIN_FILE=/home/tony/data/ws.play/vcpkg/scripts/buildsystems/vcpkg.cmake
-DOpenCV_DIR=/home/tony/ws.local/ep.install.opencv/share/OpenCV
-DQt5_DIR=/opt/Qt/5.11.0/gcc_64/lib/cmake/Qt5
-DCaffe2_DIR=/home/tony/data/pytorch-v1.0rc1-install/share/cmake/Caffe2


** run
*** windows
export PATH=$PATH:/c/Qt/Qt5.11.0/5.11.0/msvc2017_64/bin/
windeployqt.exe sample_apputil_qt.exe