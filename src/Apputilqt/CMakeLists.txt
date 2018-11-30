

# Add source to this project's executable.

set(package_name Apputilqt)

set(qplayer_src 
#./src/qplayer/VPlayer.cpp
)
set(qplayer_hdr 
#include/${PROJECT_NAME}/${package_name}/VPlayer.h
include/${PROJECT_NAME}/${package_name}/Player.h 
include/${PROJECT_NAME}/${package_name}/PlayerControls.h 
include/${PROJECT_NAME}/${package_name}/PlaylistModel.h 
include/${PROJECT_NAME}/${package_name}/VideoWidget.h 
include/${PROJECT_NAME}/${package_name}/HistogramWidget.h 
include/${PROJECT_NAME}/${package_name}/QAudioLevel.h 
include/${PROJECT_NAME}/${package_name}/ImageViewer.h 
include/${PROJECT_NAME}/${package_name}/QtApplication.h
include/${PROJECT_NAME}/${package_name}/QtScheduler.h
include/${PROJECT_NAME}/${package_name}/QtRx.h
include/${PROJECT_NAME}/${package_name}/DirView.h
src/AppWidget.h
)

QT5_WRAP_CPP(qplayer_hdr_moc ${qplayer_hdr})

add_library ( ${package_name} STATIC 
src/AppWidget.cpp
src/QtApplication.cpp
src/QtScheduler.cpp
src/QtRx.cpp
src/DirView.cpp
src/Player.cpp 
src/PlaylistModel.cpp 
src/PlayerControls.cpp 
src/VideoWidget.cpp 
src/HistogramWidget.cpp 
src/ImageViewer.cpp
${qplayer_hdr_moc} 
#${qplayer_src}
)


target_link_libraries(${package_name} 
    PUBLIC Poco::Util Qt5::Core Qt5::Gui Qt5::Widgets  Qt5::Multimedia  Qt5::MultimediaWidgets Qt5::PrintSupport Async++ BZip2::BZip2 rxqt
	#PRIVATE openshot  
)

set_target_properties(${package_name} PROPERTIES 
	OUTPUT_NAME ${PROJECT_NAME}${package_name})

target_include_directories(${package_name} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# TODO: Add tests and install targets if needed.

#add_subdirectory(sample)
# Install support

PACKAGE_INSTALL(${package_name})