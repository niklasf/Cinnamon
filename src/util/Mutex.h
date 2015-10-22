/*
    Cinnamon UCI chess engine
    Copyright (C) Giuseppe Cannella

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once


#if defined(_WIN32)
//mutex on windows are slow
//https://msdn.microsoft.com/en-us/library/ms682530%28VS.85%29.aspx

#ifndef NOMINMAX
#  define NOMINMAX // Disable macros min() and max()
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX

class Mutex {
public:
    Mutex() { InitializeCriticalSection(&cs); }

    ~Mutex() { DeleteCriticalSection(&cs); }

    void lock() { EnterCriticalSection(&cs); }

    void unlock() { LeaveCriticalSection(&cs); }

private:
    CRITICAL_SECTION cs;
};

#else

#include <mutex>

class Mutex : public mutex {

};

#endif