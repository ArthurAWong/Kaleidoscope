#pragma once

#include "spdlog/spdlog.h"

namespace Kaleidoscope {
	class Log {
		
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static	std::shared_ptr<spdlog::logger> s_CoreLogger;
		static	std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define KS_CORE_TRACE(...)		::Kaleidoscope::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KS_CORE_INFO(...)		::Kaleidoscope::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KS_CORE_WARN(...)		::Kaleidoscope::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KS_CORE_ERROR(...)		::Kaleidoscope::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KS_CORE_CRITICAL(...)		::Kaleidoscope::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define KS_TRACE(...)		::Kaleidoscope::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KS_INFO(...)		::Kaleidoscope::Log::GetClientLogger()->info(__VA_ARGS__)
#define KS_WARN(...)		::Kaleidoscope::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KS_ERROR(...)		::Kaleidoscope::Log::GetClientLogger()->error(__VA_ARGS__)
#define KS_CRITICAL(...)		::Kaleidoscope::Log::GetClientLogger()->critical(__VA_ARGS__)