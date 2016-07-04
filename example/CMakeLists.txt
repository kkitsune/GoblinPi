project(example VERSION 0.1 LANGUAGES CXX)

add_executable(example MACOSX_BUNDLE
		main.cpp
)

target_link_libraries(example ${DEFAULT_LINKER_OPTIONS} engine
		${SDL2_LIBRARIES} ${Boost_LIBRARIES})

target_compile_options(example
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(example
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})
