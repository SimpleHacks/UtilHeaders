# ArraySize2.h

This header provides a **_safer_** compile-time array count macro, for both C and C++.
On C++, each macro compiles to a `constexpr`.  This allows use in compile-time assertions or other `constexpr`
functions, for example.  C does not have `constexpr`, but it is still determined at compile-time.

A little history on this concept is likely useful.

<hr/><details><summary>The common, unsafe macro</summary>

<p>

The internet is full of the following macro:
`#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])`

They tend to be used in code such as:
```C
void foo() {
  STRUCTURE_TYPE alpha[10];
  for (int i = 0; i < ARRAY_SIZE(alpha); i++) {
    alpha[i].FieldZeta = i; 
  }
  // do real work...  
}
void baz() {
  long a[] = { 7, 3, 5, -7, 12 };
  vector<long> v(a, a + ARRAY_SIZE(a));
  std::sort(a, a + ARRAY_SIZE(a));
  // use the sorted vector
}
```

</details>

<hr/><details><summary>Why the unsafe macro is dangerous</summary>

<p>

Even when used carefully, the above unsafe macro will **_silently fail_**
if (when) a later maintainer changes this to a dynamically allocated array,
or if the code is refactored so the array is passed as a parameter.
For example, if the code existed in multiple spots, someone might "helpfully"
refactor :

```C
void foo_array_manipulation(STRUCTURE_TYPE * alpha ) {
  for (int i = 0; i < ARRAY_SIZE(alpha); i++) {
    alpha[i].FieldZeta = i; 
  }
}
void foo() {
  STRUCTURE_TYPE alpha[10];
  foo_array_manipulation(alpha);
  // do real work...  
}
```

An examination of the above shows that the `ARRAY_SIZE()` macro
will now turn into:
```C++
sizeof(alpha) / sizeof(alpha[0]);
// which is...
sizeof(STRUCTURE_TYPE *) / sizeof(STRUCTURE_TYPE);
```

Since most pointers are larger than the structures,
this will compile to zero on most compilers.
In the above example, most compilers will fully optimize
away the entire `foo_array_manipulation()` function,
as it has no effect.

Although not intuitive, the following function has
the same exact result ... the parameter is actually
a pointer, not an array:

```C++
void bar(STRUCTURE_TYPE beta[13])
{
  std::cout << ARRAY_SIZE(beta);
}
```

</details>

<hr/><details><summary>Solution #1 (C++)</summary><P/>

The header includes a solution published in the [March 6th, 2007](http://drdobbs.com/cpp/197800525?pgno=1)
Dr. Dobbs Journal, written by Ivan J. Johnson.

Conceptually, this solution is simple:

```
if (x is not an array)
  issue a compile-time error
else
  use traditional non-typesafe C99 countof expression
```

How it ensures a compile-time error when it's not an
array is rather elegant....

<details><summary>Check #1</summary><P/>

`0 * sizeof(reinterpret_cast<const ::Bad_arg_to_COUNTOF*>(arr))`

Check #1 is not legal when the argument is any of the following:

* object of class type, such as `std::vector`
* floating-point type
* function pointer
* pointer-to-member

Any of these will cause a compile-time error. Naming the class
`::Bad_arg_to_COUNTOF` simply helps because the class name is
likely to appear in the compiler's error message.

Therefore, if check1 succeeded, then the parameter must be one of:

1. an integral type
2. an enumerated type
3. a pointer to an object
4. an array

</details>

<details><summary>Check #2</summary><P/>

`0 * sizeof(::Bad_arg_to_COUNTOF::check_type((arr), &(arr)))`

The meat of this expands to (approximately) `sizeof(check_type(x, &x))`,
where check_type() is an overloaded function.  Because this is purely
a compile-time computation, the function is never called or even needs
to have an implementation.  However, the compiler must apply overload
resolution, allowing further type discrimination.  There are two
function declared:

```c++
class Is_pointer; // incomplete
class Is_array {};

template <typename T>
static Is_pointer check_type(const T*, const T* const*);
static Is_array check_type(const void*, const void*);
```

When the parameter is ...

1. an integral type, neither of the two function overloads is a match,
   because they each take pointers.  This results in a compiler error.
2. an enumerated type, neither of the two function overloads is a match,
   because they each take pointers.  This results in a compiler error.
3. a pointer to an object, the first argument to `check_type()` is a pointer,
   and the second one is a pointer-to-a-pointer.  The best function match
   is the first overload of `check_type`, which returns type `Is_pointer`.
   However, because `Is_pointer` is an incomplete type, `sizeof(Is_pointer)`
   is not a valid expression, resulting in a compiler error.  Whew!
4. an array, the first argument to `check_type()` is an array, and the second
   is a pointer-to-array.  A pointer-to-array is *NOT* convertible to a
   pointer-to-a-pointer, so the first overload of `check_type()` is not a
   valid match.  However, an array IS convertible to a pointer, and a
   pointer-to-array already is a pointer.  Because any pointer is
   convertible to a `void *`, the second function overload *is* a match.
   That overload returns a complete type (`Is_array`).  Because it returns
   a complete type, `sizeof(Is_array)` is a valid expression.

</details>

<details><summary>Combining Checks #1 and #2</summary><P/>

Check #1 and Check #2, in combination, means the compiler has
**_EXCLUDED_** every possible type **_except for_** arrays via compilation
errors, before reaching the third line.  Both these checks multiply their
result by zero, and thus do not affect the final result.

</details>

<details><summary>Line #3</summary><P/>

The third line is, in essence the old C99 type-unsafe sizeof() macro:
`sizeof(arr) / sizeof((arr)[0])`
However, since that third line never compiles unless it's an array due
to check #1 or check #2, using this notation is now type-safe.

</details>

</details>

<hr/><details><summary>Solution #2 (C, w/ GNU extensions)</summary><P/>

<details><summary>Step 1 -- Macro that detects
if arguments are type-compatible</summary><P/>

The solution relies on two GNU extensions.
_(Both these extensions appear to exist for all
compilers currently on godbolt.org.)_

* __builtin_types_compatible_p() -- returns `1`
  if the two parameters are type-compatible, or
  `0` if the two parameters are not type-compatible.
* __typeof() -- gets the type of a parameter

Combining these provides a macro that returns
`1` when the types are the same, or `0` when
the types differ:

```C
#define __SIMPLEHACKS_COMPATIBLE_TYPES__(a,b) \
    __builtin_types_compatible_p(             \
        typeof(a),                            \
        typeof(b)                             \
        )
```

</details>

<details><summary>Step 2 -- Macro for compile-time error on zero</summary><P/>

GCC allows structs to contain bitfields of zero
length, and compilation will error out if the
bitfield is defined as having a negative length.
GCC also defines the size of a struct with a single, zero-length bitfield to be zero.

Based on the above, the following macro will
return a `(size_t)0` if the paramter evaluates
to zero, else result in a compile-time error.

```C
// reports "error: negative width in bit-field '<anonymous>'"
#define __SIMPLEHACKS_BUILD_ERROR_ON_NONZERO__(x) \
  (sizeof( struct { int:-!!(x) * 0x1ee7; } ) )
```


</details>

<details><summary>Step 3 -- Define macro that has compile-time error except for local arrays</summary><P/>

```C
#define __SIMPLEHACKS_MUST_BE_ARRAY__(x)  \
  __SIMPLEHACKS_BUILD_ERROR_ON_NONZERO__( \
    __SIMPLEHACKS_COMPATIBLE_TYPES__(     \
      (x), &(*x)
    )
  )
```

</details>

<details><summary>Step 4 -- Final macro</summary><P/>

The final macro matches the signature of the legacy
macro, for highest compatibility.

```C
#define ARRAY_SIZE2(_arr) (              \
  (sizeof(_arr) / sizeof((_arr)[0])) +   \
  __SIMPLEHACKS_MUST_BE_ARRAY__(_arr) )
```
</details>

<details><summary>Step 5 -- Walking through it</summary><P/>

Taking this one step at a time, from the inside out,
showing the results for both a locally declared array
and for everything else.

| Expression | Local Array | Else |
|-----|-----|------|
| `(x), &(*x)` | (local array) (pointer) | (x), (x) |
| ...COMPATIBLE_TYPES(...) | false | true |
| ...BUILD_ERROR...() | `(size_t)0` | compile error |
| ...ARRAY_SIZE2() | `old_macro + (size_t)0` | N/A |


 in a way that errors if argument is not an array

</details>


This allows 
to have a defined size in 
    #define __SIMPLEHACKS_BUILD_ERROR_ON_NONZERO__(x)  (sizeof(struct { int:-!!(x)*0x1ee7;})) // if x is zero, reports "error: negative width in bit-field '<anonymous>'"
    #define __SIMPLEHACKS_MUST_BE_ARRAY__(x)        __SIMPLEHACKS_BUILD_ERROR_ON_NONZERO__(__SIMPLEHACKS_COMPATIBLE_TYPES__((x), &(*x)))
    #define ARRAY_SIZE2(_arr)       ( (sizeof(_arr) / sizeof((_arr)[0])) + __SIMPLEHACKS_MUST_BE_ARRAY__(_arr) ) // compile-time error if not an array
asdfas

</details>
<hr/>

## Other benefits

* Results of these macros are each `constexpr` compliant
* The macros work, even for local types
* If no support for type-safe arrays, then the macro
  will NOT be defined, ensuring compilation error

## Enjoy
