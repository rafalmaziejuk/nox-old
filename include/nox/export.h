#pragma once

// clang-format off
#if defined(NOX_SHARED)
	#if defined(NOX_WIN32)
		#define NOX_EXPORT __declspec(dllexport)
	#elif defined(NOX_UNIX)
		#define NOX_EXPORT __attribute__((__visibility__("default")))
	#else
		#error Operating system not supported! 
	#endif
#else
	#define NOX_EXPORT
#endif
// clang-format on
