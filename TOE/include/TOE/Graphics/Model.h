#pragma once

#include <TOE/Graphics/Mesh.h>

namespace TOE
{
	class Model
	{
	public:
		Model(std::vector<Mesh> meshes, const std::string& path)
			: m_Meshes(meshes), m_Path(path)
		{}

		std::vector<Mesh> GetMeshes() { return m_Meshes; }

	private:
		std::vector<Mesh> m_Meshes;
		std::string m_Path;
	};
}