#pragma once

#include <TOE/Application.h>
#include <spdlog/spdlog.h>

// To be implemented by the user
extern TOE::Application* CreateApplication();

int main(int argc, char* argv[])
{
	spdlog::info("TOE is launching!");
	TOE::Application* app = CreateApplication();
	app->Run();
	return 0;
}