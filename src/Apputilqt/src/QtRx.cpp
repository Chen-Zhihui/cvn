
#include <Cvn/Apputilqt/QtRx.h>

rx::observe_on_one_worker QtRx::ui_thread() {
	return _rxqt_run_loop->observe_on_run_loop();
}

rx::observe_on_one_worker ui_thread() {
	return qtRx().ui_thread();
}

QtRx * gSch = nullptr;
std::once_flag flag;

QtRx & QtRx::instance() {
	std::call_once(flag, []() {
		gSch = new QtRx; 
		gSch->init();
	});
	return *gSch;
}

QtRx & qtRx() {
	return QtRx::instance();
}

void QtRx::init() { //must be call in ui thread
	_rxqt_run_loop.reset(new rxqt::run_loop());
}