

set(dep_ins 
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Base/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocv/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Caffe2/include>
)


###
add_executable (ocv_feature_extract
ocv_feature_extract.cpp 
)

target_link_libraries(ocv_feature_extract 
PRIVATE Apputil FaceBase FaceOcv FaceCaffe2 ${OpenCV_LIBRARIES}  fmt::fmt glog::glog
)
target_include_directories( ocv_feature_extract
PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
)

EXECUTABLE_INSTALL(ocv_feature_extract)

