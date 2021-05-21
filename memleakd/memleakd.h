
/*

A minimal memory leak detector. Largely inspired by http://wyw.dcweb.cn/leakage.htm

Usage:
    Add "memleakd.cc" to your compilation targets, then build and run your program as usual.
    To locate the leak at source level, #include "memleakd.h" in your source files.

Limitations:
    1. Only works for memory allocated by operator new and opreator new[]
    2. Not thread safe


Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2021 Hank Meng (ymenghank@gmail.com)

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <new>

void *operator new(size_t size, const char *file, int line);
void *operator new[](size_t size, const char *file, int line);

#ifndef NO_REDEFINE_NEW
#define DEBUG_NEW new (__FILE__, __LINE__)
#define new DEBUG_NEW
#endif

class memleakd
{
public:
    memleakd();
    ~memleakd();

    void insert(void *addr, const char *file, int line);
    void remove(void *addr);

    static memleakd _single_news;
    static memleakd _array_news;

private:
    struct list_node_t;
    list_node_t *_M_head;
};
