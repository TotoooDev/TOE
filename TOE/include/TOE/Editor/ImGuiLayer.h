#pragma once

#include <TOE/Core/Layer.h>

namespace TOE
{
	class ImGuiLayer : public Layer
	{
	public:
		~ImGuiLayer();

		virtual void OnCreate() override;

		void Begin();
		void End();

		void SetDarkColors();

	private:

	};
}