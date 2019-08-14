# ArraySize2.h

This header provides a **_safer_** compile-time array count macro, when using a C++ compiler.
When compiling for C99, a warning is generated indicating that use of this macro is not type-safe.
Each macro compiles to a `constexpr`.  This allows use in compile-time assertions or other `constexpr`
functions, for example.

A little history on this concept is likely useful.

<details>
    <summary>The common, unsafe macro</summary>

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


<details>
    <summary>Why the unsafe macro is dangerous</summary>

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

Even the following has a non-intuitive result, typically outputting `1`:
```C++
void bar(int beta[13])
{
  std::cout << ARRAY_SIZE(beta);
}
```


</details>


<details>
    <summary>Solution #1</summary>

<p>

### Solution #1

The header includes a solution published in the [March 6th, 2007](http://drdobbs.com/cpp/197800525?pgno=1)
Dr. Dobbs Journal, written by Ivan J. Johnson. 

Conceptually, this solution is simple:
```
if x is not an array
  issue a compile-time error
else
  use traditional non-typesafe C99 countof expression
```

How it ensures a compile-time error when it's not an
array is rather elegant....

<details>
    <summary>Check #1</summary>

<p>

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


<details>
    <summary>Check #2</summary>

<p>

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

<details>
    <summary>Combining Checks #1 and #2</summary>

<p>

Check #1 and Check #2, in combination, means the compiler has
**_EXCLUDED_** every possible type **_except for_** arrays via compilation
errors, before reaching the third line.  Both these checks multiply their
result by zero, and thus do not affect the final result.


</details>


<details>
    <summary>Line #3</summary>

<p>

The third line is, in essence the old C99 type-unsafe sizeof() macro:
`sizeof(arr) / sizeof((arr)[0])`
However, since that third line never compiles unless it's an array due
to check #1 or check #2, using this notation is now type-safe.

</details>

</details>

##### Other benefits
* Results of these macros are each `constexpr` compliant.
* At least solution #1 works even for local types.

# Enjoy!
