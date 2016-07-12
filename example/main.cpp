#include <engine/Application.hpp>
#include <engine/Painter.hpp>

using namespace std;

class ExampleApplication : public Application
{
	Painter* _p;

public:
	ExampleApplication() : _p(nullptr)
	{
		WARN("ExampleApplication => Constructed");
	}

	virtual ~ExampleApplication()
	{
		WARN("ExampleApplication => Destroyed");
	}

	virtual void initialize() override
	{
		WARN("ExampleApplication => Initialized");
		_p = new Painter;
	}

	virtual void update(Duration) override
	{ }

	virtual void frame_start() override
	{ }

	virtual void frame_end() override
	{
		_p->begin_frame(engine().framebuffer_size(), 1.f);
		_p->begin_path();
		_p->fill_color({1, 0, 0, 1});
		_p->circle({100, 100}, 75.f);
		_p->fill();
		_p->end_frame();
	}

	virtual void keyboard_event(Key key, bool pressed) override
	{
		if(key == Key::Escape && pressed)
		{
			WARN("ExampleApplication => Escape pressed");
			engine().exit();
		}
	}

	virtual void resize_event(int width, int height) override
	{
		Application::resize_event(width, height);
	}
};

int main(int argc, char* argv[])
{ return Engine::run<ExampleApplication>(argc, argv); }
