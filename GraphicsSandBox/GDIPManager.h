#pragma once

#include "ModWin.h"

class GDIPManager
{
public:
    GDIPManager();
    ~GDIPManager();
private:
    static ULONG_PTR token;
    static int refCount;
};