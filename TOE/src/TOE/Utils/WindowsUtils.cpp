#include "pch.h"
#include <TOE/Utils/WindowsUtils.h>
#include <TOE/Core/Application.h>

#include <commdlg.h>

namespace TOE
{
	namespace Utils
	{
		std::string SaveFileDialog(const char* filter)
		{
			OPENFILENAMEA openFileName;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&openFileName, sizeof(OPENFILENAME));
			openFileName.lStructSize = sizeof(OPENFILENAME);
			openFileName.hwndOwner = glfwGetWin32Window(Application::Get().GetWindow().GetNativeWindow());
			openFileName.lpstrFile = szFile;
			openFileName.nMaxFile = sizeof(szFile);
			openFileName.lpstrFilter = filter;
			openFileName.nFilterIndex = 1;
			openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetSaveFileNameA(&openFileName) == TRUE)
			{
				return openFileName.lpstrFile;
			}
			return "";
		}

		std::string OpenFileDialog(const char* filter)
		{
			OPENFILENAMEA openFileName;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&openFileName, sizeof(OPENFILENAME));
			openFileName.lStructSize = sizeof(OPENFILENAME);
			openFileName.hwndOwner = glfwGetWin32Window(Application::Get().GetWindow().GetNativeWindow());
			openFileName.lpstrFile = szFile;
			openFileName.nMaxFile = sizeof(szFile);
			openFileName.lpstrFilter = filter;
			openFileName.nFilterIndex = 1;
			openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetOpenFileNameA(&openFileName) == TRUE)
			{
				return openFileName.lpstrFile;
			}
			return "";
		}
	}
}