/**

    The MIT License (MIT)

    Copyright (c) SimpleHacks, Henry Gabryjelski

    All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

#ifndef ARRAYSIZE2_H
#define ARRAYSIZE2_H

/**
    The following, if defined prior to inclusion of this header file,
    will modify its behavior as noted:

        ARRAYSIZE2_SHOW_VERSION_MESSAGE
        -- if defined, will show which version of ARRAY_SIZE2 macro is selected
 */


// see example source at:
// [https://godbolt.org/z/FKilKo]
#ifndef __has_feature
    #define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

#if __cplusplus >= 201103L ||    /* any compiler claiming C++11 support */ \
    (_MSC_VER >= 1900 && __cplusplus != 199711L) ||    /* Visual C++ 2015 or higher           */ \
    __has_feature(cxx_constexpr) /* CLang versions supporting constexp  */

    #include <stddef.h> // required for size_t
    #if defined(ARRAYSIZE2_SHOW_VERSION_MESSAGE)
        #pragma message( "ARRAY_SIZE2 -- Using C++11 version" )
    #endif

    namespace detail
    {
        template <typename T, size_t N>
        constexpr size_t ARRAY_SIZE2_ARGUMENT_CANNOT_BE_POINTER(T const (&)[N]) noexcept
        {
            return N;
        }
    } // namespace detail
    #define ARRAY_SIZE2(arr) detail::ARRAY_SIZE2_ARGUMENT_CANNOT_BE_POINTER(arr)

#elif __cplusplus >= 199711L && ( /* C++ 98 trick */   \
      defined(__INTEL_COMPILER) ||                     \
      defined(__clang__) ||                            \
      (defined(__GNUC__) && (                          \
          (__GNUC__ > 4) ||                            \
          (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)       \
      )))

    #include <stddef.h> // required for size_t
    #if defined(ARRAYSIZE2_SHOW_VERSION_MESSAGE)
        #pragma message "ARRAY_SIZE2 -- Using C++98 version"
    #endif
    template <typename T, size_t N>
    char(&_ArraySizeHelperRequiresArray(T(&)[N]))[N];
    #define ARRAY_SIZE2(x) sizeof(_ArraySizeHelperRequiresArray(x))

#elif defined(__cplusplus) // && ((__cplusplus >= 199711L) || defined(__INTEL_COMPILER) || defined(__clang__))
    
    #if defined(ARRAYSIZE2_SHOW_VERSION_MESSAGE)
        #pragma message( "ARRAY_SIZE2 -- Using Ivan J. Johnson's C++ version" )
    #endif
    /*
        Works on older compilers, even Visual C++ 6....
        Created by Ivan J. Johnson, March 06, 2007
        See http://drdobbs.com/cpp/197800525?pgno=1
        
        Pseudocode:
        if x is not an array
        issue a compile-time error
        else
        use the traditional (non-typesafe) C99 COUNTOF expression
        
        If the argument is any of:
        object of class type, such as an std::vector
        floating-point type
        function pointer
        pointer-to-member
        then the first reinterpret_cast<> is not legal (compiler error)
        
        The type for check1 is chosen and named to help understand
        the cause of the error, because the class name is likely to
        appear in the compiler error message.
        
        If check1 succeeds, then the argument must be one of:
        an integral type
        an enumerated type
        a pointer to an object
        an array
        
        Check2 expands approximately to sizeof(check_type(x, &x)),
        where check_type is an overloaded function.
        Because this is purely a compile-time computation,
        the function is never really called or even implemented,
        but it lets the compiler apply overload resolution,
        which allows further type discrimination.
        There are three possibilities to consider:
        x is an integral type or enumerated type.
            In this case, neither of the two function overloads
            is a match, resulting in a compiler error.
        x is a pointer to an object.
            In this case, the first argument to check_type()
            is a pointer and the second one is a pointer-to-pointer.
            The best function match is the first overload of check_type,
            the one that returns an incomplete type (Is_pointer).
            However, because Is_pointer is an incomplete type,
            sizeof(Is_pointer) is not a valid expression,
            resulting in a compiler error.
        x is an array.
            In this case, the first argument to check_type()
            is an array and the second is a pointer-to-array.
            A pointer-to-array is *NOT* convertible to a
            pointer-to-pointer, so the first overload of
            check_type() is not a match.
            However, an array IS convertible to a pointer,
            and a pointer-to-array already is a pointer.
            Any pointer is convertible to a void*,
            so the second function overload is a match.
            That overload returns a complete type (Is_array).
            Because it's a complete type,
            sizeof(Is_array) is a valid expression.
        
        Thus, the compiler has EXCLUDED every possible type
        except arrays via compilation errors before reaching
        the third line.
        Moreover, check1 and check2 are reduced to the value zero,
        while the third line is the old type-unsafe C-style macro,
        now made entirely type-safe.
        
        Additional benefits:
        The result is itself constexpr
    */
    #define ARRAY_SIZE2(arr) ( \
       0 * sizeof(reinterpret_cast<const ::Bad_arg_to_ARRAY_SIZE2*>(arr)) + /*check1*/ \
       0 * sizeof(::Bad_arg_to_ARRAY_SIZE2::check_type((arr), &(arr)))    + /*check2*/ \
       sizeof(arr) / sizeof((arr)[0])                                       /* eval */ \
       )

    struct Bad_arg_to_ARRAY_SIZE2 {
       class Is_pointer; // incomplete
       class Is_array {};
       template <typename T>
       static Is_pointer check_type(const T*, const T* const*);
       static Is_array check_type(const void*, const void*);
    };

#elif !defined(__cplusplus) && defined(__GNUC__)

    // Even C can have type-safety for equivalent of ARRAY_SIZE() macro,
    // when using the following two GCC extensions:
    //     typeof()
    //     __builtin_types_compatible_p()

    #if defined(ARRAYSIZE2_SHOW_VERSION_MESSAGE)
        #pragma message( "ARRAY_SIZE2 -- Using GNUC version" )
    #endif

    // validated using:
    //   MSP430  gcc   4.5.3
    //   x86-64  icc  16.0.3
    //   x86-64  gcc   4.1.2
    //   x86-64 clang  3.0.0
    //   AVR     gcc   4.5.4
    //   ARM     gcc   4.5.4

    #define __SIMPLEHACKS_COMPATIBLE_TYPES__(a,b)   __builtin_types_compatible_p(typeof(a), typeof(b)) // GCC extensions
    #define __SIMPLEHACKS_BUILD_ERROR_ON_NONZERO__(x)  (sizeof(struct { int:-!!(x)*0x1ee7;})) // if x is zero, reports "error: negative width in bit-field '<anonymous>'"
    #define __SIMPLEHACKS_MUST_BE_ARRAY__(x)        __SIMPLEHACKS_BUILD_ERROR_ON_NONZERO__(__SIMPLEHACKS_COMPATIBLE_TYPES__((x), &(*x)))
    #define ARRAY_SIZE2(_arr)       ( (sizeof(_arr) / sizeof((_arr)[0])) + __SIMPLEHACKS_MUST_BE_ARRAY__(_arr) ) // compile-time error if not an array

#else

    // *** ALL *** compilers on godbolt.org as of 2020-05-08
    // now have a type-safe array element count macro defined.
    //
    // *** BOTH *** C++ and C source files are supported.
    // 
    // Thus, this appears to be 99.99% coverage.
    
    #error "Unable to provide type-safe ARRAY_SIZE2 macro"

    // The worst-case scenario macro, found in many places:
    //
    // #define ARRAY_SIZE(arr) ( sizeof(arr) / sizeof(arr[0]) ) <-- EVIL!
    //
    // While it is valid C, it is NOT typesafe.
    //
    // For example, if the parameter `arr` was originally an array,
    // the code would work.  If the code was then refactored, and the
    // parameter became a pointer, then the compiler will SILENTLY
    // give a (likely) incorrect result, because sizeof(void*) is rarely
    // equal to the array's true size.
    //
    // The good news is that all compilers on godbolt.org are now
    // fully supported.  Therefore, if a new compiler is found,
    // force a compile-time error, to avoid any suggestion that
    // the above would be a safe macro to define/use.

#endif


#endif // ARRAYSIZE2_H
