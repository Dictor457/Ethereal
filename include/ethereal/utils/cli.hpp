#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace ethereal::utils {

struct CliConfig {
    std::optional<pid_t> pid;
    bool scan_all{false};
    bool verbose{false};
    bool json_output{false};
    bool show_help{false};
    std::string dump_dir;
};

inline void print_help(std::string_view program_name) {
    // ANSI-цвета для красивого терминала
    std::cout << "\033[1;36m" << "Ethereal" << "\033[0m" 
              << " - Linux Process & Memory Anomaly Scanner\n\n"
              << "\033[1mUSAGE:\033[0m\n"
              << "  " << program_name << " [OPTIONS]\n\n"
              << "\033[1mOPTIONS:\033[0m\n"
              << "  \033[32m-p, --pid <PID>\033[0m       Target process ID to inspect\n"
              << "  \033[32m-a, --all\033[0m             Scan all running processes\n"
              << "  \033[32m-d, --dump <DIR>\033[0m      Dump suspicious memory regions to directory\n"
              << "  \033[32m-v, --verbose\033[0m         Enable verbose forensic output\n"
              << "  \033[32m-j, --json\033[0m            Output results in JSON format\n"
              << "  \033[32m-h, --help\033[0m            Show this help message\n\n"
              << "\033[1mEXAMPLES:\033[0m\n"
              << "  " << program_name << " --pid 1337 --verbose\n"
              << "  " << program_name << " --all\n";
}

inline std::optional<CliConfig> parse_cli(int argc, char* argv[]) {
    CliConfig config;
    const std::vector<std::string_view> args(argv + 1, argv + argc);

    if (args.empty()) {
        config.show_help = true;
        return config;
    }

    for (std::size_t i = 0; i < args.size(); ++i) {
        const auto arg = args[i];

        if (arg == "-h" || arg == "--help") {
            config.show_help = true;
            return config;
        } else if (arg == "-a" || arg == "--all") {
            config.scan_all = true;
        } else if (arg == "-v" || arg == "--verbose") {
            config.verbose = true;
        } else if (arg == "-j" || arg == "--json") {
            config.json_output = true;
        } else if (arg == "-p" || arg == "--pid") {
            if (i + 1 >= args.size()) {
                std::cerr << "\033[31m[ERROR]\033[0m Missing PID after " << arg << "\n";
                return std::nullopt;
            }
            try {
                config.pid = std::stoi(std::string(args[++i]));
            } catch (...) {
                std::cerr << "\033[31m[ERROR]\033[0m Invalid PID format: " << args[i] << "\n";
                return std::nullopt;
            }
        } else if (arg == "-d" || arg == "--dump") {
            if (i + 1 >= args.size()) {
                std::cerr << "\033[31m[ERROR]\033[0m Missing directory path after " << arg << "\n";
                return std::nullopt;
            }
            config.dump_dir = std::string(args[++i]);
        } else {
            std::cerr << "\033[31m[ERROR]\033[0m Unknown option: " << arg << "\n";
            return std::nullopt;
        }
    }

    return config;
}

} // namespace ethereal::utils
