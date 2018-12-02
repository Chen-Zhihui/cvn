
#pragma once


#include <QTreeView>
#include <QFileSystemModel>
#include <QWidget>

namespace Cvn
{
	namespace Apputil
	{

		class DirView : public QTreeView {
			Q_OBJECT
		public:
			DirView(QWidget * parent = nullptr);

			void setRootPath(const QString & newPath);

			QFileSystemModel & filesystem();

		private :
			QFileSystemModel _filesystem;
		};

	}
}