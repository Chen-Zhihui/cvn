


* build
** cmake

vcpkg install poco nlohmann-json tl-expected Catch2 fmt sqlite3 sqlite-orm asyncplusplus zlib bzip2 expat pcre eigen3 rxcpp gsl openblas hdf5 highfive 

vcpkg install poco:x64-windows nlohmann-json:x64-windows tl-expected:x64-windows Catch2:x64-windows fmt:x64-windows sqlite3:x64-windows sqlite-orm:x64-windows asyncplusplus:x64-windows zlib:x64-windows bzip2:x64-windows expat:x64-windows pcre:x64-windows eigen3:x64-windows rxcpp:x64-windows gsl:x64-windows openblas:x64-windows hdf5:x64-windows highfive:x64-windows 

-DCMAKE_TOOLCHAIN_FILE=/home/tony/data/ws.play/vcpkg/scripts/buildsystems/vcpkg.cmake
-DOpenCV_DIR=/home/tony/ws.local/ep.install.opencv/share/OpenCV
-DQt5_DIR=/opt/Qt/5.11.0/gcc_64/lib/cmake/Qt5
-DCaffe2_DIR=/home/tony/data/pytorch-v1.0rc1-install/share/cmake/Caffe2


** run
*** windows
export PATH=$PATH:/c/Qt/Qt5.11.0/5.11.0/msvc2017_64/bin/
windeployqt.exe sample_apputil_qt.exe