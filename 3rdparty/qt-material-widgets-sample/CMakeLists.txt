


add_executable(sample_qtmaterial sample_qtmaterial.cpp)
target_link_libraries(sample_qtmaterial qtmaterial)

set(example_dir ${eproot}/qt-material-widgets/examples/ )

set( examp_hdrs
        ${example_dir}/mainwindow.h
        ${example_dir}/avatarsettingseditor.h
        ${example_dir}/badgesettingseditor.h
        ${example_dir}/checkboxsettingseditor.h
        ${example_dir}/fabsettingseditor.h
        ${example_dir}/raisedbuttonsettingseditor.h
        ${example_dir}/flatbuttonsettingseditor.h
        ${example_dir}/iconbuttonsettingseditor.h
        ${example_dir}/progresssettingseditor.h
        ${example_dir}/circularprogresssettingseditor.h
        ${example_dir}/slidersettingseditor.h
        ${example_dir}/radiobuttonsettingseditor.h
        ${example_dir}/togglesettingseditor.h
        ${example_dir}/textfieldsettingseditor.h
        ${example_dir}/tabssettingseditor.h
        ${example_dir}/snackbarsettingseditor.h
        ${example_dir}/dialogsettingseditor.h
        ${example_dir}/drawersettingseditor.h
        ${example_dir}/scrollbarsettingseditor.h
        ${example_dir}/appbarsettingseditor.h
        ${example_dir}/autocompletesettingseditor.h
        ${example_dir}/menusettingseditor.h
        )

set( forms
        ${example_dir}/avatarsettingsform.ui
        ${example_dir}/badgesettingsform.ui
        ${example_dir}/checkboxsettingsform.ui
        ${example_dir}/fabsettingsform.ui
        ${example_dir}/flatbuttonsettingsform.ui
        ${example_dir}/iconbuttonsettingsform.ui
        ${example_dir}/progresssettingsform.ui
        ${example_dir}/circularprogresssettingsform.ui
        ${example_dir}/slidersettingsform.ui
        ${example_dir}/snackbarsettingsform.ui
        ${example_dir}/radiobuttonsettingsform.ui
        ${example_dir}/togglesettingsform.ui
        ${example_dir}/textfieldsettingsform.ui
        ${example_dir}/tabssettingsform.ui
        ${example_dir}/dialogsettingsform.ui
        ${example_dir}/drawersettingsform.ui
        ${example_dir}/scrollbarsettingsform.ui
        ${example_dir}/appbarsettingsform.ui
        )

QT5_WRAP_CPP(examp_hdrs_moc ${examp_hdrs})
QT5_ADD_RESOURCES(examp_res ${example_dir}/examples.qrc)
qt5_wrap_ui(examp_ui ${forms})

add_executable(sample_qtmaterial_examples
        ${example_dir}/mainwindow.cpp
        ${example_dir}/main.cpp
        ${example_dir}/avatarsettingseditor.cpp
        ${example_dir}/badgesettingseditor.cpp
        ${example_dir}/checkboxsettingseditor.cpp
        ${example_dir}/fabsettingseditor.cpp
        ${example_dir}/raisedbuttonsettingseditor.cpp
        ${example_dir}/flatbuttonsettingseditor.cpp
        ${example_dir}/iconbuttonsettingseditor.cpp
        ${example_dir}/progresssettingseditor.cpp
        ${example_dir}/circularprogresssettingseditor.cpp
        ${example_dir}/slidersettingseditor.cpp
        ${example_dir}/radiobuttonsettingseditor.cpp
        ${example_dir}/togglesettingseditor.cpp
        ${example_dir}/textfieldsettingseditor.cpp
        ${example_dir}/tabssettingseditor.cpp
        ${example_dir}/snackbarsettingseditor.cpp
        ${example_dir}/dialogsettingseditor.cpp
        ${example_dir}/drawersettingseditor.cpp
        ${example_dir}/scrollbarsettingseditor.cpp
        ${example_dir}/appbarsettingseditor.cpp
        ${example_dir}/autocompletesettingseditor.cpp
        ${example_dir}/menusettingseditor.cpp
        ${examp_res}
        ${examp_hdrs_moc}
        ${examp_ui}
        )
target_include_directories(sample_qtmaterial_examples
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        )
target_link_libraries(sample_qtmaterial_examples qtmaterial)