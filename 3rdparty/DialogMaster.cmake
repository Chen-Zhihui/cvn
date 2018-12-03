
set(package_name dialogmaster)

#QT5_ADD_RESOURCES(dialogmaster_res ${eproot}/DialogMaster/dialogmaster_res.qrc)

QT5_WRAP_CPP(dialogmaster_hdr_moc ${eproot}/DialogMaster/dialogmaster.h )

add_library( ${package_name} STATIC  
#${dialogmaster_res}
${dialogmaster_hdr_moc}
${eproot}/DialogMaster/dialogmaster.cpp
)

target_link_libraries(${package_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(${package_name} 
PUBLIC $<BUILD_INTERFACE:${eproot}/DialogMaster/>
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