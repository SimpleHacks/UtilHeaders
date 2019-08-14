
# Timestamp.h
This repo provides header files that allow `C` and `C++` source
files to generate integer representations of the date/time
that the source file was last modified.

This ability relies upon the compiler implementing the
macro '__TIMESTAMP__'.  This is supported by Microsoft's
compilers and gcc/g++ (__since what versions?__), and likely
other compilers.

This header relies upon the __TIMESTAMP__ macro having the
string format "as per asctime", which is defined as
> exactly 26 characters and has the form `Wed Jan 02 02:03:55 1980\n\0`


These integer representations are `const` / `constexpr` compliant:
* `__TIMESTAMP_YEAR_INT__`
* `__TIMESTAMP_MONTH_INT__`
* `__TIMESTAMP_DAY_INT__`
* `__TIMESTAMP_HOUR_INT__`
* `__TIMESTAMP_MINUTE_INT__`
* `__TIMESTAMP_SECONDS_INT__`
* `__TIMESTAMP_MSDOS_INT__`
* `__TIMESTAMP_MSDOS_INT__`

As a silly example, this prevents compilation of any file that was last
modified on a leap-day:

```C
#include "Timestamp.h"

#if (__TIMESTAMP_MONTH_INT__ == 2) && (__TIMESTAMP_DAY_INT == 29)
  #error "Files last modified on a leap day cannot be compiled"
#endif
```

More usefully, it allows for things such as creating embedded filesystems.
For example, if each file in the embedded file system has a corresponding
header file that defines its contents, that header file could also now
define a macro that corresponds to the last modification time of the file.
This allows the timestamp in the embedded file system to reflect the
last modification time of the files in an automated manner.

