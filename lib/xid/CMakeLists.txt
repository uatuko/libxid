add_library(xid
	exceptions.h
	id.cpp
	id.h
	xid.cpp
	xid.h
)

target_link_libraries(xid
	PRIVATE OpenSSL::Crypto
)

# tests
if (LIBXID_ENABLE_TESTING)
	add_executable(xid_tests
		id_test.cpp
	)

	target_link_libraries(xid_tests
		PRIVATE xid
		PRIVATE gtest_main
	)

	if (LIBXID_ENABLE_COVERAGE)
		target_compile_options(xid_tests
			PRIVATE --coverage
		)

		target_link_options(xid_tests
			PRIVATE -fprofile-arcs
		)
	endif()

	include(GoogleTest)
	gtest_discover_tests(xid_tests)
endif()

