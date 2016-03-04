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

using namespace std;

template<class T>
class Singleton {
private:
    static T *_instance;
    static bool _pointer;
public:

    static T &getReference() {
        if (!_instance) {
            static T i;
            _pointer = false;
            _instance = &i;
        }
        return *_instance;
    }

    static T &getPointer() {
        if (!_instance) {
            _instance = new T;usare smart pointer
            _pointer = true;
        }
        return *_instance;
    }

    static void destroytInstance() {
        if (_instance && _pointer) {
            delete _instance;
            _instance = nullptr;
            _pointer = false;
        }
    }
};

template<class T>
T *Singleton<T>::_instance = nullptr;

template<class T>
bool Singleton<T>::_pointer = false;
