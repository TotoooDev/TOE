#include <TOE/TOE.h>

class CustomLayer : public TOE::Layer
{
	virtual void OnCreate() override
	{
		spdlog::info("Layer created");

		// Sub to the keyboard events
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyPressedEvent);
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyUpEvent);

		Shader.LoadFromFile("shader.vert", "shader.frag");
		Shader.Use();
		Shader.SetInt("uTexture", 0);
		Texture.CreateFromFile("image.png");

		// OpenGL setup
		std::vector<float> vertices =
		{
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};
		std::vector<unsigned int> indices =
		{
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		TOE::VertexLayout layout;
		layout.AddAttribute(TOE::Type::Float, 3); // Pos
		layout.AddAttribute(TOE::Type::Float, 3); // Color
		layout.AddAttribute(TOE::Type::Float, 2); // Tex Coords

		VAO.SetData(vertices, layout);
		EBO.SetData(indices);
	}

	virtual void OnUpdate(double timestep) override
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Shader.Use();
		Texture.Use(0);
		VAO.Use();
		EBO.Use();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	void KeyPressedEvent(TOE::KeyDownEvent* event)
	{
		switch (event->Keycode)
		{

		}
	}
	void KeyUpEvent(TOE::KeyUpEvent* event)
	{
		switch (event->Keycode)
		{

		}
	}

	TOE::Shader Shader;
	TOE::Texture2D Texture;
	TOE::VAO VAO;
	TOE::EBO EBO;
};

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	TOE::Application* app = new TOE::Application(data);
	app->AddLayer(new CustomLayer());
	return app;
}