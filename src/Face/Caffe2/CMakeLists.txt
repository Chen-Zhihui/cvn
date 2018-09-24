
set(package_name FaceCaffe2)

add_library ( ${package_name} STATIC 
src/mtcnn.cpp 
src/sphereface.cpp
include/${PROJECT_NAME}/Face/Caffe2/mtcnn.h
include/${PROJECT_NAME}/Face/Caffe2/sphereface.h
)

target_link_libraries(${package_name} PUBLIC opencv_core)

set_target_properties(${package_name} PROPERTIES 
    OUTPUT_NAME ${PROJECT_NAME}${package_name})
    
target_include_directories(${package_name} 
	PRIVATE ${OpenCV_INCLUDE_DIRS}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install support

PACKAGE_INSTALL(${package_name})

add_subdirectory(samples)