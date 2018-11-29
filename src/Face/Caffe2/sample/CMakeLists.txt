

set(dep_ins 
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Base/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocv/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Caffe2/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/3rdparty/FALCONN/src/include>
)


###
add_executable (ocv_feature_extract
ocv_feature_extract.cpp 
)

target_link_libraries(ocv_feature_extract
        FaceBase FaceOcv FaceCaffe2 ${OpenCV_LIBRARIES} Apputil  Poco::Foundation
        fmt::fmt
        )
target_include_directories( ocv_feature_extract
PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
)

EXECUTABLE_INSTALL(ocv_feature_extract)



###
add_executable (ocv_feature_extract_h5
ocv_feature_extract_h5.cpp 
)

target_link_libraries(ocv_feature_extract_h5
        FaceBase FaceOcv FaceCaffe2 ${OpenCV_LIBRARIES} Apputil  Poco::Foundation
        fmt::fmt
        )
target_include_directories( ocv_feature_extract_h5
PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
)

EXECUTABLE_INSTALL(ocv_feature_extract_h5)




###
add_executable (ocv_feature_index
ocv_feature_index_falconn.cpp 
)

target_link_libraries(ocv_feature_index
        FaceBase FaceOcv FaceCaffe2 ${OpenCV_LIBRARIES} Apputil  Poco::Foundation
        fmt::fmt GSL::gsl 
		#nmslib::lshkit
        )
target_include_directories( ocv_feature_index
PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
)
if(MSVC)
target_compile_options(ocv_feature_index PRIVATE /bigobj)
endif()

EXECUTABLE_INSTALL(ocv_feature_index)
