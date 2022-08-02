#include <TOE/TOE.h>

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	return new TOE::Application(data);
}