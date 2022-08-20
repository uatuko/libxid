#include <bitset>
#include <iomanip>
#include <iostream>

#include <xid/xid.h>

int main() {
	auto id = xid::next(); // generate the next id

	auto encoded = id.encode(); // base32 encode
	std::cout << "id.encode(): " << encoded << std::endl;

	// Get embedded info
	std::cout << "Embedded info:" << std::endl;

	auto t = std::chrono::system_clock::to_time_t(id.time());
	std::cout << "  time: " << std::put_time(std::localtime(&t), "%FT%T%z") << std::endl;

	auto m = id.machine();
	std::cout << "  machine: 0x" << std::hex << static_cast<int>(m[0]) << static_cast<int>(m[1])
			  << static_cast<int>(m[2]) << std::dec << std::endl;

	std::cout << "  pid: " << id.pid() << std::endl;
	std::cout << "  counter: " << id.counter() << std::endl << std::endl;

	// Convenience operators
	std::cout << "Convenience operators:" << std::endl;
	std::cout << "  id: " << id << std::endl;

	std::string s = xid::next();
	std::cout << "  s: " << s << std::endl << std::endl;

	// Decode id strings
	id = xid::id("cc0a2mn6i1e6brmdbip0");
	std::cout << "id (from string): " << id << std::endl;

	auto bytes = xid::id::decode("cc0aar76i1e6jr6no620");
	std::cout << "decoded bytes: " << std::endl;
	for (auto i = 0; i < bytes.size(); i++) {
		std::cout << std::setw(2) << i << std::setw(0) << ": " << std::bitset<8>(bytes[i])
				  << std::endl;
	}

	return EXIT_SUCCESS;
}
