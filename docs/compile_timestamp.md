
# compile_timestamp.h

This repo provides header files that allow `C` and `C++` source
files to generate integer representations of the compile date/time.

These integer representations are `const` / `constexpr` compliant:

* `__TIMESTAMP_YEAR_INT__`
* `__TIMESTAMP_MONTH_INT__`
* `__TIMESTAMP_DAY_INT__`
* `__TIMESTAMP_HOUR_INT__`
* `__TIMESTAMP_MINUTE_INT__`
* `__TIMESTAMP_SECONDS_INT__`
* `__TIMESTAMP_MSDOS_DATE_INT__`
* `__TIMESTAMP_MSDOS_TIME_INT__`

The header also provides two `static const` null-terminated strings:

* `__TIMESTAMP_ISO8601_DATE__`, e.g., `"2022-12-25"`
* `__TIMESTAMP_ISO8601_DATETIME__`, e.g., `"2022-12-25T03:48:17"`

As a silly example, this allows generation of code that will
fail to compile if it was last modified on a leap-day:

```C
#include "compile_timestamp.h"

#if (__DATE_MONTH_INT__ == 2) && (__DATE_DAY_INT == 29)
  #error "Cannot compile on leap days"
#endif
```

More usefully, it allows for things such as creating embedded filesystems
to have a reasonable default date/time.  As an example, an early version
of `compile_date.h` was used by Adafruit's implementation of [UF2](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/661827c166989eeadbebe0ef7b4230793b678a4e/src/usb/uf2/ghostfat.c#L247-L254).
