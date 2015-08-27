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

#include "_TPerftRes.h"
#include "../Search.h"
#include "../threadPool/Thread.h"
#include <iomanip>
#include <atomic>
#include <fstream>
#include <unistd.h>
#include "../util/Timer.h"
#include <mutex>

class PerftThread : public Thread, public GenMoves {
public:

    void setParam(string fen, int from, int to, _TPerftRes *);

    PerftThread();

    virtual ~PerftThread();

    virtual void run();

    virtual void endRun();

private:

    static const int N_MUTEX_BUCKET = 4096;
    static shared_timed_mutex MUTEX_BUCKET[N_MUTEX_BUCKET];

    static mutex mutexPrint;
    u64 tot = 0;

    template<int side, bool useHash, bool smp>
    u64 search(const int depth);

    int from, to;
    _TPerftRes *tPerftRes;
};

