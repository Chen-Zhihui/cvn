

set(qtawesome_src
	${eproot}/QtAwesome/QtAwesome/QtAwesome.cpp
	${eproot}/QtAwesome/QtAwesome/QtAwesomeAnim.cpp
)


set(qtawesome_hdr
	${eproot}/QtAwesome/QtAwesome/QtAwesome.h
	${eproot}/QtAwesome/QtAwesome/QtAwesomeAnim.h
)

QT5_WRAP_CPP(qtawesome_hdr_moc ${qtawesome_hdr})
QT5_ADD_RESOURCES(qtawesome_res ${eproot}/QtAwesome/QtAwesome/QtAwesome.qrc)

add_library( qtawesome STATIC ${qtawesome_src} ${qtawesome_res} ${qtawesome_hdr_moc})
target_link_libraries(qtawesome PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(qtawesome PUBLIC ${eproot}/QtAwesome/QtAwesome/)