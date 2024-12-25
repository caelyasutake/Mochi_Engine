#pragma once

#ifdef MC_PLATFORM_WINDOWS
	#ifdef MC_BUILD_DLL
		#define MOCHI_API __declspec(dllexport)
	#else 
		#define MOCHI_API __declspec(dllimport)
	#endif
#else 
	#error Mochi only supports Windows!
#endif