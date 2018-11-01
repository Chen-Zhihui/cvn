

set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Base/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Face/Ocv/include>
)


###
add_executable (ocv_face_detector 
ocv_face_detector.cpp 
)

target_link_libraries(ocv_face_detector 
    PRIVATE Apputil FaceBase FaceOcv ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_face_detector
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_detector)


###
add_executable (ocv_image_import 
ocv_image_import.cpp 
)

target_link_libraries(ocv_image_import 
    PRIVATE Apputil FaceBase FaceOcv ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_image_import
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_image_import)


###
add_executable (ocv_face_landmark 
ocv_face_landmark.cpp 
)

target_link_libraries(ocv_face_landmark 
    PRIVATE Apputil FaceBase FaceOcv ${OpenCV_LIBRARIES}  fmt::fmt 
	)
target_include_directories( ocv_face_landmark
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_landmark)


###
add_executable (ocv_face_headpose 
ocv_face_headpose.cpp 
)

target_link_libraries(ocv_face_headpose 
    PRIVATE Apputil FaceBase FaceOcv ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( ocv_face_headpose
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_headpose)


###
add_executable (ocv_face_track 
ocv_face_track.cpp 
)

target_link_libraries(ocv_face_track 
    PRIVATE Apputil FaceBase FaceOcv ${OpenCV_LIBRARIES}  fmt::fmt 
	)
target_include_directories( ocv_face_track
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(ocv_face_track)