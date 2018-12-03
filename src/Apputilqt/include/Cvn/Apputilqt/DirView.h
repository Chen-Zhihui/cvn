
#pragma once


#include <QTreeView>
#include <QFileSystemModel>
#include <QWidget>


class QPathEdit;

		class DirView : public QWidget {
			Q_OBJECT
		public:
			DirView(QWidget * parent = nullptr);

			void setRootPath(const QString & newPath);

			QFileSystemModel & filesystem();

		private :
			QFileSystemModel _filesystem;
			QTreeView * view = nullptr;
			QPathEdit * edit = nullptr;
		};

