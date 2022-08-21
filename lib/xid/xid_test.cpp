#include <vector>

#include <gtest/gtest.h>

#include "xid.h"

TEST(xid, next) {
	const int            n = 100;
	std::vector<xid::id> ids;

	// Generate n ids
	for (auto i = 0; i < n; i++) {
		ids.push_back(xid::next());
	}

	for (auto i = 1; i < n; i++) {
		auto previous = ids[i - 1];
		auto current  = ids[i];

		// Test for uniqueness among all other generated ids
		for (auto j = 0; j < ids.size(); j++) {
			if (j == i) {
				continue;
			}

			EXPECT_NE(current, ids[j]);
		}

		// Check that timestamp was incremented and is within 30 seconds of the previous one
		auto d = std::chrono::duration_cast<std::chrono::seconds>(current.time() - previous.time())
					 .count();
		EXPECT_LE(0, d);  // 0 <= d
		EXPECT_GT(30, d); // 30 > d

		// Check that machine ids are the same
		EXPECT_EQ(current.machine(), previous.machine());

		// Check that pids are the same
		EXPECT_EQ(current.pid(), previous.pid());

		// Test for proper increment
		auto diff = current.counter() - previous.counter();
		EXPECT_EQ(1, diff);
	}
}
