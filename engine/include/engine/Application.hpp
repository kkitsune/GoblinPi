#pragma once

#include <engine/InputEnums.hpp>
#include <engine/Engine.hpp>
#include <engine/Time.hpp>

class ENGINE_API Application
{
public:
	virtual ~Application();

	Application(Application const& other) = delete;

	Application(Application&& other) = default;

	Application& operator=(Application const& other) = delete;

	Application& operator=(Application&& other) = default;

	static Engine& engine()
	{ return Engine::ref(); }

	virtual Duration fixed_time_step() const
	{ return duration_cast<Duration>(std::chrono::duration<Duration::rep, std::ratio<1, 30>>{1}); }

	virtual void initialize() = 0;

	virtual void update(Duration) = 0;

	virtual void frame_start()
	{ }

	virtual void frame()
	{ }

	virtual void frame_end()
	{ }

	virtual void mouse_button_event(glm::ivec2 const&, MouseButton, bool)
	{ }

	virtual void mouse_move_event(glm::ivec2 const&)
	{ }

	virtual void scroll_event(glm::ivec2 const&)
	{ }

	virtual void keyboard_event(Key, bool)
	{ }

	virtual void keyboard_character_event(char const*)
	{ }

	virtual void resize_event(int width, int height);

protected:
	Application();
};
