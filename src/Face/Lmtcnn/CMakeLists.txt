#openblas

include_directories(${OpenBLAS_INCLUDE_DIR})


set(package_name FaceLmtcnn)

add_library ( ${package_name} STATIC 
src/mtcnn.cpp
src/network.cpp
src/pBox.cpp
include/${PROJECT_NAME}/Face/Lmtcnn/mtcnn.h
include/${PROJECT_NAME}/Face/Lmtcnn/network.h
include/${PROJECT_NAME}/Face/Lmtcnn/pBox.h
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