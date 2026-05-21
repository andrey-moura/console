#include <iostream>
#include <application.hpp>

#include <console_controller.hpp>

using namespace lavi;
using namespace routing;
using namespace console;

DECLARE_CONSOLE_APPLICATION(
    ROUTE("new-project",    console_controller::new_project);
    ROUTE("new-controller", console_controller::controller);
)