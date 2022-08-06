#include <TOE/TOE.h>

class CustomLayer : public TOE::Layer
{
	virtual void OnCreate() override
	{
		spdlog::info("Layer created");

		// Sub to the keyboard events
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyPressedEvent);
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyUpEvent);

		// Load the shader
		Shader.LoadFromFile("shader.vert", "shader.frag");

		// Triangle setup
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, // left  
			 0.5f, -0.5f, 0.0f, // right 
			 0.0f,  0.5f, 0.0f  // top   
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	virtual void OnUpdate(double timestep) override
	{
		Shader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	void KeyPressedEvent(TOE::KeyDownEvent* event)
	{
		spdlog::info("Key pressed, keycode = {}", event->Keycode);
		if (event->Keycode == TOE_KEY_R)
		{
			Shader.Reload();
			spdlog::info("Shader reloaded");
		}
	}
	void KeyUpEvent(TOE::KeyUpEvent* event)
	{
		spdlog::info("Key up, keycode = {}", event->Keycode);
	}

	TOE::Shader Shader;
	unsigned int VBO;
	unsigned int VAO;
};

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	TOE::Application* app = new TOE::Application(data);
	app->AddLayer(new CustomLayer());
	return app;
}