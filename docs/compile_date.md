
# CompileDate.h
This repo provides header files that allow `C` and `C++` source
files to generate integer representations of the compile date/time.

These integer representations are `const` / `constexpr` compliant:
* `__DATE_YEAR_INT__`
* `__DATE_MONTH_INT__`
* `__DATE_DAY_INT__`
* `__TIME_HOUR_INT__`
* `__TIME_MINUTE_INT__`
* `__TIME_SECONDS_INT__`
* `__DATE_MSDOS_INT__`
* `__TIME_MSDOS_INT__`

As a silly example, this allows generation of code that will
fail to compile on leap-days:

```C
#include "compileDate.h"

#if (__DATE_MONTH_INT__ == 2) && (__DATE_DAY_INT == 29)
  #error "Cannot compile on leap days"
#endif
```

More usefully, it allows for things such as creating embedded filesystems
to have a reasonable default date/time.  As an example, an early version
of this is used by Adafruit's implementation of [UF2](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/661827c166989eeadbebe0ef7b4230793b678a4e/src/usb/uf2/ghostfat.c#L247-L254).
