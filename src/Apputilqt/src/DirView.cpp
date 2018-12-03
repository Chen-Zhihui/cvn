
#include <Cvn/Apputilqt/DirView.h>

#include <QFileSystemModel>
#include <QFileIconProvider>
#include <qpathedit.h>
#include <QVBoxLayout>
#include <QLabel>



		DirView::DirView(QWidget * parent ) : QWidget(parent) {
			auto layout = new QVBoxLayout(this);
			{
				auto hlayout = new QHBoxLayout;
				layout->addLayout(hlayout);
				hlayout->addWidget(new QLabel("Folder:", this));
				edit = new QPathEdit(this);
				hlayout->addWidget(edit);
			}
			view = new QTreeView(this);
			layout->addWidget(view);
			setRootPath("");

			{
				edit->setPathMode(QPathEdit::ExistingFolder);
			}
			{	
			    QTreeView & tree(*view);
				tree.setAnimated(false);
				tree.setIndentation(20);
				tree.setSortingEnabled(true);
				tree.setModel(&_filesystem);
			}

			{
				//setup 
				connect(edit, &QPathEdit::pathChanged, [this](QString  path){
					_filesystem.setRootPath(path);
					QTreeView & tree(*view);
					if (!path.isEmpty()) {
						const QModelIndex rootIndex = _filesystem.index(QDir::cleanPath(path));
						if (rootIndex.isValid())
							tree.setRootIndex(rootIndex);
					}
				});
			}
		}

		void DirView::setRootPath(const QString & rootPath) {
			edit->setPath(rootPath, false);
		}
