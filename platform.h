#ifndef PLATFORM_H

#define PLATFORM_H

#include <stdint.h>


/*
	Compilers
*/
#ifndef	COMPILER_MSVC
	#define COMPILER_MSVC 		0
#endif

#ifndef COMPILER_LLVM
	#define COMPILER_LLVM 		0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
	#undef COMPILER_MSVC
	#define COMPILER_MSVC		1
#else
	#undef COMPILER_LLVM
	#define COMPILER_LLVM		1
#endif
#endif

typedef int8_t				int8;
typedef int16_t				int16;
typedef int32_t				int32;
typedef int64_t				int64;

typedef uint8_t				uint8;
typedef uint16_t			uint16;
typedef uint32_t			uint32;
typedef uint64_t			uint64;

typedef size_t				memory_index;
typedef float				real32;
typedef double				real64;

#define internal			static
#define local_persist		static
#define global_variable		static

#define PI32				3.141592653F



#endif // if !defined(HANDMADE_PLATFORM_H)
