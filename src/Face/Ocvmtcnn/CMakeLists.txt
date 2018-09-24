#openblas



set(package_name FaceOcvmtcnn)

add_library ( ${package_name} STATIC 
src/detector.cc
src/onet.cc
src/pnet.cc 
src/rnet.cc 
include/${PROJECT_NAME}/Face/Ocvmtcnn/detector.h
include/${PROJECT_NAME}/Face/Ocvmtcnn/face.h
include/${PROJECT_NAME}/Face/Ocvmtcnn/helpers.h
include/${PROJECT_NAME}/Face/Ocvmtcnn/onet.h
include/${PROJECT_NAME}/Face/Ocvmtcnn/pnet.h
include/${PROJECT_NAME}/Face/Ocvmtcnn/rnet.h
)


target_link_libraries(${package_name} PUBLIC opencv_core ${OpenBLAS_LIB})
set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})
target_include_directories(${package_name} 
	PRIVATE ${OpenCV_INCLUDE_DIRS}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install support

PACKAGE_INSTALL(${package_name})


add_subdirectory(samples)