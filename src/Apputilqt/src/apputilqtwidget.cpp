
#include <Cvn/Apputilqt/DirView.h>
#include <Cvn/Apputilqt/Player.h>
#include <Cvn/Apputilqt/ImageViewer.h>
#include <Cvn/Apputilqt/ImageListModel.h>
#include <QApplication>
#include <QTabWidget>

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);

    QTabWidget widget;
    widget.resize(QSize(1000, 800));

    auto view = new ImageListView;
    ImageListModel model;
    model.setupTestData();
    view->setModel(&model);
    widget.addTab(view, "ImageList");

    widget.addTab(new Cvn::Apputil::DirView(), "DirView");
    widget.addTab(new Player(), "Player");
    widget.addTab(new ImageViewer(), "Image");

    widget.show();
    return app.exec();
}
