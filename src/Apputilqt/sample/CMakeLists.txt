
set(dep_ins 
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputilqt/include>
)

add_executable (sample_apputil_qt
        sample_apputil_qt.cpp
AppWin.cpp
)

target_link_libraries(sample_apputil_qt
        PUBLIC qtmaterial qtawesome qdarkstyle Apputilqt  Apputil Poco::Foundation
        ${FFMPEG_LIBRARIES}
        )

target_include_directories(sample_apputil_qt
    PUBLIC ${dep_ins} 
    )

EXECUTABLE_INSTALL(sample_apputil_qt)
