#pragma once

#include <string>

namespace TOE
{
	namespace Utils
	{
		std::string SaveFileDialog(const char* filter);
		std::string OpenFileDialog(const char* filter);
	}
}