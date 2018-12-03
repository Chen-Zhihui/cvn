
set(package_name qcoloredit)

#QT5_ADD_RESOURCES(coloredit_res ${eproot}/ColorEdit/coloredit_res.qrc)

QT5_WRAP_CPP(qcoloredit_hdr_moc 
${eproot}/QColorEdit/qcoloredit.h 
${eproot}/QColorEdit/colorbutton.h 
)

add_library( ${package_name} STATIC  
#${coloredit_res}
${qcoloredit_hdr_moc}
${eproot}/QColorEdit/qcoloredit.cpp
${eproot}/QColorEdit/colorbutton.cpp
)

target_link_libraries(${package_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(${package_name} 
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