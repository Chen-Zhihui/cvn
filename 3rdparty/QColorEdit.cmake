
set(package_name qcoloredit)

#QT5_ADD_RESOURCES(coloredit_res ${eproot}/ColorEdit/coloredit_res.qrc)

QT5_WRAP_CPP(qcoloredit_hdr_moc ${eproot}/QColorEdit/qcoloredit.h )

add_library( qcoloredit STATIC  
#${coloredit_res}
${coloredit_hdr_moc}
${eproot}/QColorEdit/qcoloredit.cpp
)

target_link_libraries(qcoloredit PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(qcoloredit 
PUBLIC $<BUILD_INTERFACE:${eproot}/QColorEdit/>
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