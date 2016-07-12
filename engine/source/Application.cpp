#include "engine/Application.hpp"

Application::Application()
{ }

Application::~Application()
{ }

void Application::resize_event(int width, int height)
{
	glViewport(0, 0, width, height);
}
