#pragma once

#include <array>
#include <cstdint>
#include <string>

#include "id.h"

namespace xid {
using byte_t      = id::byte_t;
using mid_t       = std::array<byte_t, 3>;
using pid_t       = std::array<byte_t, 2>;
using timestamp_t = std::array<byte_t, 4>;

/**
 * Returns the 3 byte machine ID
 */
mid_t mid() noexcept;

/**
 * Returns the next ID
 */
id next() noexcept;

/**
 * Returns the current process ID as 2 bytes (little-endian)
 */
pid_t pid() noexcept;

/**
 * Returns the timestamp as 4 bytes (little-endian)
 */
timestamp_t timestamp() noexcept;

/**
 * Returns the machine ID
 */
std::string machine_id() noexcept;

/**
 * Returns a non-deterministic random number
 */
std::uint32_t rand() noexcept;
} // namespace xid
