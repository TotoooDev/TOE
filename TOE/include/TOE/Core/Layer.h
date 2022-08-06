#pragma once

namespace TOE
{
	// Base layer class
	class Layer
	{
	public:
		virtual void OnCreate() {}
		virtual void OnUpdate(double timestep) {}
	};
}