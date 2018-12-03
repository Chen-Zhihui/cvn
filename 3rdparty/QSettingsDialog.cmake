
set(package_name qsettingsdialog)

QT5_ADD_RESOURCES(qsettingsdialog_res ${eproot}/QSettingsDialog/QSettingsDialog/qsettingsdialog_res.qrc)

QT5_WRAP_CPP(qsettingsdialog_hdr_moc 
${eproot}/QSettingsDialog/QSettingsDialog/qsettingsdialog_global.h 
${eproot}/QSettingsDialog/QSettingsDialog/qtexception.h 

)

add_library( ${package_name} STATIC  
${qsettingsdialog_res}
${qsettingsdialog_hdr_moc}

${eproot}/QSettingsDialog/QSettingsDialog/qtexception.cpp
${eproot}/QSettingsDialog/QSettingsDialog/qsettingsdialog_global.cpp

)

target_link_libraries(${package_name} PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets 
PRIVATE dialogmaster qpathedit qcoloredit
)
target_include_directories(${package_name} 
PUBLIC $<BUILD_INTERFACE:${eproot}/QSettingsDialog/QSettingsDialog/>
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