#pragma once

#include <TOE/Core/Ref.h>
#include <TOE/Scene/Scene.h>
#include <TOE/Scene/Entity.h>
#include <string>

namespace TOE
{
	class ScenePanel
	{
	public:
		void SetCurrentScene(Ref<Scene> scene);
		void Draw(bool* isOpen);

	private:
		Ref<Scene> m_CurrentScene;
		Entity m_SelectedEntity;
		bool m_RenameScene = false;

		friend class PropertiesPanel;
	};
}