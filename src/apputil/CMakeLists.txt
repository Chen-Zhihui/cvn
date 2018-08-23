

# Add source to this project's executable.

set(package_name apputil)

add_library ( ${package_name} STATIC 
TmplApp.cpp
TmplApp.h
)

target_link_libraries(${package_name} PUBLIC Poco::Util)

# TODO: Add tests and install targets if needed.


# Install support

install(TARGETS apputil EXPORT ${package_name}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

#export(EXPORT ${package_name}
#  FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${package_name}Targets.cmake
#  NAMESPACE "${PROJECT_NAME}::"
#)

install(EXPORT ${package_name}
  FILE  ${PROJECT_NAME}${package_name}Targets.cmake
  DESTINATION ${config_package_location}
  NAMESPACE "${PROJECT_NAME}::"
)


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}${package_name}ConfigVersion.cmake"
	  VERSION ${PROJECT_VERSION}
	  COMPATIBILITY AnyNewerVersion
)

configure_file("cmake/${package_name}Config.cmake.in"
	  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}${package_name}Config.cmake"
	  @ONLY
)
	
install(
		FILES
			"${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}${package_name}Config.cmake"
			"${CMAKE_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}${package_name}ConfigVersion.cmake"
		DESTINATION ${config_package_location}
		COMPONENT Devel
)