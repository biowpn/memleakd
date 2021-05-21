
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

#define NO_REDEFINE_NEW
#include "memleakd.h"

#include <cstdio>
#include <cstdlib>

memleakd memleakd::_single_news;
memleakd memleakd::_array_news;

struct memleakd::list_node_t
{
    memleakd::list_node_t *prev;
    const char *file;
    int line;
    void *addr;
};

memleakd::memleakd()
{
    _M_head = nullptr;
}

memleakd::~memleakd()
{
    for (list_node_t *p; _M_head != nullptr;)
    {
        fprintf(stderr, "Memory leak at %s:%i\n", _M_head->file, _M_head->line);
        p = _M_head->prev;
        free(_M_head);
        _M_head = p;
    }
}

void memleakd::insert(void *addr, const char *file, int line)
{
    list_node_t *p = (list_node_t *)malloc(sizeof(list_node_t));
    p->file = file;
    p->line = line;
    p->addr = addr;
    p->prev = _M_head;
    _M_head = p;
}

void memleakd::remove(void *addr)
{
    for (list_node_t *cur = _M_head, *last = nullptr; cur != nullptr; last = cur, cur = cur->prev)
    {
        if (cur->addr == addr)
        {
            (last == nullptr ? _M_head : last->prev) = cur->prev;
            free(cur);
            break;
        }
    }
}

void *operator new(size_t size)
{
    return operator new(size, "<Unknown>", 0);
}

void *operator new[](size_t size)
{
    return operator new[](size, "<Unknown>", 0);
}

void *operator new(size_t size, const char *file, int line)
{
    void *addr = malloc(size);
    memleakd::_single_news.insert(addr, file, line);
    return addr;
}

void *operator new[](size_t size, const char *file, int line)
{
    void *addr = malloc(size);
    memleakd::_array_news.insert(addr, file, line);
    return addr;
}

void operator delete(void *addr) noexcept
{
    free(addr);
    memleakd::_single_news.remove(addr);
}

void operator delete(void *addr, std::size_t size) noexcept
{
    operator delete(addr);
}

void operator delete[](void *addr) noexcept
{
    free(addr);
    memleakd::_array_news.remove(addr);
}
