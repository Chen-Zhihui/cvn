

# Add source to this project's executable.

set(package_name Apputil)

QT5_ADD_RESOURCES(RECS ./src/qdarkstyle/style.qrc)

add_library ( ${package_name} STATIC 
src/TmplApp.cpp
src/AppWidget.cpp
src/QtApplication.cpp
src/QtScheduler.cpp
include/${PROJECT_NAME}/${package_name}/TmplApp.h
src/AppWidget.h
include/${PROJECT_NAME}/${package_name}/QtApplication.h
include/${PROJECT_NAME}/${package_name}/QtScheduler.h
${RECS}
)

target_link_libraries(${package_name} 
    PUBLIC Poco::Util Qt5::Core Qt5::Gui Qt5::Widgets Async++)

set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})

target_include_directories(${package_name} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# TODO: Add tests and install targets if needed.

add_subdirectory(sample)
# Install support

PACKAGE_INSTALL(${package_name})