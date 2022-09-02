#include <TOE/TOE.h>

class CustomLayer : public TOE::Layer
{
	virtual void OnCreate() override
	{
		TOE_PROFILE_FUNCTION();

		// Sub to the keyboard events
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyPressedEvent);
		TOE::Application::Get().EventBus.Subscribe(this, &CustomLayer::KeyUpEvent);

		TOE::Ref<TOE::Texture2D> Texture = TOE::CreateRef<TOE::Texture2D>();
		TOE::Ref<TOE::VAO> VAO = TOE::CreateRef<TOE::VAO>();
		TOE::Ref<TOE::EBO> EBO = TOE::CreateRef<TOE::EBO>();
		Framebuffer = TOE::CreateRef<TOE::Framebuffer>();
		
		Texture->CreateFromFile("image.png");
		
		TOE::FramebufferData data;
		data.Width = TOE::Application::Get().GetWindowData().Width;
		data.Height = TOE::Application::Get().GetWindowData().Height;
		Framebuffer->Create(data);

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

		VAO->SetData(vertices, layout);
		EBO->SetData(indices);

		Ent = Scene.CreateEntity();
		Ent.AddComponent<TOE::RenderComponent>(VAO, EBO, Texture);

		CamEnt = Scene.CreateEntity();
		auto& camComponent = CamEnt.AddComponent<TOE::CameraComponent>(TOE::CreateRef<TOE::PerspectiveCamera>(TOE::PerspectiveCamera()));
		camComponent.Primary = true;
		auto& transform = CamEnt.GetComponent<TOE::TransformComponent>();
		transform = glm::translate(transform.Transform, glm::vec3(0.0f, 0.0f, 2.0f));
	}

	virtual void OnUpdate(double timestep) override
	{
		Framebuffer->Use();
		Scene.Update(timestep);
		Framebuffer->Unbind();
	}

	virtual void OnImGuiRender() override
	{
		TOE_PROFILE_FUNCTION();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close"))
					TOE::Application::Get().Stop();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Scene viewport");
		ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
		if (currentViewportSize.x != m_ViewportSize.x || currentViewportSize.y != m_ViewportSize.y)
		{
			m_ViewportSize = currentViewportSize;
			Framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
		}
		ImGui::Image((void*)Framebuffer->GetColorAttachmentID(), ImGui::GetContentRegionAvail(), ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();

		// Settings window
		ImGui::Begin("Settings");
		auto stats = TOE::Renderer::GetStats();
		ImGui::Text("Renderer stats:");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Vertex count: %d", stats.VertexCount);
		ImGui::Text("Index count: %d", stats.IndexCount);
		ImGui::End();
	}

private:
	void KeyPressedEvent(TOE::KeyDownEvent* event)
	{
		// switch (event->Keycode)
		// {
		// default:
		// 	break;
		// }
	}
	void KeyUpEvent(TOE::KeyUpEvent* event)
	{
		// switch (event->Keycode)
		// {
		// default:
		// 	break;
		// }
	}

	TOE::Ref<TOE::Framebuffer> Framebuffer;
	ImVec2 m_ViewportSize;

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