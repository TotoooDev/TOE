#pragma once

namespace TOE
{
	class Application
	{
	public:
		Application();
		~Application();

		static Application& Get();

		void Run();
		void Stop();

	private:
		static Application* m_Instance;

		bool m_IsRunning = true;
	};
}