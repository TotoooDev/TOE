#pragma once

#include <TOE/Core/Application.h>
#include <TOE/Debug/Instrumentor.h>
#include <spdlog/spdlog.h>

// To be implemented by the user
extern TOE::Application* CreateApplication();

int main(int argc, char* argv[])
{
	TOE_PROFILE_BEGIN_SESSION("Startup", "logs/TOE_Startup.json");
	TOE::Application* app = CreateApplication();
	TOE_PROFILE_END_SESSION();

	TOE_PROFILE_BEGIN_SESSION("Runtime", "logs/TOE_Runtime.json");
	app->Run();
	TOE_PROFILE_END_SESSION();

	return 0;
}