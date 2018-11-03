

# Add source to this project's executable.

set(package_name Apputil)


add_library ( ${package_name} STATIC 
src/TmplApp.cpp
)


target_link_libraries(${package_name}
		PUBLIC Poco::Util Poco::Zip Poco::XML Poco::Data Poco::Foundation ZLIB::ZLIB EXPAT::EXPAT PCRE::PCRE sqlite3 Async++
)

set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})

target_include_directories(${package_name} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# TODO: Add tests and install targets if needed.

# Install support

PACKAGE_INSTALL(${package_name})