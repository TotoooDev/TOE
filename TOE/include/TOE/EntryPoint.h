#pragma once

#include <TOE/Application.h>
#include <spdlog/spdlog.h>

extern TOE::Application* CreateApplication();

int main(int argc, char* argv[])
{
	spdlog::info("TOE is launching!");
	TOE::Application* app = CreateApplication();
	app->Run();
	return 0;
}