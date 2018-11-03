

# Add source to this project's executable.

set(package_name Apputilqt)

set(qplayer_src ./src/qplayer/VPlayer.cpp)
set(qplayer_hdr include/${PROJECT_NAME}/${package_name}/VPlayer.h)
QT5_WRAP_CPP(qplayer_hdr_moc ${qplayer_hdr})

add_library ( ${package_name} STATIC 
src/AppWidget.cpp
src/QtApplication.cpp
src/QtScheduler.cpp
src/AppWidget.h
include/${PROJECT_NAME}/${package_name}/QtApplication.h
include/${PROJECT_NAME}/${package_name}/QtScheduler.h

${qplayer_hdr_moc} ${qplayer_src}
)


target_link_libraries(${package_name} 
    PUBLIC Poco::Util Qt5::Core Qt5::Gui Qt5::Widgets Async++
	PRIVATE openshot  
)

set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})

target_include_directories(${package_name} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# TODO: Add tests and install targets if needed.

add_subdirectory(sample)
# Install support

PACKAGE_INSTALL(${package_name})