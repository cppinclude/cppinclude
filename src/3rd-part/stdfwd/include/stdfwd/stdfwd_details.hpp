#pragma once

// see https://stackoverflow.com/questions/31657499/how-to-detect-stdlib-libc-in-the-preprocessor
#include <ciso646>

//------------------------------------------------------------------------------

#if defined(_LIBCPP_VERSION)
#define STDFWD_IS_LIBCPP
#elif defined(__GLIBCXX__)
#define STDFWD_IS_LIBSTDCPP
#elif defined(_MSC_VER)
#define STDFWD_IS_MSVC
#else
#error "Unknown stdlib"
#endif

//------------------------------------------------------------------------------

#if defined(STDFWD_IS_LIBSTDCPP)

#if _GLIBCXX_USE_CXX11_ABI
	#define _GLIBCXX_BEGIN_NAMESPACE_CXX11_INLINE inline _GLIBCXX_BEGIN_NAMESPACE_CXX11
#else
	#define _GLIBCXX_BEGIN_NAMESPACE_CXX11_INLINE
#endif

#define STDFWD_STD_BEGIN														\
namespace std _GLIBCXX_VISIBILITY(default)										\
{																				\
	_GLIBCXX_BEGIN_NAMESPACE_VERSION											\

#define STDFWD_STD_PURE_BEGIN STDFWD_STD_BEGIN

#define STDFWD_STD_END } // namespace std_GLIBCXX_VISIBILITY(default)
#define STDFWD_STD_PURE_END STDFWD_STD_END

#elif defined(STDFWD_IS_MSVC)

#define STDFWD_STD_BEGIN _STD_BEGIN
#define STDFWD_STD_PURE_BEGIN STDFWD_STD_BEGIN

#define STDFWD_STD_END _STD_END
#define STDFWD_STD_PURE_END STDFWD_STD_END

#elif defined(STDFWD_IS_LIBCPP)

#define STDFWD_STD_BEGIN _LIBCPP_BEGIN_NAMESPACE_STD
#define STDFWD_STD_PURE_BEGIN namespace std {

#define STDFWD_STD_END _LIBCPP_END_NAMESPACE_STD
#define STDFWD_STD_PURE_END }

#else

#error "not implemented"

#endif

//------------------------------------------------------------------------------

#ifdef _GLIBCXX_BEGIN_NAMESPACE_CXX11_INLINE
	#define STDFWD_CPP11_INLINE_BEGIN _GLIBCXX_BEGIN_NAMESPACE_CXX11_INLINE
#else
	#define STDFWD_CPP11_INLINE_BEGIN
#endif

#ifdef _GLIBCXX_BEGIN_NAMESPACE_CXX11
	#define STDFWD_CPP11_BEGIN _GLIBCXX_BEGIN_NAMESPACE_CXX11
#else
	#define STDFWD_CPP11_BEGIN
#endif

#ifdef _GLIBCXX_END_NAMESPACE_CXX11
	#define STDFWD_CPP11_END _GLIBCXX_END_NAMESPACE_CXX11
#else
	#define STDFWD_CPP11_END
#endif

//------------------------------------------------------------------------------

#if ( defined(STDFWD_IS_MSVC) && defined (_LIBCPP_TEMPLATE_VIS) )
	#define STDFWD_TEMPLATE_VISIBILITY _LIBCPP_TEMPLATE_VIS
#else
	#define STDFWD_TEMPLATE_VISIBILITY
#endif

//------------------------------------------------------------------------------

// Visual Studio has bug https://stackoverflow.com/questions/14131454/visual-studio-2012-cplusplus-and-c-11
#if  defined(_MSVC_LANG)
	#define STDFWD_CPP_VERSION _MSVC_LANG
#elif defined(__cplusplus)
	#define STDFWD_CPP_VERSION __cplusplus
#endif

#if STDFWD_CPP_VERSION >= 201103L
	#define STDFWD_IS_CPP11
#endif

#if STDFWD_CPP_VERSION >= 201402L
	#define STDFWD_IS_CPP14
#endif

#if STDFWD_CPP_VERSION >= 201703L
	#define STDFWD_IS_CPP17
#endif

#if STDFWD_CPP_VERSION >= 202002L
	#define STDFWD_IS_CPP20
#endif
