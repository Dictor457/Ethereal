#include <ethereal/mem/maps_parser.hpp>
#include <fstream>
#include <sstream>
#include <charconv>

namespace ethereal::mem {

std::optional<MemoryRegion> MapsParser::parse_line(std::string_view line) {
    if (line.empty()) {
        return std::nullopt;
    }

    MemoryRegion region;

    // 1. Парсинг адресов: 7f9d8a200000-7f9d8a221000
    const auto dash_pos = line.find('-');
    if (dash_pos == std::string_view::npos) return std::nullopt;

    const auto space_pos = line.find(' ', dash_pos);
    if (space_pos == std::string_view::npos) return std::nullopt;

    const auto start_sv = line.substr(0, dash_pos);
    const auto end_sv = line.substr(dash_pos + 1, space_pos - dash_pos - 1);

    std::from_chars(start_sv.data(), start_sv.data() + start_sv.size(), region.start_addr, 16);
    std::from_chars(end_sv.data(), end_sv.data() + end_sv.size(), region.end_addr, 16);

    // 2. Парсинг прав доступа (4 символа: rwxp)
    auto cur_pos = space_pos + 1;
    if (cur_pos + 4 > line.size()) return std::nullopt;

    if (line[cur_pos] == 'r')     region.perms = region.perms | Perms::Read;
    if (line[cur_pos + 1] == 'w') region.perms = region.perms | Perms::Write;
    if (line[cur_pos + 2] == 'x') region.perms = region.perms | Perms::Execute;
    if (line[cur_pos + 3] == 'p') region.perms = region.perms | Perms::Private;

    // 3. Парсинг offset, dev, inode и pathname через потоковый разбор остатка строки
    std::istringstream iss{std::string(line.substr(cur_pos + 4))};
    std::string dev;
    if (!(iss >> std::hex >> region.offset >> dev >> std::dec >> region.inode)) {
        return std::nullopt;
    }

    // Оставшаяся часть строки — путь к файлу (если есть)
    std::string path;
    if (iss >> path) {
        region.pathname = path;
    }

    return region;
}

std::optional<std::vector<MemoryRegion>> MapsParser::parse_pid(pid_t pid) {
    const std::string path = "/proc/" + std::to_string(pid) + "/maps";
    std::ifstream maps_file(path);
    if (!maps_file.is_open()) {
        return std::nullopt;
    }

    std::vector<MemoryRegion> regions;
    std::string line;
    while (std::getline(maps_file, line)) {
        if (auto region = parse_line(line); region.has_value()) {
            regions.push_back(*region);
        }
    }

    return regions;
}

} // namespace ethereal::mem
