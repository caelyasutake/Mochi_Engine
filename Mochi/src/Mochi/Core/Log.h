#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Mochi {

	class MOCHI_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define MC_CORE_TRACE(...)	::Mochi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MC_CORE_INFO(...)	::Mochi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MC_CORE_WARN(...)	::Mochi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MC_CORE_ERROR(...)	::Mochi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MC_CORE_FATAL(...)	::Mochi::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define MC_TRACE(...)		::Mochi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MC_INFO(...)		::Mochi::Log::GetClientLogger()->info(__VA_ARGS__)
#define MC_WARN(...)		::Mochi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MC_ERROR(...)		::Mochi::Log::GetClientLogger()->error(__VA_ARGS__)
#define MC_FATAL(...)		::Mochi::Log::GetClientLogger()->fatal(__VA_ARGS__)