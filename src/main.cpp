#include <ethereal/utils/cli.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    auto config_opt = ethereal::utils::parse_cli(argc, argv);
    if (!config_opt.has_value()) {
        return 1;
    }

    const auto& config = *config_opt;

    if (config.show_help) {
        ethereal::utils::print_help(argv[0]);
        return 0;
    }

    if (config.pid.has_value()) {
        std::cout << "[*] Inspecting single target PID: " << *config.pid << "\n";
        if (config.verbose) {
            std::cout << "[*] Verbose logging enabled\n";
        }
    } else if (config.scan_all) {
        std::cout << "[*] System-wide scan mode enabled\n";
    }

    return 0;
}
