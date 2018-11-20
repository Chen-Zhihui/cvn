
add_library(rxqt INTERFACE)
target_include_directories(rxqt INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include>
  $<INSTALL_INTERFACE:include/rxqt>
)

install(TARGETS rxqt EXPORT rxqtExport)
install(EXPORT rxqtExport NAMESPACE Upstream::
  DESTINATION lib/cmake/rxqt
)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include/CXX14Support.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include/rxqt_event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include/rxqt_run_loop.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include/rxqt_signal.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include/rxqt_util.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rxqt/include/rxqt.hpp
  DESTINATION include/rxqt
)