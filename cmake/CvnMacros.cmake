


#===============================================================================
# Macros for simplified installation
#
#  Cvn_INSTALL - Install the given target
#    Usage: Cvn_INSTALL(target_name)
#      INPUT:
#           target_name             the name of the target. e.g. Foundation for CvnFoundation
#    Example: Cvn_INSTALL(Foundation)
macro(Cvn_INSTALL target_name)

install(TARGETS ${package_name} EXPORT ${package_name}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/Cvn
  DESTINATION include/
  FILES_MATCHING PATTERN "*.h"
)

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

configure_file("cmake/${package_name}Config.in.cmake"
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

endmacro()