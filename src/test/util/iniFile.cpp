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
#include "../../util/IniFile.h"

TEST(IniFile, test1) {
    const string fileName = "inifile.tmp";
    std::ofstream fout(fileName);
    fout << "Hello#a";
    fout.close();

    IniFile iniFile(fileName);
    pair<string, string>* p=iniFile.get();
_assert(p);
    ASSERT_EQ("Hello", p->first);
    ASSERT_EQ("a", p->second);
}

#endif