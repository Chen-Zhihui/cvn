

# Add source to this project's executable.

set(package_name Cvutil)

add_library ( ${package_name} STATIC 
src/ImageCapture.cpp
include/${PROJECT_NAME}/${package_name}/ImageCapture.h
)

target_link_libraries(${package_name} PUBLIC opencv_videoio)
set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})
target_include_directories(${package_name} 
	PRIVATE ${OpenCV_INCLUDE_DIRS}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# TODO: Add tests and install targets if needed.


# Install support

Cvn_INSTALL(${package_name})