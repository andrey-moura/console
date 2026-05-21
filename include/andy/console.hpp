#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <andy/format.hpp>
#include <functional>

#include <andy.hpp>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace lavi
{
    namespace console
    {
        enum class color_code
        {
            red = 31,
            green = 32,
            yellow = 33,
            blue = 34,
            pink = 35,
            light_blue = 36

        }; 

        struct arg_binding
        {
            std::function<void()> handler;
            size_t argc;
        };

        struct color
        {
            const size_t code;
            std::ostream* os = nullptr;
            color(const color_code& code)
                : code((const size_t)code)
            {
                
            }

            void begin_color() const
            {
                *os << "\033[" << (size_t)code << "m";
            }

            void end_color() const 
            {
                *os << "\033[0m";
            }

            // Override for std::cout compatibility - that starts color code on console
            friend const color& operator << (std::ostream& os, const color& c)
            {
                color* pColor = (color*)&c;
                pColor->os = &os;

                c.begin_color();
                return c;
            }

            // Override for std::cout compatibility - that finishs the color code on console
            friend std::ostream& operator << (const color& color, const std::string& str)
            {
                std::ostream& os = *color.os;

                for(const char& c : str) {

                    os << c;

                    if(c == '\n') {
                        color.end_color();
                        color.begin_color();
                    }

                }

                color.end_color();

                return os;
            }
        };

        void put_on_cout(const std::string& msg);

        template<class... Args>
        void format_on_cout(const std::string& fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            put_on_cout(msg);
        }

        void log(const std::string& msg);

        template<class... Args>
        void log(const std::string& fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log(msg);
        }

        void log_error(const std::string& msg);

        template<class... Args>
        void log_error(const std::string& fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log_error(msg);
        }

        void log_success(const std::string& msg);

        template<class... Args>
        void log_success(const char* const fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log_success(msg);
        }

        template<class... Args>
        void log_success(std::string_view fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log_success(msg);
        }

        void print_warning(std::string_view msg);

        void log_warning(const std::string& msg);

        template<class... Args>
        void log_warning(const std::string& fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            //log_warning(msg);
        }

        template<class... Args>
        void log_warning(std::string_view fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log_warning(msg);
        }

        template<class... Args>
        void log_warning(const char* fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            //log_warning(msg);
        }
#ifdef NDEBUG
        void log_debug(const std::string& msg);
        template<class... Args>
        void log_debug(const std::string& fmt, Args... args) {}    
#else
        ANDY_EXPORT_SYMBOL void log_debug(const std::string& msg);

        template<class... Args>
        void log_debug(const std::string& fmt, Args... args)
        {
            std::string msg = std::vformat(fmt, std::make_format_args(args...));
            log_debug(msg);
        }
#endif
        std::vector<std::string>& get_args();
        std::map<std::string, std::string>& get_named_args();

        void init_args(const int argc, char** argv);

        size_t args_count();
        bool has_args();
        std::string front_arg();
    };
};