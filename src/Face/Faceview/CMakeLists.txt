
set(dep_ins 
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputilqt/include>
)

add_executable (app_faceview
        sample_apputil_qt.cpp
AppWin.cpp
)

target_link_libraries(app_faceview
        PUBLIC qtmaterial qtawesome qdarkstyle qpathedit qsettingsdialog Apputilqt  Apputil Poco::Foundation
        ${FFMPEG_LIBRARIES}
        )

target_include_directories(app_faceview
    PUBLIC ${dep_ins} 
    )

EXECUTABLE_INSTALL(app_faceview)
