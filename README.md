This repo provides header files that provide utility for `C` and `C++`
projects.  Each header has its own markdown file:

* [array_size2.h](./array_size2.h) - Provides a type-safe, `constexpr` compliant
  macros to get the count of elements in a statically-allocated array.
  See [array_size2.md](./array_size2.md) for more details.
* [compile_date.h](./CompileDate.h) - Provides `constexpr` compliant
  macros to get integers corresponding to the compilate date / time.
  See [CompileDate.md](./CompileDate.md) for more details.
* [constexpr_strlen.h](./constexpr_strlen.h) - Provides `constexpr` compliant
  strlen for string literals via template function constexpr_strlen().
  See [constexpr_strlen.md](./constexpr_strlen.md) for more details.
* [static_eval.h](./static_eval.h) - Provides a method to force a `constexpr`
  to be evaluated at compile-time, without polluting the namespace with enums.
  See [static_eval.md](./static_eval.md) for more details.
* [timestamp.h](./timestamp.h) - Provides `constexpr` compliant macros
  to get integers and strings corresponding to the last-modified-date
  of the file being compiled.
  See [timestamp.md](./timestamp.md) for more details.

# Enjoy!
