#pragma once

#include <stdexcept>

namespace xid {
struct invalid_id_string : public std::invalid_argument {
	invalid_id_string() : std::invalid_argument("ID string must be 20 characters") {}
};

struct decode_validation_failure : public std::runtime_error {
	decode_validation_failure() : std::runtime_error("Validation checks on decoded ID failed") {}
};
} // namespace xid
