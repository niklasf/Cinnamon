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
#include <set>
#include "../SearchManager.h"

TEST(endgame, test1) {
    SearchManager &searchManager = Singleton<SearchManager>::getPointer();

    //KQKP
	searchManager.loadFen("k7/8/q7/8/8/8/1P6/1K6 w - - 0 1");
    int score = searchManager.getScore(WHITE, false);
    EXPECT_EQ(-890, score);
    score = searchManager.getScore(BLACK, false);
    EXPECT_EQ(890, score);

    //KRKP
    searchManager.loadFen("k7/8/r7/8/8/8/1P6/1K6 w - - 0 1");
    score = searchManager.getScore(WHITE, false);
    EXPECT_EQ(-514, score);
    score = searchManager.getScore(BLACK, false);
    EXPECT_EQ(514, score);

    //KQKR
    searchManager.loadFen("k7/8/q7/8/8/8/1R6/1K6 w - - 0 1");
    score = searchManager.getScore(WHITE, false);
    EXPECT_EQ(-560, score);
    score = searchManager.getScore(BLACK, false);
    EXPECT_EQ(560, score);

    //KRKB
    searchManager.loadFen("k7/8/r7/8/8/8/1B6/1K6 w - - 0 1");
    score = searchManager.getScore(WHITE, false);
    EXPECT_EQ(-90, score);
    score = searchManager.getScore(BLACK, false);
    EXPECT_EQ(90, score);

    //KRKN
    searchManager.loadFen("k7/8/r7/8/8/8/1N6/1K6 w - - 0 1");
    score = searchManager.getScore(WHITE, false);
    EXPECT_EQ(-148, score);
    score = searchManager.getScore(BLACK, false);
    EXPECT_EQ(148, score);

    //KBNK
    searchManager.loadFen("k7/8/b7/8/8/8/1N6/1K6 w - - 0 1");
    score = searchManager.getScore(WHITE, false);
    EXPECT_EQ(3, score);
    score = searchManager.getScore(BLACK, false);
    EXPECT_EQ(-3, score);

    SearchManager::destroytInstance();

}

#endif