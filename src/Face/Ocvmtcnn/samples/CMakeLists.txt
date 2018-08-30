

set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Base/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocv/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocvmtcnn/include>
)


###
add_executable (ocv_face_detector_ocvmtcnn
ocv_face_detector_ocvmtcnn.cpp 
)

target_link_libraries(ocv_face_detector_ocvmtcnn 
    PRIVATE Apputil FaceBase FaceOcv Ocvmtcnn ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_face_detector_ocvmtcnn
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_detector_ocvmtcnn)

###
add_executable (ocv_face_track_ocvmtcnn
ocv_face_track_ocvmtcnn.cpp 
)

target_link_libraries(ocv_face_track_ocvmtcnn 
    PRIVATE Apputil FaceBase FaceOcv Ocvmtcnn ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_face_track_ocvmtcnn
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_track_ocvmtcnn)

