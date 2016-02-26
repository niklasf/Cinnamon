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

#if defined(DEBUG_MODE) || defined(FULL_TEST)

#include <gtest/gtest.h>
#include "../perft/Perft.h"

TEST(perftTest, oneCore) {
    Perft *perft = &Perft::getPointer();
    perft->setParam("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3, 1, 0, "", false);
    perft->start();
    perft->join();
    ASSERT_EQ(97862, perft->getResult());
    Perft::destroytInstance();
}

TEST(perftTest, twoCore) {
    Perft *perft = &Perft::getPointer();
    perft->setParam("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3, 4, 10, "", false);
    perft->start();
    perft->join();
    ASSERT_EQ(97862, perft->getResult());
    Perft::destroytInstance();
}

#ifdef FULL_TEST
TEST(perftTest, fullTest) {
    Perft *perft = &Perft::getPointer();
    perft->setParam("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 6, 4, 1000, "", false);
    perft->start();
    perft->join();
    ASSERT_EQ(8031647685, perft->getResult());
}

#endif
#endif