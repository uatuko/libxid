add_subdirectory(xid)

add_library(libxid INTERFACE)
target_link_libraries(libxid
	INTERFACE xid
)

target_include_directories(libxid
	INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_library(libxid::xid ALIAS libxid)

