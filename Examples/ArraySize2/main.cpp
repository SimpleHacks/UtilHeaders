
#define ARRAYSIZE2_SHOW_VERSION_MESSAGE

#include "ArraySize2.h"


int main()
{
    char stringArray[] = "hello";
    char otherArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };

    unsigned char result1 = ARRAY_SIZE2(stringArray); // should be 6 (trailing null)
    unsigned char result2 = ARRAY_SIZE2(otherArray);  // should be 9

    static_assert(ARRAY_SIZE2(stringArray) == 6, "alpha");
    static_assert(ARRAY_SIZE2(otherArray)  == 9, "beta");

    //printf("Results: %d, %d", result1, result2);
    return (result1 << 8) | result2;
}
