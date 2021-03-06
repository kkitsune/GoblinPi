#include "engine/Engine.hpp"

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;
using namespace glm;

unique_ptr<Engine> Engine::_inst{};

Engine::Engine(int argc, char* argv[]) : _log{}, _wnd{nullptr}
{
	for(int i = 0; i < argc; ++i)
		_args.push_back(string{argv[i]});

	std::vector<spdlog::sink_ptr> sinks
	{
		make_shared<spdlog::sinks::ansicolor_sink>(make_shared<spdlog::sinks::stdout_sink_st>()),
		make_shared<spdlog::sinks::simple_file_sink_st>("GoblinPi.log", true)
	};
	_log = make_shared<spdlog::logger>("engine", begin(sinks), end(sinks));
	_log->set_level(spdlog::level::trace);
	spdlog::register_logger(_log);

	string cmd;
	for(auto a : _args)
		cmd += a + " ";
	_log->warn("Invocation : {}", cmd);
}

Engine::~Engine()
{ }

int Engine::run(Application* app)
{
	TRACE("Engine::run => Begin");

	_running = true;
	_exit_code = 0;
	SDL_Init(SDL_INIT_VIDEO);

	SDL_DisplayMode ask_mode;
	ask_mode.format = SDL_PIXELFORMAT_RGBA8888;
	ask_mode.w = 1920;
	ask_mode.h = 1080;
	ask_mode.refresh_rate = 60;
	SDL_DisplayMode mode;
	SDL_GetClosestDisplayMode(0, &ask_mode, &mode);

	DEBUG("Engine::run => Display Mode selected : {}x{}@{}", mode.w, mode.h, mode.refresh_rate);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	_wnd = SDL_CreateWindow("OpenGL ES 2.0 Rendering Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
	SDL_SetWindowDisplayMode(_wnd, &mode);
	SDL_GLContext ctx = SDL_GL_CreateContext(_wnd);
	SDL_GL_MakeCurrent(_wnd, ctx);

	TRACE("Engine::run => Window created");

	gladLoadGLES2Loader((GLADloadproc) &SDL_GL_GetProcAddress);
	TRACE("Engine::run => OpenGL ES loaded");

	auto fb_size = framebuffer_size();
	glViewport(0, 0, fb_size.x, fb_size.y);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(_wnd);

	DEBUG("Engine::run => OpenGL Renderer : {}", glGetString(GL_RENDERER));
	DEBUG("Engine::run => OpenGL Version : {}", glGetString(GL_VERSION));
	DEBUG("Engine::run => OpenGL Vendor : {}", glGetString(GL_VENDOR));
	DEBUG("Engine::run => GLSL Version : {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

	TRACE("Engine::run => Initializing application");
	app->initialize();
	TRACE("Engine::run => Application initialized");

	TRACE("Engine::run => Entering main loop");
	TimePoint last_time{};
	Duration accumulator{};
	while(_running)
	{
		TimePoint new_time = Clock::now();
		Duration dT = new_time - last_time;
		last_time = new_time;
		accumulator += dT;

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_WINDOWEVENT_CLOSE)
				_running = false;
			if(e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
				app->keyboard_event(static_cast<Key>(e.key.keysym.scancode), e.type == SDL_KEYDOWN);
			if(e.type == SDL_TEXTINPUT)
				app->keyboard_character_event(e.text.text);
			if(e.type == SDL_MOUSEMOTION)
				app->mouse_move_event({e.motion.xrel, e.motion.yrel});
			if(e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEBUTTONDOWN)
				app->mouse_button_event({e.button.x, e.button.y}, static_cast<MouseButton>(e.button.button), e.type == SDL_MOUSEBUTTONDOWN);
			if(e.type == SDL_MOUSEWHEEL)
				app->scroll_event({e.wheel.x, e.wheel.y});
			if(e.type == SDL_WINDOWEVENT_RESIZED)
				app->resize_event(e.window.data1, e.window.data2);
		}

		if(accumulator >= app->fixed_time_step())
		{
			app->update(accumulator);
			accumulator = Duration{};
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		app->frame_start();
		app->frame();
		app->frame_end();

		SDL_GL_SwapWindow(_wnd);
	}

	TRACE("Engine::run => Exited main loop");
	delete app;

	SDL_GL_DeleteContext(ctx);
	SDL_DestroyWindow(_wnd);
	_wnd = nullptr;
	TRACE("Engine::run => Window destroyed");

	SDL_Quit();
	TRACE("Engine::run => Bye ({})\n", _exit_code);
	return _exit_code;
}

glm::ivec2 Engine::framebuffer_size() const
{
	ivec2 ret;
	SDL_GL_GetDrawableSize(_wnd, &ret.x, &ret.y);
	return ret;
}

glm::ivec2 Engine::window_size() const
{
	ivec2 ret;
	SDL_GetWindowSize(_wnd, &ret.x, &ret.y);
	return ret;
}
