#include <engine/Application.hpp>
#include <engine/Painter.hpp>
#include <iostream>

using namespace std;

class ExampleApplication : public Application
{
	Painter* _p;

public:
	ExampleApplication(Engine* e) : Application(e), _p(nullptr)
	{
		cerr << "ExampleApplication => Constructed" << endl;
		for(auto i : engine().arguments())
			cout << " - " << i << endl;
		cout << endl;
	}

	virtual ~ExampleApplication()
	{
		cerr << "ExampleApplication => Destroyed" << endl;
	}

	virtual void initialize() override
	{
		cerr << "ExampleApplication => Initialized" << endl;
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
			cerr << "ExampleApplication => Escape pressed" << endl;
			engine().exit();
		}
	}

	virtual void resize_event(int width, int height) override
	{
		Application::resize_event(width, height);
	}
};

int main(int argc, char* argv[])
{ return Engine(argc, argv).run<ExampleApplication>(); }
