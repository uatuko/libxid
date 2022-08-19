#include "id.h"

#include <vector>

#include "exceptions.h"

static const char b32_alphabet[] = "0123456789abcdefghijklmnopqrstuv";

namespace xid {
id::id(data_t data) noexcept : _data(data) {}

id::id(const std::string &encoded) : _data(decode(encoded)) {}

id::counter_t id::counter() const noexcept {
	return (_data[9] << 16) | (_data[10] << 8) | (_data[11] << 0);
}

id::data_t id::decode(const std::string &encoded) {
	if (encoded.size() != 20) {
		throw invalid_id_string();
	}

	static std::vector<byte_t> map(256, 0xff);

	// Initialise decoder map
	if (map[118] != 0x1f) {
		for (auto i = 0; i < std::strlen(b32_alphabet); i++) {
			map[b32_alphabet[i]] = (i & 0xff);
		}
	}

	data_t data;
	data[11] = map[encoded[17]] << 6 | map[encoded[18]] << 1 | map[encoded[19]] >> 4;
	data[10] = map[encoded[16]] << 3 | map[encoded[17]] >> 2;
	data[9]  = map[encoded[14]] << 5 | map[encoded[15]];
	data[8]  = map[encoded[12]] << 7 | map[encoded[13]] << 2 | map[encoded[14]] >> 3;
	data[7]  = map[encoded[11]] << 4 | map[encoded[12]] >> 1;
	data[6]  = map[encoded[9]] << 6 | map[encoded[10]] << 1 | map[encoded[11]] >> 4;
	data[5]  = map[encoded[8]] << 3 | map[encoded[9]] >> 2;
	data[4]  = map[encoded[6]] << 5 | map[encoded[7]];
	data[3]  = map[encoded[4]] << 7 | map[encoded[5]] << 2 | map[encoded[6]] >> 3;
	data[2]  = map[encoded[3]] << 4 | map[encoded[4]] >> 1;
	data[1]  = map[encoded[1]] << 6 | map[encoded[2]] << 1 | map[encoded[3]] >> 4;
	data[0]  = map[encoded[0]] << 3 | map[encoded[1]] >> 2;

	// Safety checks.
	// Validate that there are no padding in data that would cause the re-encoded id to not equal
	// data.
	std::vector<byte_t> check(4, 0x00);
	check[3] = b32_alphabet[(data[11] << 4) & 0x1f];
	check[2] = b32_alphabet[(data[11] >> 1) & 0x1f];
	check[1] = b32_alphabet[(data[11] >> 6) & 0x1f | (data[10] << 2) & 0x1f];
	check[0] = b32_alphabet[data[10] >> 3];

	if (check[0] != encoded[16] || check[1] != encoded[17] || check[2] != encoded[18] ||
		check[3] != encoded[19]) {
		throw decode_validation_failure();
	}

	return data;
}

std::string id::encode() const noexcept {
	std::array<char, 20> chars;
	chars[19] = b32_alphabet[(_data[11] << 4) & 0x1f];
	chars[18] = b32_alphabet[(_data[11] >> 1) & 0x1f];
	chars[17] = b32_alphabet[(_data[11] >> 6) & 0x1f | (_data[10] << 2) & 0x1f];
	chars[16] = b32_alphabet[_data[10] >> 3];
	chars[15] = b32_alphabet[_data[9] & 0x1f];
	chars[14] = b32_alphabet[(_data[9] >> 5) | (_data[8] << 3) & 0x1f];
	chars[13] = b32_alphabet[(_data[8] >> 2) & 0x1f];
	chars[12] = b32_alphabet[_data[8] >> 7 | (_data[7] << 1) & 0x1f];
	chars[11] = b32_alphabet[(_data[7] >> 4) & 0x1f | (_data[6] << 4) & 0x1f];
	chars[10] = b32_alphabet[(_data[6] >> 1) & 0x1f];
	chars[9]  = b32_alphabet[(_data[6] >> 6) & 0x1f | (_data[5] << 2) & 0x1f];
	chars[8]  = b32_alphabet[_data[5] >> 3];
	chars[7]  = b32_alphabet[_data[4] & 0x1f];
	chars[6]  = b32_alphabet[_data[4] >> 5 | (_data[3] << 3) & 0x1f];
	chars[5]  = b32_alphabet[(_data[3] >> 2) & 0x1f];
	chars[4]  = b32_alphabet[_data[3] >> 7 | (_data[2] << 1) & 0x1f];
	chars[3]  = b32_alphabet[(_data[2] >> 4) & 0x1f | (_data[1] << 4) & 0x1f];
	chars[2]  = b32_alphabet[(_data[1] >> 1) & 0x1f];
	chars[1]  = b32_alphabet[(_data[1] >> 6) & 0x1f | (_data[0] << 2) & 0x1f];
	chars[0]  = b32_alphabet[_data[0] >> 3];

	return std::string(chars.data(), 20);
}

id::machine_t id::machine() const noexcept {
	return {
		_data[4],
		_data[5],
		_data[6],
	};
}

id::pid_t id::pid() const noexcept {
	return (_data[7] << 8) | (_data[8] << 0);
}

id::time_t id::time() const noexcept {
	return std::chrono::system_clock::from_time_t(
		(_data[0] << 24) | (_data[1] << 16) | (_data[2] << 8) | (_data[3] << 0));
}
} // namespace xid
