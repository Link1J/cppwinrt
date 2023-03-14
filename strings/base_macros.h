
#if defined(__GNUC__) && !defined(_MSC_VER) && !defined(__MINGW32__)
#define WINRT_NOT_ON_WINDOWS

#ifdef __i386__
#define _M_IX86 __i386__
#endif
#ifdef __amd64__
#define _M_X64 __amd64__
#endif
#ifdef __arm__
#define _M_ARM __arm__
#endif
#ifdef __aarch64__
#define _M_ARM64 __aarch64__
#endif

#if defined(_M_X64) || defined(_M_ARM64)
#define _WIN64
#endif

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __is_identifier
#define __is_identifier(x) 0
#endif

#if !__is_identifier(__declspec)
#define __declspec(...) __attribute__((__VA_ARGS__))
#if __has_attribute(selectany)
#define selectany weak
#endif
#endif  

#if !__is_identifier(__pragma)
#define __pragma _Pragma
#endif

#if !__is_identifier(__stdcall) && __has_attribute(stdcall)
#define __stdcall __attribute__((stdcall))
#endif

#define memcpy_s(a, b, c, d) memcpy(a, c, d)
#define swprintf_s(a, c, ...) swprintf(a, sizeof a / sizeof *a, c, __VA_ARGS__)

#define _InterlockedIncrement(a) __sync_add_and_fetch(a, 1)
#define _InterlockedDecrement(a) __sync_fetch_and_sub(a, 1)
#define _InterlockedCompareExchange64(a, b, c) __sync_val_compare_and_swap(a, c, b)

#define _InterlockedIncrement64(a) __sync_add_and_fetch(a, 1)
#define _InterlockedDecrement64(a) __sync_fetch_and_sub(a, 1)
#define _InterlockedCompareExchange128(a, b1, b2, c)                                                                   \
    __sync_val_compare_and_swap((__int128_t *)a, *(__int128_t *)c, (__int128_t)b1 << 64 | b2)

#define _InterlockedCompareExchangePointer(a, b, c) __sync_val_compare_and_swap(a, c, b)

#define _ReadWriteBarrier() asm volatile("" : : : "memory")

#pragma GCC diagnostic ignored "-Wattributes"

#endif

#ifdef _DEBUG

#define WINRT_ASSERT _ASSERTE
#define WINRT_VERIFY WINRT_ASSERT
#define WINRT_VERIFY_(result, expression) WINRT_ASSERT(result == expression)

#else

#define WINRT_ASSERT(expression) ((void)0)
#define WINRT_VERIFY(expression) (void)(expression)
#define WINRT_VERIFY_(result, expression) (void)(expression)

#endif

#define WINRT_IMPL_SHIM(...) (*(abi_t<__VA_ARGS__> **)&static_cast<__VA_ARGS__ const &>(static_cast<D const &>(*this)))

#ifdef _MSC_VER
// Note: this is a workaround for a false-positive warning produced by the Visual C++ 15.9 compiler.
#pragma warning(disable : 5046)

// Note: this is a workaround for a false-positive warning produced by the Visual C++ 16.3 compiler.
#pragma warning(disable : 4268)
#endif

#if defined(__cpp_lib_coroutine) || defined(__cpp_coroutines) || defined(_RESUMABLE_FUNCTIONS_SUPPORTED)
#define WINRT_IMPL_COROUTINES
#endif

#ifndef WINRT_EXPORT
#define WINRT_EXPORT
#endif

#ifdef WINRT_IMPL_NUMERICS
#define _WINDOWS_NUMERICS_NAMESPACE_ winrt::Windows::Foundation::Numerics
#define _WINDOWS_NUMERICS_BEGIN_NAMESPACE_ WINRT_EXPORT namespace winrt::Windows::Foundation::Numerics
#define _WINDOWS_NUMERICS_END_NAMESPACE_
#include <windowsnumerics.impl.h>
#undef _WINDOWS_NUMERICS_NAMESPACE_
#undef _WINDOWS_NUMERICS_BEGIN_NAMESPACE_
#undef _WINDOWS_NUMERICS_END_NAMESPACE_
#endif

#if defined(_MSC_VER)
#define WINRT_IMPL_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
#define WINRT_IMPL_NOINLINE __attribute__((noinline))
#else
#define WINRT_IMPL_NOINLINE
#endif

#if defined(_MSC_VER)
#define WINRT_IMPL_EMPTY_BASES __declspec(empty_bases)
#else
#define WINRT_IMPL_EMPTY_BASES
#endif

#if defined(_MSC_VER)
#define WINRT_IMPL_NOVTABLE __declspec(novtable)
#else
#define WINRT_IMPL_NOVTABLE
#endif

#if defined(__clang__)
#define WINRT_IMPL_HAS_DECLSPEC_UUID __has_declspec_attribute(uuid)
#elif defined(_MSC_VER)
#define WINRT_IMPL_HAS_DECLSPEC_UUID 1
#else
#define WINRT_IMPL_HAS_DECLSPEC_UUID 0
#endif

#ifdef __IUnknown_INTERFACE_DEFINED__
#define WINRT_IMPL_IUNKNOWN_DEFINED
#else
// Forward declare so we can talk about it.
struct IUnknown;
typedef struct _GUID GUID;
#endif

// std::source_location is a C++20 feature, which is above the C++17 feature floor for cppwinrt.  The source location
// needs to be the calling code, not cppwinrt itself, so that it is useful to developers building on top of this
// library.  As a result any public-facing method that can result in an error needs a default-constructed
// source_location argument.  Because this type does not exist in C++17 we need to use a macro to optionally add
// parameters and forwarding wherever appropriate.
//
// Some projects may decide to disable std::source_location support to prevent source code information from ending up in
// their release binaries, or to reduce binary size.  Defining WINRT_NO_SOURCE_LOCATION will prevent this feature from
// activating.
#if defined(__cpp_lib_source_location) && !defined(WINRT_NO_SOURCE_LOCATION)
#define WINRT_IMPL_SOURCE_LOCATION_ARGS_NO_DEFAULT , std::source_location const &sourceInformation
#define WINRT_IMPL_SOURCE_LOCATION_ARGS                                                                                \
    , std::source_location const &sourceInformation = std::source_location::current()
#define WINRT_IMPL_SOURCE_LOCATION_ARGS_SINGLE_PARAM                                                                   \
    std::source_location const &sourceInformation = std::source_location::current()

#define WINRT_IMPL_SOURCE_LOCATION_FORWARD , sourceInformation
#define WINRT_IMPL_SOURCE_LOCATION_FORWARD_SINGLE_PARAM sourceInformation

#define WINRT_SOURCE_LOCATION_ACTIVE

#ifdef _MSC_VER
#pragma detect_mismatch("WINRT_SOURCE_LOCATION", "true")
#endif
#else
#define WINRT_IMPL_SOURCE_LOCATION_ARGS_NO_DEFAULT
#define WINRT_IMPL_SOURCE_LOCATION_ARGS
#define WINRT_IMPL_SOURCE_LOCATION_ARGS_SINGLE_PARAM

#define WINRT_IMPL_SOURCE_LOCATION_FORWARD
#define WINRT_IMPL_SOURCE_LOCATION_FORWARD_SINGLE_PARAM

#ifdef _MSC_VER
#pragma detect_mismatch("WINRT_SOURCE_LOCATION", "false")
#endif
#endif
