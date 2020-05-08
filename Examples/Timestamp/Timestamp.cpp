#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <..\..\timestamp.h>

// This example just uses each macro declared in timestamp.h
int main()
{
    printf("Year %d Month %d day %d\n", __TIMESTAMP_YEAR_INT__, __TIMESTAMP_MONTH_INT__, __TIMESTAMP_DAY_INT__);
    printf("Hour %d Minute %d Second %d\n", __TIMESTAMP_HOUR_INT__, __TIMESTAMP_MINUTE_INT__, __TIMESTAMP_SECONDS_INT__);
    printf("MSDOS Date %d Time %d\n", __TIMESTAMP_MSDOS_DATE_INT__, __TIMESTAMP_MSDOS_TIME_INT__);
    printf("ISO8601     Date: %s\n", __TIMESTAMP_ISO8601_DATE__);
    printf("ISO8601 DateTime: %s\n", __TIMESTAMP_ISO8601_DATETIME__);
    return 0;
}
