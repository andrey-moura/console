#include "andy/console.hpp"
#include <filesystem>
#include <fstream>

void lavi::console::put_on_cout(const std::string & msg)
{
    std::cout << msg << std::endl;
}

std::ofstream& get_log()
{
    static std::ofstream* stream = nullptr;

    if(!stream) {
        std::filesystem::path logs_folder = std::filesystem::absolute("logs");
        if(!std::filesystem::exists(logs_folder)) {
            if(!std::filesystem::create_directories(logs_folder)) {
                throw std::runtime_error("error: cannot create directory");
            }
        }
        stream = new std::ofstream(logs_folder / "log.txt");
    }
    return *stream;
}

void lavi::console::log(const std::string &msg)
{
    std::cout << msg << std::endl;
}

void lavi::console::log_error(const std::string& msg)
{
    std::cerr << lavi::console::color(lavi::console::color_code::red) << msg << std::endl;
}

void lavi::console::log_success(const std::string& msg)
{
    std::cout << lavi::console::color(lavi::console::color_code::green) << msg << std::endl;
}

void lavi::console::print_warning(std::string_view msg)
{
    std::cout << lavi::console::color(lavi::console::color_code::yellow) << msg.data();
}

void lavi::console::log_warning(const std::string &msg) {
  std::cout << lavi::console::color(lavi::console::color_code::yellow) << msg
            << std::endl;
}

void lavi::console::log_debug(const std::string& msg)
{
#ifdef __ANDY_DEBUG__
    get_log() << msg << std::endl;
#endif
}

std::vector<std::string>& lavi::console::get_args()
{
    static std::vector<std::string> args;

    return args;
}

std::map<std::string, std::string>& lavi::console::get_named_args()
{
    static std::map<std::string, std::string> named_args;

    return named_args;
}


void lavi::console::init_args(const int argc, char** argv)
{
    auto& args = get_args();
    auto& named_args = get_named_args();

    args.reserve(argc);

    for(size_t i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        args.push_back(std::string(arg));

        if(arg.starts_with("--")) {
            std::string key;
            std::string val;

            size_t to_reserve = 128;
            key.reserve(to_reserve);
            val.reserve(to_reserve);

            arg.remove_prefix(2);

            while(arg.size() && !arg.starts_with('=')) {
                key.push_back(arg.front());
                arg.remove_prefix(1);
            }

            if(arg.size()) {
                arg.remove_prefix(1);

                while(arg.size()) {
                    val.push_back(arg.front());
                    arg.remove_prefix(1);
                }
            }

            named_args.insert({key,val});
        }

    }
}

size_t lavi::console::args_count()
{
    return get_args().size();
}

bool lavi::console::has_args()
{
    return args_count();
}

std::string lavi::console::front_arg()
{
    if(!has_args()) {
        throw std::out_of_range("front_arg error: there is no arguments");
    }

    auto& args = get_args();

    std::string arg = std::move(args.front());

    args.erase(args.begin());

    return arg;
}