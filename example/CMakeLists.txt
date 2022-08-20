add_executable(${PROJECT_NAME}_example EXCLUDE_FROM_ALL
	main.cpp
)

target_link_libraries(${PROJECT_NAME}_example
	PRIVATE libxid::xid
)

