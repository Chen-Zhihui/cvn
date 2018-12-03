
set(package_name qsettingsdialog)

QT5_ADD_RESOURCES(qsettingsdialog_res ${eproot}/QSettingsDialog/QSettingsDialog/qsettingsdialog_res.qrc)

QT5_WRAP_CPP(qsettingsdialog_hdr_moc 
${eproot}/QSettingsDialog/QSettingsDialog/qsettingsdialog_global.h 
${eproot}/QSettingsDialog/QSettingsDialog/qtexception.h 

)

set(core_path ${eproot}/QSettingsDialog/QSettingsDialog/core)
set(core_pub_hdr
${core_path}/qsettingsentry.h
${core_path}/qsettingsloader.h
${core_path}/qsettingsloaderentry.h
${core_path}/qsettingsdialog.h
${core_path}/qsettingscontainer.h
${core_path}/exceptions.h
${core_path}/qsettingslayout.h
#${core_path}/qsettingsdisplayengine.h
${core_path}/qasyncsettingscontainer.h
)
set(core_hdr 
${core_path}/qsettingsdialog_p.h
${core_path}/containerelements.h
${core_path}/settingspathparser.h
${core_path}/sortedmap.h
${core_path}/specialgroupmap.h
${core_path}/qsettingslayout_p.h
${core_path}/qsettingsentry_p.h
${core_path}/qasyncsettingscontainer_p.h
)
QT5_WRAP_CPP(core_moc
${core_pub_hdr}
${core_hdr}
)
set(core_src
${core_path}/qsettingsloader.cpp
${core_path}/qsettingsdialog.cpp
${core_path}/settingspathparser.cpp
${core_path}/qsettingscontainer.cpp
${core_path}/exceptions.cpp
${core_path}/specialgroupmap.cpp
${core_path}/qsettingslayout.cpp
${core_path}/qsettingsentry.cpp
${core_path}/qasyncsettingscontainer.cpp
)


set(dialogui_path ${eproot}/QSettingsDialog/QSettingsDialog/dialogui)
set(dialogui_pub_hdr
${dialogui_path}/qsettingswidget.h
${dialogui_path}/qsettingswidgetdialogengine.h
${dialogui_path}/qsettingswidgetfactory.h
${dialogui_path}/qsettingswidgetfactoryregistry.h
${dialogui_path}/qsettingsdialogwidget.h
${dialogui_path}/qsettingsgroupwidget.h
${dialogui_path}/qsettingsgroupwidgetfactory.h
)
set(dialogui_prv_hdr
${dialogui_path}/settingsdisplaydialog.h
${dialogui_path}/settingsengine.h
${dialogui_path}/checkinghelper.h
${dialogui_path}/commonfactoryregistry.h
)
set(dialogui_src
${dialogui_path}/qsettingswidget.cpp
${dialogui_path}/settingsdisplaydialog.cpp
${dialogui_path}/settingsengine.cpp
${dialogui_path}/qsettingswidgetdialogengine.cpp
${dialogui_path}/commonfactoryregistry.cpp
${dialogui_path}/qsettingsdialogwidget.cpp
${dialogui_path}/qsettingsgroupwidget.cpp
)
QT5_WRAP_CPP(dialogui_moc
${dialogui_pub_hdr}
${dialogui_prv_hdr}
)
QT5_WRAP_UI(dialogui_uis ${dialogui_path}/settingsdisplaydialog.ui )

set(variantwidgets_path ${eproot}/QSettingsDialog/QSettingsDialog/variantwidgets)
set(variantwidgets_pub_hdr
${variantwidgets_path}/variantwidgetsregistrator.h
${variantwidgets_path}/qsettingsextendedtypes.h
)

set(variantwidgets_prv_hdr
${variantwidgets_path}/settingscheckbox.h
${variantwidgets_path}/settingsspinbox.h
${variantwidgets_path}/settingslineedit.h
${variantwidgets_path}/settingsdatetimeedit.h
${variantwidgets_path}/settingscoloredit.h
${variantwidgets_path}/settingslistedit.h
${variantwidgets_path}/settingsfontcombobox.h
${variantwidgets_path}/settingskeyedit.h
${variantwidgets_path}/settingsenumcombobox.h
${variantwidgets_path}/settingsflagschecklist.h
${variantwidgets_path}/settingsenumwidgetfactory.h
${variantwidgets_path}/settingspathedit.h
${variantwidgets_path}/settingsslider.h
${variantwidgets_path}/settingsrichtextedit.h
${variantwidgets_path}/settingsgroupbox.h
)
set(variantwidgets_src
    ${variantwidgets_path}/variantwidgetsregistrator.cpp
	${variantwidgets_path}/settingscheckbox.cpp
	${variantwidgets_path}/settingsspinbox.cpp
	${variantwidgets_path}/settingslineedit.cpp
	${variantwidgets_path}/settingsdatetimeedit.cpp
	${variantwidgets_path}/settingscoloredit.cpp
	${variantwidgets_path}/settingslistedit.cpp
	${variantwidgets_path}/settingsfontcombobox.cpp
	${variantwidgets_path}/settingskeyedit.cpp
	${variantwidgets_path}/settingsenumcombobox.cpp
	${variantwidgets_path}/settingsflagschecklist.cpp
    ${variantwidgets_path}/settingsenumwidgetfactory.cpp
    ${variantwidgets_path}/settingspathedit.cpp
    ${variantwidgets_path}/settingsslider.cpp
    ${variantwidgets_path}/settingsrichtextedit.cpp
    ${variantwidgets_path}/settingsgroupbox.cpp
)
QT5_WRAP_UI(variantwidgets_uis ${variantwidgets_path}/settingslistedit.ui )
QT5_ADD_RESOURCES(variantwidget_res 
${variantwidgets_path}/qsettingsdialog_variantwidgets_res.qrc)


QT5_WRAP_CPP(variantwidgets_moc
${variantwidgets_pub_hdr}
${variantwidgets_prv_hdr}
)

set(loaders_path ${eproot}/QSettingsDialog/QSettingsDialog/loaders)
set(loaders_pub_hdr
${loaders_path}/qsettingspropertyloader.h
${loaders_path}/qsettingspropertyentry.h
${loaders_path}/qsettingssettingsloader.h
)

set(loaders_src
${loaders_path}/qsettingspropertyloader.cpp
${loaders_path}/qsettingspropertyentry.cpp
${loaders_path}/qsettingssettingsloader.cpp
)
QT5_WRAP_CPP(loaders_mocs
    ${loaders_path}/qsettingspropertyloader.h
)


add_library( ${package_name} SHARED  
${qsettingsdialog_res}
${qsettingsdialog_hdr_moc}

${eproot}/QSettingsDialog/QSettingsDialog/qtexception.cpp
${eproot}/QSettingsDialog/QSettingsDialog/qsettingsdialog_global.cpp

${core_moc}
${core_src}

${dialogui_moc}
${dialogui_src}
${dialogui_uis}

${variantwidgets_moc}
${variantwidgets_src}
${variantwidgets_uis}
${variantwidget_res}

${loaders_mocs}
${loaders_src}
)

target_link_libraries(${package_name} 
PUBLIC  Qt5::Core Qt5::Gui Qt5::Widgets 
PRIVATE dialogmaster qpathedit qcoloredit
)
target_compile_definitions(${package_name} PRIVATE QSETTINGSDIALOG_LIBRARY)
target_include_directories(${package_name} 
PUBLIC $<BUILD_INTERFACE:${eproot}/QSettingsDialog/QSettingsDialog/>
PUBLIC $<BUILD_INTERFACE:${eproot}/QSettingsDialog/QSettingsDialog/core>
PUBLIC $<BUILD_INTERFACE:${eproot}/QSettingsDialog/QSettingsDialog/dialogui>
PUBLIC $<BUILD_INTERFACE:${eproot}/QSettingsDialog/QSettingsDialog/loaders>
PUBLIC $<BUILD_INTERFACE:${eproot}/QSettingsDialog/QSettingsDialog/variantwidgets>
PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
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