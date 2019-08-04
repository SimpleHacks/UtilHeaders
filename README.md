
# CompileDate.h
This repo provides header files that allow `C` and `C++` source
files to generate integer representations of the compile date/time.

These integer representations are `const` / `constexpr` compliant.

As a silly example, this allows generation of code that will
fail to compile on leap-days:

```C
#include "compileDate.h"

#if (__MONTH_INT__ == 2) && (__DAY_INT == 29)
  #error "Cannot compile on leap days"
#endif
```

More usefully, it allows for things such as creating embedded filesystems
to have a reasonable default date/time.  As an example, this is used
by Adafruit's implementation of [UF2](https://github.com/adafruit/Adafruit_nRF52_Bootloader/blob/661827c166989eeadbebe0ef7b4230793b678a4e/src/usb/uf2/ghostfat.c#L247-L254).



