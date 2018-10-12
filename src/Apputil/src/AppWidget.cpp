//
// Created by tom on 16-11-12.
//

#include "AppWidget.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <async++.h>
#include <Cvn/Apputil/QtScheduler.h>

using namespace async;
using namespace std;

namespace Cvn
{
	namespace Apputil
	{

		struct Data
		{
		public:
			list<int> values;
		};

		AppWidget::AppWidget(QWidget * parent)
			:QWidget(parent) {

			auto hlayout = new QHBoxLayout(this);
			auto listwdg = new QListWidget(this);
			auto vlayout = new QVBoxLayout();
			auto label = new QLabel(this);
			auto line = new QLineEdit(this);
			auto btn = new QPushButton(this);

			hlayout->addWidget(listwdg);
			hlayout->addLayout(vlayout);
			vlayout->addWidget(label);
			vlayout->addWidget(line);
			vlayout->addWidget(btn);
			vlayout->addStretch();

			connect(btn, &QPushButton::pressed, [listwdg, line, label, btn]() {
				auto gen = spawn([]() ->Data {
					auto r = rand();
					r = r % 30;
					Data d;
					for (int i = 0; i < r; i++)
						d.values.push_back(i);
					return d;
				}).share();

				// need shared_task<Data>, rather than task<Data>
				auto showlist = gen.then(qtui(), [listwdg, label](shared_task<Data> l)->void {
					try {
						Data d = l.get(); //.get();
						listwdg->clear();
						for (auto i : d.values) {
							listwdg->addItem(QString::number(i));
						}
					}
					catch (...) {
						label->setText("error, showlist ");
					}
				});

				// need shared_task<Data>, rather than task<Data>
				auto sumlist = gen.then([label](shared_task<Data> l) ->int {
					try {
						return parallel_map_reduce(l.get().values, 0, [label](int x) {
							return x;
						}, [](int x, int y) {
							return x + y;
						});
					}
					catch (...)
					{
						spawn(qtui(), [label]() {
							label->setText("error, sumlist");
						});
					}
					return 0;
				});

				auto showsum = sumlist.then(qtui(), [label](task<int> v) {
					try {
						label->setNum(v.get());
					}
					catch (...)
					{
						label->setText("error, showsum");
					}
				});
			});
		}

	}
}