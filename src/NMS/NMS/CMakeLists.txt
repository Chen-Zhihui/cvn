

# Add source to this project's executable.

set(package_name NMSUtil)

add_library ( ${package_name} STATIC
    src/QueryService.cpp
    src/QueryService_impl.cpp 
)
target_link_libraries(${package_name} PUBLIC Poco::Foundation Poco::Util nmslib::nmslib)
set_target_properties(${package_name} PROPERTIES 
    OUTPUT_NAME ${PROJECT_NAME}${package_name})
target_include_directories(${package_name} 
PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/Cvn/NMS/NMS>
   $<INSTALL_INTERFACE:include>  #
)

# TODO: Add tests and install targets if needed.

# Install support

PACKAGE_INSTALL(${package_name})