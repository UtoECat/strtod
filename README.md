# Custom `strtod` implementation

A custom implementation of `strtod` which functions very similar to the standard C `strtod` function with some minor differences:
2. by default, `errno` is not set to `ERANGE` when the normalized value of the number would be less tha `DBL_MIN` but is instead set if the resulting double would be 0 where the value is not 0.
3. where `strtod` can check for the `0x` prefix tp parse hexadecimals, `c8_strToD` does not by default. 

*There may be more inconsistancies. These are the minor ones due to design, simplicity, or what I have found through testing...*

## Options
There are no toggable options remain :D
