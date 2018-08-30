

set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Base/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocv/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Lmtcnn/include>
)


###
add_executable (ocv_face_detector_lmtcnn
ocv_face_detector_lmtcnn.cpp 
)

target_link_libraries(ocv_face_detector_lmtcnn 
    PRIVATE Apputil FaceBase FaceOcv FaceLmtcnn ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_face_detector_lmtcnn
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_detector_lmtcnn)

###
add_executable (ocv_face_track_lmtcnn
ocv_face_track_lmtcnn.cpp 
)

target_link_libraries(ocv_face_track_lmtcnn 
    PRIVATE Apputil FaceBase FaceOcv FaceLmtcnn ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_face_track_lmtcnn
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_track_lmtcnn)

