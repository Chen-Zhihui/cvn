
set(dep_ins 
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputilqt/include>
)

add_executable (cv_qt_app
cvn_qt_app.cpp  
AppWin.cpp
)

target_link_libraries(cv_qt_app 
    qtmaterial qtawesome qdarkstyle Apputilqt 
    /lib/x86_64-linux-gnu/libz.so.1
    )
if(LINUX)
target_link_libraries(cv_qt_app 
    /lib/x86_64-linux-gnu/libz.so.1
)
endif()

target_include_directories(cv_qt_app
    PUBLIC ${dep_ins} 
    )

EXECUTABLE_INSTALL(cv_qt_app)
