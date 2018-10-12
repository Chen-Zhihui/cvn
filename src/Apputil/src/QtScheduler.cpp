
#include <Cvn/Apputil/QtScheduler.h>
#include <Poco/SingletonHolder.h>
#include <QtGui>
#include <QtCore>
#include <mutex>

namespace Cvn
{
namespace Apputil
{

class MyEvent : public QEvent {
public:
    MyEvent(async::task_run_handle &&t)
            : QEvent(QEvent::User), _handle(std::move(t)) {
        ;
    }

    async::task_run_handle get() { return std::move(_handle); }
private:
    async::task_run_handle _handle;
};

class QtSchedulerImpl : public QtScheduler, public QObject
{
public:
    QtSchedulerImpl()
    {
        Q_ASSERT(qApp);
    }

    void schedule(async::task_run_handle t)
    {
        qApp->postEvent(this, new MyEvent(std::move(t)), Qt::HighEventPriority);
    }

    bool event(QEvent * ev)
    {
        //SHOULD N
        //bool accept = QObject::event(ev);
        //if( accept )
        //    return true;
        //cast event to myEvent
        if( ev->type() == QEvent::User) {
            MyEvent *myEvent = dynamic_cast<MyEvent *>(ev);
            if (myEvent) {
                myEvent->get().run();
                myEvent->accept();
                return true;
            }
        }
        return false;
    }
};


QtSchedulerImpl * gSch = nullptr;
std::once_flag flag;

QtScheduler & QtScheduler::instance() {
      std::call_once(flag, [](){gSch = new QtSchedulerImpl;});
      return *gSch;
}


QtScheduler & qtui() {
    return QtScheduler::instance();
}

}
}