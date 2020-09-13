#pragma once

// For implementation of dll exporting and importing in Sandbox and Kaleidoscope w/o
// using __declspec
#ifdef KS_PLATFORM_WINDOWS
	#ifdef KS_BUILD_DLL
		#define KALEIDOSCOPE_API __declspec(dllexport)
	#else
		#define KALEIDOSCOPE_API __declspec(dllimport)
	#endif
#else
	#error Kaleidoscope only supports Windows
#endif
