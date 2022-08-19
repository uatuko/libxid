#include "xid.h"

#include <atomic>
#include <chrono>
#include <random>

#include <unistd.h>

#include <openssl/md5.h>

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <sys/sysctl.h>
#elif __linux__
#include <filesystem>
#include <fstream>
#endif

namespace xid {
mid_t mid() noexcept {
	auto id = machine_id();

	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5(reinterpret_cast<const unsigned char *>(id.data()), id.size(), digest);

	return {
		digest[0],
		digest[1],
		digest[2],
	};
}

id next() noexcept {
	static auto mid = xid::mid();
	static auto pid = xid::pid();

	static std::atomic<std::uint32_t> counter = rand();
	counter++;

	auto ts = timestamp();

	return id::data_t{
		// Timestamp, 4 bytes (big-endian)
		ts[3],
		ts[2],
		ts[1],
		ts[0],

		// Machine ID, 3 bytes
		mid[0],
		mid[1],
		mid[2],

		// Process ID, 2 bytes (specs don't specify endianness, use big-endian)
		pid[1],
		pid[0],

		// Increment, 3 bytes (big-endian)
		static_cast<byte_t>((counter >> 16) & 0xff), // counter[2]
		static_cast<byte_t>((counter >> 8) & 0xff),  // counter[1]
		static_cast<byte_t>((counter >> 0) & 0xff),  // counter[0]
	};
}

pid_t pid() noexcept {
	auto pid = getpid();

	// Can't fit a 4 byte `pid_t` into 2 bytes, ignore the most significant bytes
	return {
		static_cast<byte_t>((pid >> 0) & 0xff), // pid[0]
		static_cast<byte_t>((pid >> 8) & 0xff), // pid[1]
	};
}

timestamp_t timestamp() noexcept {
	auto time = std::chrono::system_clock::now();
	auto seconds =
		std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();

	return {
		static_cast<byte_t>((seconds >> 0) & 0xff),  // seconds[0]
		static_cast<byte_t>((seconds >> 8) & 0xff),  // seconds[1]
		static_cast<byte_t>((seconds >> 16) & 0xff), // seconds[2]
		static_cast<byte_t>((seconds >> 24) & 0xff), // seconds[3]
	};
}

std::string machine_id() noexcept {
#if defined(__APPLE__) || defined(__FreeBSD__)
	std::size_t size = 40;
	char        buffer[size];

#if __APPLE__
	sysctlbyname("kern.uuid", &buffer, &size, nullptr, 0);
#else
	sysctlbyname("kern.hostuuid", &buffer, &size, nullptr, 0);
#endif

	return buffer;
#elif __linux__
	std::filesystem::path paths[] = {
		"/etc/machine-id",
		"/sys/class/dmi/id/product_uuid",
	};

	std::string buffer;
	for (const auto &path : paths) {
		auto f = std::ifstream(path);
		std::getline(f, buffer);

		if (buffer.size() > 0) {
			break;
		}
	}

	if (buffer.size() > 0) {
		return buffer;
	}

	// We've exhausted all machine id sources, fallback to a random number
	return std::to_string(rand());
#else
	return std::to_string(rand());
#endif
}

std::uint32_t rand() noexcept {
	std::random_device rd;
	return rd();
}

} // namespace xid
