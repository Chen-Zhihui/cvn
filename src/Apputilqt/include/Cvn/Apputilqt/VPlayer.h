
#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtGui/qevent.h>
#include <QApplication>


// Define the QtPlayer without including it (due to build issues with Qt moc / Qt macros)
namespace openshot
{
	class QtPlayer;
}
class VideoRenderWidget;

namespace Cvn {
	namespace Apputil {

		class VPlayer : public QWidget
		{
			Q_OBJECT

		public:
			VPlayer(QWidget *parent = 0);
			~VPlayer();

		signals:
			void present(const QImage &image);

		protected:
			void keyPressEvent(QKeyEvent *event);
			void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

		private slots:
			void open(bool checked);

		private:
			QVBoxLayout *vbox = nullptr;
			VideoRenderWidget *video = nullptr;
			openshot::QtPlayer *player = nullptr;
		};

	}
}