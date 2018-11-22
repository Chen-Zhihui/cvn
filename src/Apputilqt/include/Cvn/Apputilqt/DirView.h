
#pragma once


#include <QTreeView>
#include <QFileSystemModel>

namespace Cvn
{
	namespace Apputil
	{

		class DirView : public QTreeView {
		public:
			DirView();

			void setRootPath(const QString & newPath);

			QFileSystemModel & filesystem();

		private :
			QFileSystemModel _filesystem;
		};

	}
}