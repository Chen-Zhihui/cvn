set(dep_ins 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Apputil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Cvutil/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Caffe2util/include>
)


###
add_executable (sample_caffe2_predictor
		sample_caffe2_predictor.cpp
)

target_link_libraries(sample_caffe2_predictor
    PRIVATE Apputil Caffe2util ${OpenCV_LIBRARIES}  fmt::fmt
	)
target_include_directories( sample_caffe2_predictor
    PUBLIC ${dep_ins} ${OpenCV_INCLUDE_DIRS}
    )

EXECUTABLE_INSTALL(sample_caffe2_predictor)