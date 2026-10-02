#pragma once

#include <ethereal/mem/region.hpp>
#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

namespace ethereal::mem {

class MapsParser {
public:
    // Парсит карту памяти конкретного процесса по его PID
    [[nodiscard]] static std::optional<std::vector<MemoryRegion>> parse_pid(pid_t pid);

    // Парсит одну отдельную строку из maps (вынесено отдельно для покрытия Unit-тестами!)
    [[nodiscard]] static std::optional<MemoryRegion> parse_line(std::string_view line);
};

} // namespace ethereal::mem
