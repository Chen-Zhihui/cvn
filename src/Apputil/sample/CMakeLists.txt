
set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
)

add_executable (demo_qt_app
cvn_qt_app.cpp  
AppWin.cpp
)

target_link_libraries(demo_qt_app 
    PRIVATE qtmaterial qtawesome qdarkstyle Apputil
	)

target_include_directories(demo_qt_app
    PUBLIC ${dep_ins} 
    )

EXECUTABLE_INSTALL(demo_qt_app)
