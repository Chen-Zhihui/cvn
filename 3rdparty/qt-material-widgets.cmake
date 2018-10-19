
set(qtmaterial_src_dir ${eproot}/qt-material-widgets/components/ )

set(qtmaterial_src
${qtmaterial_src_dir}/qtmaterialavatar.cpp 
${qtmaterial_src_dir}/lib/qtmaterialstyle.cpp 
${qtmaterial_src_dir}/lib/qtmaterialtheme.cpp 
${qtmaterial_src_dir}/qtmaterialbadge.cpp 
${qtmaterial_src_dir}/lib/qtmaterialoverlaywidget.cpp 
${qtmaterial_src_dir}/qtmaterialcheckbox.cpp 
${qtmaterial_src_dir}/lib/qtmaterialcheckable_internal.cpp 
${qtmaterial_src_dir}/lib/qtmaterialcheckable.cpp 
${qtmaterial_src_dir}/lib/qtmaterialripple.cpp 
${qtmaterial_src_dir}/lib/qtmaterialrippleoverlay.cpp 
${qtmaterial_src_dir}/qtmaterialfab.cpp 
${qtmaterial_src_dir}/qtmaterialraisedbutton.cpp 
${qtmaterial_src_dir}/qtmaterialflatbutton_internal.cpp 
${qtmaterial_src_dir}/qtmaterialflatbutton.cpp 
${qtmaterial_src_dir}/lib/qtmaterialstatetransition.cpp 
${qtmaterial_src_dir}/qtmaterialiconbutton.cpp 
${qtmaterial_src_dir}/qtmaterialprogress_internal.cpp 
${qtmaterial_src_dir}/qtmaterialprogress.cpp 
${qtmaterial_src_dir}/qtmaterialcircularprogress_internal.cpp 
${qtmaterial_src_dir}/qtmaterialcircularprogress.cpp 
${qtmaterial_src_dir}/qtmaterialslider_internal.cpp 
${qtmaterial_src_dir}/qtmaterialslider.cpp 
${qtmaterial_src_dir}/qtmaterialsnackbar_internal.cpp 
${qtmaterial_src_dir}/qtmaterialsnackbar.cpp 
${qtmaterial_src_dir}/qtmaterialradiobutton.cpp 
${qtmaterial_src_dir}/qtmaterialtoggle_internal.cpp 
${qtmaterial_src_dir}/qtmaterialtoggle.cpp 
${qtmaterial_src_dir}/qtmaterialtextfield_internal.cpp 
${qtmaterial_src_dir}/qtmaterialtextfield.cpp 
${qtmaterial_src_dir}/qtmaterialtabs_internal.cpp 
${qtmaterial_src_dir}/qtmaterialtabs.cpp 
${qtmaterial_src_dir}/qtmaterialscrollbar_internal.cpp 
${qtmaterial_src_dir}/qtmaterialscrollbar.cpp 
${qtmaterial_src_dir}/qtmaterialdialog_internal.cpp 
${qtmaterial_src_dir}/qtmaterialdialog.cpp 
${qtmaterial_src_dir}/qtmaterialdrawer_internal.cpp 
${qtmaterial_src_dir}/qtmaterialdrawer.cpp 
${qtmaterial_src_dir}/qtmaterialappbar.cpp 
${qtmaterial_src_dir}/qtmaterialautocomplete.cpp 
${qtmaterial_src_dir}/qtmaterialpaper.cpp 
${qtmaterial_src_dir}/qtmaterialtable.cpp 
${qtmaterial_src_dir}/layouts/qtmaterialsnackbarlayout.cpp 
${qtmaterial_src_dir}/qtmaterialautocomplete_internal.cpp 
${qtmaterial_src_dir}/qtmaterialmenu.cpp 
${qtmaterial_src_dir}/qtmaterialmenu_internal.cpp 
${qtmaterial_src_dir}/qtmateriallist.cpp 
${qtmaterial_src_dir}/qtmateriallistitem.cpp
)

set(qtmaterial_hdr
${qtmaterial_src_dir}/qtmaterialavatar_p.h 
${qtmaterial_src_dir}/qtmaterialavatar.h 
${qtmaterial_src_dir}/lib/qtmaterialstyle_p.h 
${qtmaterial_src_dir}/lib/qtmaterialstyle.h 
${qtmaterial_src_dir}/lib/qtmaterialtheme_p.h 
${qtmaterial_src_dir}/lib/qtmaterialtheme.h 
${qtmaterial_src_dir}/qtmaterialbadge_p.h 
${qtmaterial_src_dir}/qtmaterialbadge.h 
${qtmaterial_src_dir}/lib/qtmaterialoverlaywidget.h 
${qtmaterial_src_dir}/qtmaterialcheckbox_p.h 
${qtmaterial_src_dir}/qtmaterialcheckbox.h 
${qtmaterial_src_dir}/lib/qtmaterialcheckable_internal.h 
${qtmaterial_src_dir}/lib/qtmaterialcheckable_p.h 
${qtmaterial_src_dir}/lib/qtmaterialripple.h 
${qtmaterial_src_dir}/lib/qtmaterialrippleoverlay.h 
${qtmaterial_src_dir}/lib/qtmaterialcheckable.h 
${qtmaterial_src_dir}/qtmaterialfab_p.h 
${qtmaterial_src_dir}/qtmaterialfab.h 
${qtmaterial_src_dir}/qtmaterialraisedbutton_p.h 
${qtmaterial_src_dir}/qtmaterialraisedbutton.h 
${qtmaterial_src_dir}/qtmaterialflatbutton_internal.h 
${qtmaterial_src_dir}/qtmaterialflatbutton_p.h 
${qtmaterial_src_dir}/qtmaterialflatbutton.h 
${qtmaterial_src_dir}/lib/qtmaterialstatetransition.h 
${qtmaterial_src_dir}/lib/qtmaterialstatetransitionevent.h 
${qtmaterial_src_dir}/qtmaterialiconbutton_p.h 
${qtmaterial_src_dir}/qtmaterialiconbutton.h 
${qtmaterial_src_dir}/qtmaterialprogress_internal.h 
${qtmaterial_src_dir}/qtmaterialprogress_p.h 
${qtmaterial_src_dir}/qtmaterialprogress.h 
${qtmaterial_src_dir}/qtmaterialcircularprogress_internal.h 
${qtmaterial_src_dir}/qtmaterialcircularprogress_p.h 
${qtmaterial_src_dir}/qtmaterialcircularprogress.h 
${qtmaterial_src_dir}/qtmaterialslider_internal.h 
${qtmaterial_src_dir}/qtmaterialslider_p.h 
${qtmaterial_src_dir}/qtmaterialslider.h 
${qtmaterial_src_dir}/qtmaterialsnackbar_internal.h 
${qtmaterial_src_dir}/qtmaterialsnackbar_p.h 
${qtmaterial_src_dir}/qtmaterialsnackbar.h 
${qtmaterial_src_dir}/qtmaterialradiobutton_p.h 
${qtmaterial_src_dir}/qtmaterialradiobutton.h 
${qtmaterial_src_dir}/qtmaterialtoggle_internal.h 
${qtmaterial_src_dir}/qtmaterialtoggle_p.h 
${qtmaterial_src_dir}/qtmaterialtoggle.h 
${qtmaterial_src_dir}/qtmaterialtextfield_internal.h 
${qtmaterial_src_dir}/qtmaterialtextfield_p.h 
${qtmaterial_src_dir}/qtmaterialtextfield.h 
${qtmaterial_src_dir}/qtmaterialtabs_internal.h 
${qtmaterial_src_dir}/qtmaterialtabs_p.h 
${qtmaterial_src_dir}/qtmaterialtabs.h 
${qtmaterial_src_dir}/qtmaterialscrollbar_internal.h 
${qtmaterial_src_dir}/qtmaterialscrollbar_p.h 
${qtmaterial_src_dir}/qtmaterialscrollbar.h 
${qtmaterial_src_dir}/qtmaterialdialog_internal.h 
${qtmaterial_src_dir}/qtmaterialdialog_p.h 
${qtmaterial_src_dir}/qtmaterialdialog.h 
${qtmaterial_src_dir}/qtmaterialdrawer_internal.h 
${qtmaterial_src_dir}/qtmaterialdrawer_p.h 
${qtmaterial_src_dir}/qtmaterialdrawer.h 
${qtmaterial_src_dir}/qtmaterialappbar.h 
${qtmaterial_src_dir}/qtmaterialappbar_p.h 
${qtmaterial_src_dir}/qtmaterialautocomplete.h 
${qtmaterial_src_dir}/qtmaterialautocomplete_p.h 
${qtmaterial_src_dir}/qtmaterialpaper.h 
${qtmaterial_src_dir}/qtmaterialpaper_p.h 
${qtmaterial_src_dir}/qtmaterialtable.h 
${qtmaterial_src_dir}/qtmaterialtable_p.h 
${qtmaterial_src_dir}/layouts/qtmaterialsnackbarlayout.h 
${qtmaterial_src_dir}/layouts/qtmaterialsnackbarlayout_p.h 
${qtmaterial_src_dir}/qtmaterialautocomplete_internal.h 
${qtmaterial_src_dir}/qtmaterialmenu.h 
${qtmaterial_src_dir}/qtmaterialmenu_p.h 
${qtmaterial_src_dir}/qtmaterialmenu_internal.h 
${qtmaterial_src_dir}/qtmateriallist.h 
${qtmaterial_src_dir}/qtmateriallist_p.h 
${qtmaterial_src_dir}/qtmateriallistitem.h 
${qtmaterial_src_dir}/qtmateriallistitem_p.h
)

QT5_WRAP_CPP(qtmaterial_hdr_moc ${qtmaterial_hdr})
QT5_ADD_RESOURCES(qtmaterial_res ${qtmaterial_src_dir}/resources.qrc)

add_library( qtmaterial STATIC ${qtmaterial_src} ${qtmaterial_res} ${qtmaterial_hdr_moc})
target_link_libraries(qtmaterial PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)
target_include_directories(qtmaterial PUBLIC ${qtmaterial_src_dir})