

#pragma once


#include <rxqt.hpp>
#include <rxcpp/operators/rx-subscribe_on.hpp>
#include <rxcpp/operators/rx-observe_on.hpp>


namespace rx {
	using namespace rxcpp;
	using namespace rxcpp::sources;
	using namespace rxcpp::operators;
	using namespace rxcpp::schedulers;
	using namespace rxcpp::util;
}

class QtRx
{
public:
	virtual ~QtRx() = default;

	rx::observe_on_one_worker ui_thread();

	static QtRx & instance();

	void init(); //must be call in ui thread
private:
	QtRx() = default;
	std::shared_ptr<rxqt::run_loop> _rxqt_run_loop;
};

QtRx & qtRx();

rx::observe_on_one_worker ui_thread();