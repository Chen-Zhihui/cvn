

#include <libopenshot/Qt/VideoRenderWidget.h>
#include <libopenshot/QtPlayer.h>
#include "stdio.h"
#include <QMessageBox>
#include <QFileDialog>
#include <Cvn/Apputilqt/VPlayer.h>

using namespace openshot;

namespace Cvn {
	namespace Apputil {

		VPlayer::VPlayer(QWidget *parent)
			: QWidget(parent)
			, vbox(new QVBoxLayout(this))
			, video(new VideoRenderWidget(this))
			, player(new openshot::QtPlayer(video->GetRenderer()))
		{
			setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
			setWindowTitle("OpenShot Player");
			vbox->addWidget(video);
			vbox->setMargin(0);
			vbox->setSpacing(0);
			setFocusPolicy(Qt::StrongFocus);
		}

		VPlayer::~VPlayer()
		{
		}

		void VPlayer::closeEvent(QCloseEvent *event)
		{
			// Close window, stop player, and quit
			//QWidget *pWin = QApplication::activeWindow();
			//pWin->hide();
			player->Stop();
			//QApplication::quit();
		}

		void VPlayer::keyPressEvent(QKeyEvent *event)
		{
			if (event->key() == Qt::Key_O) {
				open(true);
			}
			else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_K) {

				if (player->Mode() == openshot::PLAYBACK_PAUSED)
				{
					// paused, so start playing again
					player->Play();

				}
				else if (player->Mode() == openshot::PLAYBACK_PLAY)
				{

					if (player->Speed() == 0)
						// already playing, but speed is zero... so just speed up to normal
						player->Speed(1);
					else
						// already playing... so pause
						player->Pause();

				}

			}
			else if (event->key() == Qt::Key_J) {
				cout << "BACKWARD" << player->Speed() - 1 << endl;
				if (player->Speed() - 1 != 0)
					player->Speed(player->Speed() - 1);
				else
					player->Speed(player->Speed() - 2);

				if (player->Mode() == openshot::PLAYBACK_PAUSED)
					player->Play();
			}
			else if (event->key() == Qt::Key_L) {
				cout << "FORWARD" << player->Speed() + 1 << endl;
				if (player->Speed() + 1 != 0)
					player->Speed(player->Speed() + 1);
				else
					player->Speed(player->Speed() + 2);

				if (player->Mode() == openshot::PLAYBACK_PAUSED)
					player->Play();

			}
			else if (event->key() == Qt::Key_Left) {
				cout << "FRAME STEP -1" << endl;
				if (player->Speed() != 0)
					player->Speed(0);
				player->Seek(player->Position() - 1);
			}
			else if (event->key() == Qt::Key_Right) {
				cout << "FRAME STEP +1" << endl;
				if (player->Speed() != 0)
					player->Speed(0);
				player->Seek(player->Position() + 1);
			}
			else if (event->key() == Qt::Key_Escape) {
				cout << "QUIT PLAYER" << endl;
				QWidget *pWin = QApplication::activeWindow();
				pWin->hide();

				player->Stop();

				QApplication::quit();
			}

			event->accept();
			QWidget::keyPressEvent(event);
		}

		void VPlayer::open(bool checked)
		{
			// Get filename of media files
			const QString filename = QFileDialog::getOpenFileName(this, "Open Video File");
			if (filename.isEmpty()) return;

			// Create FFmpegReader and open file
			player->SetSource(filename.toStdString());

			// Set aspect ratio of widget
			video->SetAspectRatio(player->Reader()->info.display_ratio, player->Reader()->info.pixel_ratio);

			// Play video
			player->Play();
		}

	}
}