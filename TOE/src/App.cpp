#include "pch.h"
#include <TOE/TOE.h>

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	TOE::Application* app = new TOE::Application(data);
	app->AddLayer(new TOE::EditorLayer());
	return app;
}