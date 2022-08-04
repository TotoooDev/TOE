#include <TOE/TOE.h>

class CustomLayer : public TOE::Layer
{
	virtual void OnCreate() override
	{
		spdlog::info("Layer created");
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyPressedEvent);
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyUpEvent);
	}

	virtual void OnUpdate(double timestep) override
	{
		// spdlog::info("Layer updated, timestep = {}", timestep);
	}

private:
	void KeyPressedEvent(TOE::KeyDownEvent* event)
	{
		spdlog::info("Key pressed, keycode = {}", event->Keycode);
	}
	void KeyUpEvent(TOE::KeyUpEvent* event)
	{
		spdlog::info("Key up, keycode = {}", event->Keycode);
	}
};

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	TOE::Application* app = new TOE::Application(data);
	app->AddLayer(new CustomLayer());
	return app;
}