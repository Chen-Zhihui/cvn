


QT5_ADD_RESOURCES(QDarkStyleSheet_res ${eproot}/QDarkStyleSheet/qdarkstyle/style.qrc)

add_library( qdarkstyle STATIC  ${QDarkStyleSheet_res})
target_link_libraries(qdarkstyle PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)