# memleakd

Minimal cross-platform memory leak detector.

## Usage

Consider the following program `foo.cc`:

```
int main()
{
    int **matrix = new int *[2];
    matrix[0] = new int[2];
    matrix[1] = new int[2];

    delete[] matrix[0];
    delete matrix[1];

    return 0;
}
```

Compile as usual, except adding `memleakd.cc` and its include path:

```
g++ -o foo foo.cc -Imemleakd memleakd/memleakd.cc
```

Output of running `foo`:

```
Memory leak at <Unknown>:0
Memory leak at <Unknown>:0
```

To locate the leak at source level, `#include` `"memleakd.h"` in your sources (right after the standard headers):

```
#include "memleakd.h"

int main()
{
```

Then the output for the above program would be:

```
Memory leak at foo.cc:7
Memory leak at foo.cc:5
```

## Limitations

1. Only works for memory allocated by `operator new` and `opreator new[]`

2. Not thread safe
