#pragma once

#include <engine/config.h>

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <type_traits>
#include <string>
#include <list>

class Application;

#ifndef GOBLIN_DISABLE_LOG_MACROS
#define TRACE(...) Engine::ptr()->log()->trace(__VA_ARGS__)
#define DEBUG(...) Engine::ptr()->log()->debug(__VA_ARGS__)
#define INFO(...) Engine::ptr()->log()->info(__VA_ARGS__)
#define WARN(...) Engine::ptr()->log()->warn(__VA_ARGS__)
#define ERR(...) Engine::ptr()->log()->error(__VA_ARGS__)
#define FATAL(...) Engine::ptr()->log()->critical(__VA_ARGS__)
#else
#define TRACE(...)
#define DEBUG(...)
#define INFO(...)
#define WARN(...)
#define ERR(...)
#define FATAL(...)
#endif

class ENGINE_API Engine final
{
public:
	template<class AppType>
	static int run(int argc, char* argv[]);

	static inline Engine* ptr()
	{ return _inst.get(); }

	static inline Engine& ref()
	{ return *ptr(); }

	Engine(int argc, char** argv);

	virtual ~Engine();

	Engine(Engine const& other) = delete;

	Engine(Engine&& other) = default;

	Engine& operator=(Engine const& other) = delete;

	Engine& operator=(Engine&& other) = default;

	inline std::list<std::string> arguments() const
	{ return _args; }

	inline std::shared_ptr<spdlog::logger> log() const
	{ return _log; }

	glm::ivec2 framebuffer_size() const;

	glm::ivec2 window_size() const;

	inline float pixel_ratio() const
	{ return (float) framebuffer_size().x / (float) window_size().x; }

	inline void exit(int code = 0)
	{
		_running = false;
		_exit_code = code;
	}

private:
	static std::unique_ptr<Engine> _inst;

	int run(Application* app);

	std::shared_ptr<spdlog::logger> _log;
	std::list<std::string> _args;
	int _exit_code;
	bool _running;

	struct SDL_Window* _wnd;
};

#include "Application.hpp"

template<class AppType>
inline int Engine::run(int argc, char* argv[])
{
	static_assert(std::is_base_of<Application, AppType>::value, "AppType must be a subclass of Application");
	static_assert(std::is_constructible<AppType>::value, "AppType must have an empty constructor");

	_inst = std::make_unique<Engine>(argc, argv);
	return _inst->run(new AppType);
}
