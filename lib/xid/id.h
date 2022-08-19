#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <string>

namespace xid {
class id {
public:
	using byte_t    = std::uint8_t;
	using counter_t = std::uint32_t;
	using data_t    = std::array<byte_t, 12>;
	using machine_t = std::array<byte_t, 3>;
	using pid_t     = std::uint16_t;
	using time_t    = std::chrono::system_clock::time_point;

	id(data_t) noexcept;
	id(const std::string &);

	data_t      decode(const std::string &) const;
	std::string encode() const noexcept;

	counter_t counter() const noexcept;
	machine_t machine() const noexcept;
	pid_t     pid() const noexcept;
	time_t    time() const noexcept;

	inline std::string string() const noexcept { return encode(); }

private:
	data_t _data;
};
} // namespace xid
