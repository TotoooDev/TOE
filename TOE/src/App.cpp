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
			 0.5f,  0.5f, 0.0f,  // top right
	         0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left  
		};
		unsigned int indices[] =
		{
			0, 1, 3,
			1, 2, 3
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	virtual void OnUpdate(double timestep) override
	{
		ProcessMovement();

		Shader.Use();
		Shader.SetMat4("uTransform", Transform);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

private:
	void KeyPressedEvent(TOE::KeyDownEvent* event)
	{
		switch (event->Keycode)
		{
		case TOE_KEY_R:
			Shader.Reload();
			spdlog::info("Shader reloaded");
			break;
		case TOE_KEY_UP:
			Movement.Up = true;
			break;
		case TOE_KEY_DOWN:
			Movement.Down = true;
			break;
		case TOE_KEY_RIGHT:
			Movement.Right = true;
			break;
		case TOE_KEY_LEFT:
			Movement.Left = true;
			break;
		}
	}
	void KeyUpEvent(TOE::KeyUpEvent* event)
	{
		switch (event->Keycode)
		{
		case TOE_KEY_UP:
			Movement.Up = false;
			break;
		case TOE_KEY_DOWN:
			Movement.Down = false;
			break;
		case TOE_KEY_RIGHT:
			Movement.Right = false;
			break;
		case TOE_KEY_LEFT:
			Movement.Left = false;
			break;
		}
	}
	void ProcessMovement()
	{
		if (Movement.Up)
		{
			Transform = glm::translate(Transform, glm::vec3(0.0f, 0.1f, 0.0f));
		}
		if (Movement.Down)
		{
			Transform = glm::translate(Transform, glm::vec3(0.0f, -0.1f, 0.0f));
		}
		if (Movement.Right)
		{
			Transform = glm::translate(Transform, glm::vec3(0.1f, 0.0f, 0.0f));
		}
		if (Movement.Left)
		{
			Transform = glm::translate(Transform, glm::vec3(-0.1f, 0.0f, 0.0f));
		}
	}

	TOE::Shader Shader;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	glm::mat4 Transform = glm::mat4(1.0f);

	struct
	{
		bool Up = false, Down = false, Right = false, Left = false;
	} Movement;
};

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	TOE::Application* app = new TOE::Application(data);
	app->AddLayer(new CustomLayer());
	return app;
}