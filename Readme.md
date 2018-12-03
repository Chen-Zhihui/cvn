


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

3rdparty
[submodule "3rdparty/QtAwesome"]
	path = 3rdparty/QtAwesome
	url = https://github.com/gamecreature/QtAwesome.git
[submodule "3rdparty/qt-material-widgets"]
	path = 3rdparty/qt-material-widgets
	url = https://github.com/laserpants/qt-material-widgets.git
[submodule "3rdparty/QDarkStyleSheet"]
	path = 3rdparty/QDarkStyleSheet
	url = https://github.com/ColinDuquesnoy/QDarkStyleSheet.git
[submodule "3rdparty/rxqt"]
	path = 3rdparty/rxqt
	url = https://github.com/tetsurom/rxqt.git
[submodule "3rdparty/qsyncable"]
	path = 3rdparty/qsyncable
	url = https://github.com/benlau/qsyncable.git
[submodule "3rdparty/QHotkey"]
	path = 3rdparty/QHotkey
	url = https://github.com/Skycoder42/QHotkey.git
[submodule "3rdparty/QSettingsDialog"]
	path = 3rdparty/QSettingsDialog
	url = https://github.com/Skycoder42/QSettingsDialog.git
[submodule "3rdparty/QtMvvm"]
	path = 3rdparty/QtMvvm
	url = https://github.com/Skycoder42/QtMvvm.git
[submodule "3rdparty/QPathEdit"]
	path = 3rdparty/QPathEdit
	url = https://github.com/Skycoder42/QPathEdit.git
[submodule "3rdparty/DialogMaster"]
	path = 3rdparty/DialogMaster
	url = https://github.com/Skycoder42/DialogMaster.git
[submodule "3rdparty/QColorEdit"]
	path = 3rdparty/QColorEdit
	url = https://github.com/Skycoder42/QColorEdit.git
