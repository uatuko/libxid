#include <gtest/gtest.h>

#include "id.h"

TEST(xid, id_parts) {
	struct Test {
		xid::id            id;
		xid::id::counter_t counter;
		xid::id::machine_t machine;
		xid::id::pid_t     pid;
		xid::id::time_t    time;
	};

	Test tests[] = {
		Test{
			.id =
				xid::id::data_t{
					0x4d, 0x88, 0xe1, 0x5b, 0x60, 0xf4, 0x86, 0xe4, 0x28, 0x41, 0x2d, 0xc9},
			.counter = 4271561,
			.machine = {0x60, 0xf4, 0x86},
			.pid     = 0xe428,
			.time    = std::chrono::system_clock::from_time_t(1300816219),
		},
		Test{
			.id =
				xid::id::data_t{
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
			.counter = 0,
			.machine = {0x00, 0x00, 0x00},
			.pid     = 0x00,
			.time    = std::chrono::system_clock::from_time_t(0),
		},
		Test{
			.id =
				xid::id::data_t{
					0x00, 0x00, 0x00, 0x00, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0x00, 0x00, 0x01},
			.counter = 1,
			.machine = {0xaa, 0xbb, 0xcc},
			.pid     = 0xddee,
			.time    = std::chrono::system_clock::from_time_t(0),
		},
	};

	for (const auto &test : tests) {
		EXPECT_EQ(test.counter, test.id.counter());
		EXPECT_EQ(test.machine, test.id.machine());
		EXPECT_EQ(test.pid, test.id.pid());
		EXPECT_EQ(test.time, test.id.time());
	}
}
