project(engine VERSION 0.1 LANGUAGES C CXX)

add_library(engine SHARED
		include/spdlog/details/pattern_formatter_impl.h
		include/spdlog/details/async_logger_impl.h
		include/spdlog/details/async_log_helper.h
		include/spdlog/details/mpmc_bounded_q.h
		include/spdlog/details/spdlog_impl.h
		include/spdlog/details/logger_impl.h
		include/spdlog/details/file_helper.h
		include/spdlog/details/null_mutex.h
		include/spdlog/details/registry.h
		include/spdlog/details/log_msg.h
		include/spdlog/details/format.h
		include/spdlog/details/os.h
		include/spdlog/sinks/ansicolor_sink.h
		include/spdlog/sinks/ostream_sink.h
		include/spdlog/sinks/stdout_sinks.h
		include/spdlog/sinks/android_sink.h
		include/spdlog/sinks/syslog_sink.h
		include/spdlog/sinks/file_sinks.h
		include/spdlog/sinks/base_sink.h
		include/spdlog/sinks/dist_sink.h
		include/spdlog/sinks/msvc_sink.h
		include/spdlog/sinks/null_sink.h
		include/spdlog/sinks/sink.h
		include/spdlog/async_logger.h
		include/spdlog/formatter.h
		include/spdlog/tweakme.h
		include/spdlog/common.h
		include/spdlog/logger.h
		include/spdlog/spdlog.h

		include/glad/glad.h
		source/deps/glad/glad.c

		source/deps/nanovg/nanovg_gl_utils.h
		source/deps/nanovg/stb_truetype.h
		source/deps/nanovg/stb_image.h
		source/deps/nanovg/nanovg_gl.h
		source/deps/nanovg/fontstash.h
		source/deps/nanovg/nanovg.h
		source/deps/nanovg/nanovg.c

		include/engine/resource/VertexArray.hpp
		include/engine/resource/Resource.hpp
		include/engine/resource/Buffer.hpp
		source/resource/VertexArray.cpp
		source/resource/Buffer.cpp

		include/engine/utils/BitmaskOperators.hpp
		include/engine/utils/Json.hpp
		include/engine/utils/Sol.hpp

		include/engine/Application.hpp
		include/engine/InputEnums.hpp
		include/engine/Painter.hpp
		include/engine/Engine.hpp
		include/engine/Time.hpp
		source/Application.cpp
		source/Painter.cpp
		source/Engine.cpp
)

target_include_directories(engine PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:include>
		PRIVATE source)

export(TARGETS engine FILE EngineConfig.cmake)
generate_export_header(engine
		EXPORT_FILE_NAME  include/engine/config.h
		EXPORT_MACRO_NAME ENGINE_API)

target_compile_options(engine
		PRIVATE ${DEFAULT_COMPILE_OPTIONS})

target_compile_definitions(engine
		PRIVATE ${DEFAULT_COMPILE_DEFINITIONS} engine_EXPORTS GLAD_GLAPI_EXPORT_BUILD
		PUBLIC GLM_FORCE_RADIANS GLAD_GLAPI_EXPORT)

target_link_libraries(engine ${DEFAULT_LINKER_OPTIONS}
		${SDL2_LIBRARIES} ${Boost_LIBRARIES})

install(TARGETS engine
		ARCHIVE  DESTINATION lib
		LIBRARY  DESTINATION lib
		RUNTIME  DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
