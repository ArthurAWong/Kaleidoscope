#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Kaleidoscope {
	class KALEIDOSCOPE_API Log {
		
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
#define KS_CORE_FATAL(...)		::Kaleidoscope::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define KS_TRACE(...)		::Kaleidoscope::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KS_INFO(...)		::Kaleidoscope::Log::GetClientLogger()->info(__VA_ARGS__)
#define KS_WARN(...)		::Kaleidoscope::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KS_ERROR(...)		::Kaleidoscope::Log::GetClientLogger()->error(__VA_ARGS__)
#define KS_FATAL(...)		::Kaleidoscope::Log::GetClientLogger()->fatal(__VA_ARGS__)