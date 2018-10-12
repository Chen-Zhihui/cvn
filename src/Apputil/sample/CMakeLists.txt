

set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
)



#QT5_ADD_RESOURCES(RECS ../src/qdarkstyle/style.qrc)

###
add_executable (cvn_qt_app
cvn_qt_app.cpp  ${RECS}
)

if(MSVC)
SET (MYLIB -Wl,--whole-archive Apputil -Wl,--no-whole-archive)
else()
set(MYLIB /WHOLEARCHIVE Apputil)
endif()

target_link_libraries(cvn_qt_app 
    PRIVATE ${MYLIB}
	)
target_include_directories( cvn_qt_app
    PUBLIC ${dep_ins} 
    )

EXECUTABLE_INSTALL(cvn_qt_app)
