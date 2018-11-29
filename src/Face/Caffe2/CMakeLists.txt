
set(package_name FaceCaffe2)

set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Caffe2util/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Base/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocv/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocvmtcnn/include>
)

add_library ( ${package_name} STATIC 
src/mtcnn.cpp 
src/sphereface.cpp
src/extract.cpp
include/${PROJECT_NAME}/Face/Caffe2/mtcnn.h
include/${PROJECT_NAME}/Face/Caffe2/sphereface.h
include/${PROJECT_NAME}/Face/Caffe2/extract.h
)

target_link_libraries(${package_name} 
PUBLIC opencv_core Caffe2util Poco::Util 
  nmslib::nmslib 
  #nmslib::lshkit 
  sqlite3 fmt::fmt HighFive hdf5::hdf5_cpp-shared)

target_include_directories( ${package_name}
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
	)
	
set_target_properties(${package_name} PROPERTIES 
    OUTPUT_NAME ${PROJECT_NAME}${package_name})
    
target_include_directories(${package_name} 
	PRIVATE ${OpenCV_INCLUDE_DIRS}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install support

PACKAGE_INSTALL(${package_name})

add_subdirectory(sample)