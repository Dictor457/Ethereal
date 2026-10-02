#pragma once

#include <cstdint>
#include <string>

namespace ethereal::mem {

enum class Perms : uint8_t {
    None    = 0,
    Read    = 1 << 0,
    Write   = 1 << 1,
    Execute = 1 << 2,
    Private = 1 << 3
};

constexpr Perms operator|(Perms a, Perms b) noexcept {
    return static_cast<Perms>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

constexpr bool operator&(Perms a, Perms b) noexcept {
    return (static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0;
}

struct MemoryRegion {
    std::uintptr_t start_addr{0};
    std::uintptr_t end_addr{0};
    Perms perms{Perms::None};
    std::uint64_t offset{0};
    std::uint64_t inode{0};
    std::string pathname;

    [[nodiscard]] constexpr bool is_rwx() const noexcept {
        return (perms & Perms::Read) && 
               (perms & Perms::Write) && 
               (perms & Perms::Execute);
    }

    [[nodiscard]] constexpr bool is_executable() const noexcept {
        return (perms & Perms::Execute);
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept {
        return end_addr > start_addr ? (end_addr - start_addr) : 0;
    }
};

} // namespace ethereal::mem
