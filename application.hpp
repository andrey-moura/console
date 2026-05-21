#pragma once

#include <routing.hpp>

#define DECLARE_CONSOLE_APPLICATION(...) \
int main(int argc, const char **argv)\
{\
    __VA_ARGS__\
    application::init(argc, argv);\
    return 0;\
}\

namespace lavi
{
    namespace console
    {
        namespace application
        {
            struct argument
            {
                std::string name;
                bool        optional;
            };
            struct action
            {
                std::string route;
                std::vector<argument> arguments;
                std::vector<std::string> switchs;
            };
            void init(int argc, const char **argv);
            action action_from_command_line(std::string command_line);
        }; // namespace application
        
    }; // namespace console
     
}; // namespace console