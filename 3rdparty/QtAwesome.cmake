

set(qtawe_src
	${eproot}/QtAwesome/QtAwesome/QtAwesome.cpp
	${eproot}/QtAwesome/QtAwesome/QtAwesomeAnim.cpp
)

QT5_ADD_RESOURCES(qtawe_res ${eproot}/QtAwesome/QtAwesome/QtAwesome.qrc)

set(qtawe_hdr
	${eproot}/QtAwesome/QtAwesome/QtAwesome.h
	${eproot}/QtAwesome/QtAwesome/QtAwesomeAnim.h
)

QT5_WRAP_CPP(qtawe_hdr_moc ${qtawe_hdr})

add_library( qtawesome SHARED ${qtaew_src} ${qtawe_res} ${qtawe_hdr_moc})
target_link_libraries(qtawesome PUBLIC Qt5::Core Qt5::Gui)
target_include_directories(qtawesome PUBLIC ${eproot}/QtAwesome/QtAwesome/)