#pragma once

#include <TOE/Scene/Scene.h>

namespace TOE
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene scene);

	private:
		Scene m_Scene;
	};
}