#include <TOE/TOE.h>

class CustomLayer : public TOE::Layer
{
	virtual void OnCreate() override
	{
		// Sub to the keyboard events
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyPressedEvent);
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyUpEvent);
		
		Texture.CreateFromFile("image.png");

		// OpenGL data setup
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

		Ent = Scene.CreateEntity();
		Ent.AddComponent<TOE::RenderComponent>(VAO, EBO, Texture);

		CamEnt = Scene.CreateEntity();
		auto& camComponent = CamEnt.AddComponent<TOE::CameraComponent>(TOE::PerspectiveCamera());
		camComponent.Primary = true;
		auto& transform = CamEnt.GetComponent<TOE::TransformComponent>();
		transform = glm::translate(transform.Transform, glm::vec3(0.0f, 0.0f, 2.0f));
	}

	virtual void OnUpdate(double timestep) override
	{
		Scene.Update(timestep);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Renderer Stats");
		auto stats = TOE::Renderer::GetStats();
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Vertex count: %d", stats.VertexCount);
		ImGui::Text("Index count: %d", stats.IndexCount);
		ImGui::End();
	}

private:
	void KeyPressedEvent(TOE::KeyDownEvent* event)
	{
		switch (event->Keycode)
		{
		default:
			break;
		}
	}
	void KeyUpEvent(TOE::KeyUpEvent* event)
	{
		switch (event->Keycode)
		{
		default:
			break;
		}
	}

	TOE::Texture2D Texture;
	TOE::VAO VAO;
	TOE::EBO EBO;

	TOE::Scene Scene;
	TOE::Entity Ent, CamEnt;
};

TOE::Application* CreateApplication()
{
	TOE::WindowData data;
	TOE::Application* app = new TOE::Application(data);
	app->AddLayer(new CustomLayer());
	return app;
}