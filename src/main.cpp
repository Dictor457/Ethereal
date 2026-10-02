#include <ethereal/utils/cli.hpp>
#include <ethereal/mem/maps_parser.hpp>
#include <iostream>
#include <iomanip>

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
        const pid_t target_pid = *config.pid;
        std::cout << "\033[1;34m[*] Inspecting PID:\033[0m " << target_pid << "\n";

        auto regions_opt = ethereal::mem::MapsParser::parse_pid(target_pid);
        if (!regions_opt.has_value()) {
            std::cerr << "\033[31m[ERROR]\033[0m Could not open /proc/" << target_pid 
                      << "/maps. Check PID existence or root permissions.\n";
            return 1;
        }

        const auto& regions = *regions_opt;
        std::cout << "[+] Found " << regions.size() << " memory mappings\n";

        std::size_t rwx_count = 0;
        for (const auto& reg : regions) {
            if (reg.is_rwx()) {
                ++rwx_count;
                std::cout << "\033[1;31m[!] CRITICAL ANOMALY: RWX Segment Detected!\033[0m\n"
                          << "    Address: 0x" << std::hex << reg.start_addr 
                          << " - 0x" << reg.end_addr << std::dec << "\n"
                          << "    Size:    " << (reg.size() / 1024) << " KB\n"
                          << "    Path:    " << (reg.pathname.empty() ? "[anonymous/injected]" : reg.pathname) << "\n";
            }
        }

        if (rwx_count == 0) {
            std::cout << "\033[1;32m[+] Status: CLEAN (No dangerous RWX mappings found)\033[0m\n";
        }
    }

    return 0;
}
