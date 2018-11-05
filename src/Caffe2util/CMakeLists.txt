

# Add source to this project's executable.

set(package_name Caffe2util)

add_library ( ${package_name} STATIC
		src/CPredictor.cpp
		include/${PROJECT_NAME}/${package_name}/CPredictor.h
		)

target_link_libraries(${package_name} PUBLIC caffe2 openblas)

set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})

target_include_directories(${package_name} 
	PRIVATE ${OpenCV_INCLUDE_DIRS}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# TODO: Add tests and install targets if needed.


# Install support

PACKAGE_INSTALL(${package_name})

add_subdirectory(sample)