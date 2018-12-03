
set(package_name qpathedit)

QT5_ADD_RESOURCES(qpathedit_res ${eproot}/QPathEdit/QPathEdit/qpathedit_res.qrc)

QT5_WRAP_CPP(qpathedit_hdr_moc ${eproot}/QPathEdit/QPathEdit/qpathedit.h )

add_library( ${package_name} STATIC  
${qpathedit_res}
${qpathedit_hdr_moc}
${eproot}/QPathEdit/QPathEdit/qpathedit.cpp
)

target_link_libraries(${package_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets 
PRIVATE dialogmaster
)
target_include_directories(${package_name} 
PUBLIC $<BUILD_INTERFACE:${eproot}/QPathEdit/QPathEdit/>
)

install(TARGETS ${package_name} EXPORT ${package_name}
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
INCLUDES DESTINATION include
)

install(EXPORT ${package_name}
  FILE  ${PROJECT_NAME}${package_name}Targets.cmake
  DESTINATION ${config_package_location}
  NAMESPACE "${PROJECT_NAME}::"
)