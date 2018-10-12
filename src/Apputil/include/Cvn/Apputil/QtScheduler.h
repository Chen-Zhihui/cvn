//
// Created by tom on 16-11-12.
//

#ifndef BASEAPP_QTSCHEDULER_H
#define BASEAPP_QTSCHEDULER_H

#include <async++.h>

namespace Cvn
{
namespace Apputil
{

class QtScheduler
{
public:
    virtual ~QtScheduler() = default;
    virtual void schedule(async::task_run_handle t) = 0;
    static QtScheduler & instance();
};

QtScheduler & qtui() ;

}
}
#endif //BASEAPP_QTSCHEDULER_H
