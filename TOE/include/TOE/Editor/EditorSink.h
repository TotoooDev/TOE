#pragma once

#include <TOE/Editor/EditorLog.h>
#include <spdlog/sinks/base_sink.h>

namespace TOE
{
	template <typename Mutex>
	class EditorSink_ final : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		explicit EditorSink_(EditorLogConsole* console)
			: m_Console(console)
		{

		}

		EditorSink_(const EditorSink_&) = delete;
		EditorSink_& operator=(const EditorSink_&) = delete;

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
			m_Console->AddLog(fmt::to_string(formatted).c_str());
		}

		void flush_() override
		{

		}

	private:
		EditorLogConsole* m_Console;
	};

	using EditorSink = EditorSink_<std::mutex>;
}