#pragma once

#include <array>
#include <cstddef>
#include <string>

namespace xid {
using byte_t      = std::byte;
using id_t        = std::array<byte_t, 12>;
using mid_t       = std::array<byte_t, 3>;
using pid_t       = std::array<byte_t, 2>;
using timestamp_t = std::array<byte_t, 4>;

class xid {
public:
	/**
	 * Returns the 3 byte machine ID
	 */
	mid_t mid() const noexcept;

	/**
	 * Returns the next ID
	 */
	id_t next() const noexcept;

	/**
	 * Returns the current process ID as 2 bytes (little-endian)
	 */
	pid_t pid() const noexcept;

	/**
	 * Returns the timestamp as 4 bytes (little-endian)
	 */
	timestamp_t timestamp() const noexcept;

private:
	/**
	 * Returns the machine ID
	 */
	std::string machine_id() const noexcept;

	/**
	 * Returns a non-deterministic random number
	 */
	uint32_t rand() const noexcept;
};

void next();
} // namespace xid
