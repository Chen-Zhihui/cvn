
set(dep_ins 
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputilqt/include>
)

add_executable (cv_qt_app
cvn_qt_app.cpp  
AppWin.cpp
)

target_link_libraries(cv_qt_app
        PUBLIC qtmaterial qtawesome qdarkstyle Apputilqt  Apputil Poco::Foundation
        ${FFMPEG_LIBRARIES}
        )

if(LINUX)
target_link_libraries(cv_qt_app
)
endif()

target_include_directories(cv_qt_app
    PUBLIC ${dep_ins} 
    )

EXECUTABLE_INSTALL(cv_qt_app)
