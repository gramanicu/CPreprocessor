# CPreprocessor

A simple C Preprocessor, the first homework for the Operating Systems Course

## LIB

All the functions inside the data library return 0 for no errors, positive numbers for non-critical errors and negative numbers for critical errors (if there are any exceptions to this rule, it is written in the function description)

- pair - use `make_spair` to create a new pair, `copy_spair` to copy-construct a new pair, and `clear_spair` to free a pair
- list - to "initialize a new list object", assign to the new `PairList` structure the `INIT_PAIRLIST` macro (0 size, no head, links all the functions to the struct)
- hashmap - first, the map must be initialized, assign the new `HashMap` structure the `INIT_HASHMAP` macro, then call the `init` function. A few things to be careful to:
  - make a new pair before adding it to the map, clear it after
  - make a new pair in which the search result will be stored. Clear it after reading the result.

TODO - make all the void function return int, the error codes.
