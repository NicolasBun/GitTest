// ======================================================================== //
// Copyright 2009-2011 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_PLATFORM_H__
#define __EMBREE_PLATFORM_H__

#include <stddef.h>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
/// detect platform
////////////////////////////////////////////////////////////////////////////////

/* detect 32 or 64 platform */
#if defined(__x86_64__) || defined(__ia64__) || defined(_M_X64)
#define __X86_64__
#endif

/* detect Linux platform */
#if defined(linux) || defined(__linux__) || defined(__LINUX__)
#  if !defined(__LINUX__)
#     define __LINUX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

/* detect FreeBSD platform */
#if defined(__FreeBSD__) || defined(__FREEBSD__)
#  if !defined(__FREEBSD__)
#     define __FREEBSD__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

/* detect Windows 95/98/NT/2000/XP/Vista/7 platform */
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)) && !defined(__CYGWIN__)
#  if !defined(__WIN32__)
#     define __WIN32__
#  endif
#endif

/* detect Cygwin platform */
#if defined(__CYGWIN__)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

/* detect MAC OS X platform */
#if defined(__APPLE__) || defined(MACOSX) || defined(__MACOSX__)
#  if !defined(__MACOSX__)
#     define __MACOSX__
#  endif
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

/* try to detect other Unix systems */
#if defined(__unix__) || defined (unix) || defined(__unix) || defined(_unix)
#  if !defined(__UNIX__)
#     define __UNIX__
#  endif
#endif

#if defined(_INCLUDED_IMM)
#  if !defined(__AVX__)
#    define __AVX__
#  endif
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1600) && !defined(__INTEL_COMPILER) || defined(_DEBUG) && defined(_WIN32)
#define __NO_AVX__
#endif

#if defined(_MSC_VER) && !defined(__SSE4_2__)
#define __SSE4_2__  //! activates SSE4.2 support
#endif

////////////////////////////////////////////////////////////////////////////////
/// Makros
////////////////////////////////////////////////////////////////////////////////

#ifdef __WIN32__
#define __dllexport extern "C" __declspec(dllexport)
#define __dllimport extern "C" __declspec(dllimport)
#else
#define __dllexport extern "C"
#define __dllimport extern "C"
#endif

#ifdef __WIN32__
#undef __noinline
#define __noinline             __declspec(noinline)
//#define __forceinline          __forceinline
//#define __restrict             __restrict
#define __thread               __declspec(thread)
#define __align(...)           __declspec(align(__VA_ARGS__))
//#define __FUNCTION__           __FUNCTION__
#define debugbreak()           __debugbreak()

#else
#undef __noinline
#undef __forceinline
#define __noinline             __attribute__((noinline))
#define __forceinline          inline __attribute__((always_inline))
//#define __restrict             __restrict
//#define __thread               __thread
#define __align(...)           __attribute__((aligned(__VA_ARGS__)))
#define __FUNCTION__           __PRETTY_FUNCTION__
#define debugbreak()           asm ("int $3")
#endif

#ifdef __GNUC__
    #define MAYBE_UNUSED __attribute__((used))
#else
    #define MAYBE_UNUSED
#endif

#if defined(_MSC_VER)
#define __builtin_expect(expr,b) expr
#endif

/* debug printing macros */
#define STRING(x) #x
#define PING std::cout << __FILE__ << " (" << __LINE__ << "): " << __FUNCTION__ << std::endl
#define PRINT(x) std::cout << STRING(x) << " = " << (x) << std::endl


////////////////////////////////////////////////////////////////////////////////
/// Basic Types
////////////////////////////////////////////////////////////////////////////////

typedef          long long  int64;
typedef unsigned long long uint64;
typedef                int  int32;
typedef unsigned       int uint32;
typedef              short  int16;
typedef unsigned     short uint16;
typedef               char   int8;
typedef unsigned      char  uint8;

#ifdef __WIN32__
#if defined(__X86_64__)
typedef int64 ssize_t;
#else
typedef int32 ssize_t;
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
/// Disable some compiler warnings
////////////////////////////////////////////////////////////////////////////////

#if defined(__INTEL_COMPILER)
#pragma warning(disable:265 ) // floating-point operation result is out of range
#pragma warning(disable:383 ) // value copied to temporary, reference to temporary used
#pragma warning(disable:869 ) // parameter was never referenced
#pragma warning(disable:981 ) // operands are evaluated in unspecified order
#pragma warning(disable:1418) // external function definition with no prior declaration
#pragma warning(disable:1419) // external declaration in primary source file
#pragma warning(disable:1572) // floating-point equality and inequality comparisons are unreliable
#endif

////////////////////////////////////////////////////////////////////////////////
/// Default Includes and Functions
////////////////////////////////////////////////////////////////////////////////

#include "constants.h"
#include "ref.h"

namespace embree
{
  /*! selects */
  __forceinline bool  select(bool s, bool  t , bool f) { return s ? t : f; }
  __forceinline int   select(bool s, int   t,   int f) { return s ? t : f; }
  __forceinline float select(bool s, float t, float f) { return s ? t : f; }

  /*! aligned malloc */
  void* alignedMalloc(size_t size, size_t align = 64);
  void* alignedRealloc(void* ptr, size_t size, size_t align = 64);
  void alignedFree(const void* ptr);

#define ALIGNED_CLASS \
public:\
  void* operator new(size_t size) { return alignedMalloc(size); } \
  void operator delete(void* ptr) { alignedFree(ptr); } \
private:

  /*! random functions */
  template<typename T> T          random() { return T(0); }
  template<> __forceinline int    random() { return int(rand()); }
  template<> __forceinline uint32 random() { return uint32(rand()); }
  template<> __forceinline float  random() { return random<uint32>()/float(RAND_MAX); }
  template<> __forceinline double random() { return random<uint32>()/double(RAND_MAX); }

  /** returns performance counter in seconds */
  double getSeconds();
}

#endif
