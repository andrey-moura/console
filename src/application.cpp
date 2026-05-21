#include <application.hpp>

#include <iostream>
#include <sstream>

using namespace lavi;
using namespace routing;
using namespace console;

void print_help()
{
    auto& routes = lavi::routing::get_routes();

    log_warning("COMMANDS:");

    for(const auto& route : routes)
    {
        log_success("\t{}", route.first);
    }
}

void application::init(int argc, const char **argv)
{
    //starts a new connection. 
    std::shared_ptr<basic_connection> connection = std::make_shared<basic_connection>();
    var params = var::map();

    std::string cwd = argv[0];

    std::string action;

    if(argc > 1) {
        action = argv[1];
    }

    std::string arg;

    if(argc > 2) {
        for(size_t i = 0; i < argc-2; ++i)
        {
            arg = argv[i+2];
            params[i] = arg;
        }
    }

    basic_action_target target = find_dispatch_target(action, connection);

    if(target.controller) {
        target.controller->params = params;
        dispatch(target, connection);
    } else {
        print_help();
    }
}

application::action application::action_from_command_line(std::string command_line)
{
    application::action action;

    std::stringstream ss(command_line);

    if(command_line.empty() || !(ss >> action.route))
    {
        throw std::logic_error("error: an command line cannot be empty");
    }

    bool started_optional_paramaters = false;

    std::string buffer;
    while(ss >> buffer)
    {
        if(buffer.starts_with('[')) {
            buffer.erase(0, 1);
            started_optional_paramaters = true;
        } else if(buffer.ends_with(']')) {
            buffer.pop_back();
        }

        action.arguments.push_back({buffer,started_optional_paramaters});
    }
}
