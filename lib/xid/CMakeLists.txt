add_library(xid
	xid.cpp
	xid.h
)

target_link_libraries(xid
	PRIVATE OpenSSL::Crypto
)

