# CPreprocessor

A simple C Preprocessor, the first homework for the Operating Systems Course

## Organization

Initially, the implementation of this program was split into two parts, a library and the main source code (`/lib` and `/src`). The library contained the data structures and some "general-use" functions. However, due to problems with nmake and windows compilation, I moved all the files into one folder, the source folder (`/src`). Each data structure is defined in a separate header file (and implemented in a corresponding .c file), each data structure building on top of another one (`hashmap` -> `list` -> `pair`).

I wanted to simulate objects (like in C++), so many structs have function pointers as properties. Any private function/property has a '_' at the start of the name. I wouldn't consider the implementation slow or fast (speed wasn't a focus, but I tend to avoid writing bad/slow code).

## Implementation

Most of the functionalities are implemented (ex. there is a bug with the multiline defines). Everything that I didn't implement is shown in the tests. On the other hand, there are no additional functionalities implemented.

I was limited in a lot of places by C, C89 and the fact that the same code had to run on windows: I couldn't use more specific types such as `int32_t`, `int8_t`, etc., I had to implement myself some functions that aren't cross platform.

Some of the difficulties I faced while writing this program were:

- you can't "nest" `strtok`'s, so I had to implement my own function
- if an error was found, the dynamically allocated memory had to be freed, before returning the error code back to the main function (almost all functions return an int)
- couldn't use hash functions from internet, so I written my own function (a very simple one, adds/multiplies the hash with the ASCII value of a char, depending on the parity of it's position)
  
```c
unsigned long hash_personal(string str) {
    unsigned long hash = 0;
    int c;

    uchar op = 0;

    while ((c = *str++)) {
        switch (op % 2) {
            case 0: {
                hash += c;
            } break;
            case 1: {
                hash *= c;
            } break;
        }

        op++;
    }

    return hash;
}
```

- "compressing" multiple lines ended by the __line continuation character__ (`\`) into one line (used by the multiline defines)

## Compilation & Running

- Linux: `make build`
- Windows: `nmake build`

To run the program, see [the problem statement](https://ocw.cs.pub.ro/courses/so/teme/tema-1) (it is written in romanian)

## Sources

- [OS Laboratory]()
- [Unused hash functions](http://www.cse.yorku.ca/~oz/hash.html)
- [Make template](https://gist.github.com/keeferrourke/fe72476a8dd8c4c02ff18eaed74e1de0
)
