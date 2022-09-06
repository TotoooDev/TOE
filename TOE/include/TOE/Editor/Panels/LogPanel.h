#pragma once

#include <ImGui/imgui.h>

namespace TOE
{
	class LogPanel
	{
    public:
        ImGuiTextBuffer     Buf;
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool                AutoScroll;  // Keep scrolling if already at the bottom.

        LogPanel();

        void Clear();

        void AddLog(const char* fmt, ...) IM_FMTARGS(2);

        void Draw(const char* title, bool* p_open = NULL);
	};
}