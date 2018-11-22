
#include <Cvn/Apputilqt/DirView.h>

#include <QFileSystemModel>
#include <QFileIconProvider>

namespace Cvn
{
	namespace Apputil
	{

		DirView::DirView() {
			setRootPath("");
		}

		void DirView::setRootPath(const QString & rootPath) {
			_filesystem.setRootPath(rootPath);

			DirView & tree(*this);

			tree.setModel(&_filesystem);
			if (!rootPath.isEmpty()) {
				const QModelIndex rootIndex = _filesystem.index(QDir::cleanPath(rootPath));
				if (rootIndex.isValid())
					tree.setRootIndex(rootIndex);
			}
			tree.setAnimated(false);
			tree.setIndentation(20);
			tree.setSortingEnabled(true);
		}
	}
}