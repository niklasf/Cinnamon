/*
    Cinnamon is a UCI chess engine
    Copyright (C) 2011-2014 Giuseppe Cannella

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
#include "IterativeDeeping.h"
IterativeDeeping::IterativeDeeping()  : maxDepth(MAX_PLY) {
#if defined(DEBUG_MODE)
    string parameterFile = "parameter.txt";
    if(!_file::fileExists(parameterFile)) {
        cout << "error file not found  " << parameterFile << endl ;
        return;
    }
    ifstream inData;
    string svalue, line;
    String param;
    int value;
    inData.open(parameterFile);
    while(!inData.eof()) {
        getline(inData, line);
        stringstream ss(line);
        ss >> param;
        ss >> svalue;
        value = stoi(svalue);
        setParameter(param, value);
    }
    inData.close();
#endif
}





IterativeDeeping::~IterativeDeeping() {
}


string IterativeDeeping::run() {
    _Tmove resultMove;
    struct timeb start1;
    struct timeb end1;
    _TpvLine line;
    int val = 0, tmp;
    string pvv;
    _Tmove move2;
    int TimeTaken = 0;
    setRunning(2);
    int mply = 0;
    forceCheck = false;
    int sc = 0;
    u64 totMoves = 0;
    string ponderMove;
    val = 0;
    mply = 0;
    startClock();
    clearKillerHeuristic();
    clearAge();
    ftime(&start1);
    memset(&resultMove, 0, sizeof(resultMove));
    ponderMove = "";
    int mateIn = INT_MAX;
    while(getRunning() && mateIn == INT_MAX) {
        init();
        ++mply;
        setMainPly(mply);
        if(mply == 1) {
            memset(&line, 0, sizeof(_TpvLine));
            mateIn = INT_MAX;
            val = search(mply, -_INFINITE, _INFINITE, &line, &mateIn);
        } else {
            memset(&line, 0, sizeof(_TpvLine));
            mateIn = INT_MAX;
            tmp = search(mply, val - VAL_WINDOW, val + VAL_WINDOW, &line, &mateIn);
            if(tmp <= val - VAL_WINDOW || tmp >= val + VAL_WINDOW) {
                memset(&line, 0, sizeof(_TpvLine));
                mateIn = INT_MAX;
                tmp = search(mply, val - VAL_WINDOW * 2, val + VAL_WINDOW * 2, &line, &mateIn);
            }
            if(tmp <= val - VAL_WINDOW * 2 || tmp >= val + VAL_WINDOW * 2) {
                memset(&line, 0, sizeof(_TpvLine));
                mateIn = INT_MAX;
                tmp = search(mply, val - VAL_WINDOW * 4, val + VAL_WINDOW * 4, &line, &mateIn);
            }
            if(tmp <= val - VAL_WINDOW * 4 || tmp >= val + VAL_WINDOW * 4) {
                memset(&line, 0, sizeof(_TpvLine));
                mateIn = INT_MAX;
                tmp = search(mply, -_INFINITE, _INFINITE, &line, &mateIn);
            }
            val = tmp;
        }
        /*  if ( mateIn != INT_MAX )
          {
              cout << "mate in: " << abs ( mateIn ) << endl;
          }*/
        if(!getRunning()) {
            break;
        }
        totMoves = 0;
        if(mply == 2) {
            setRunning(1);
        }
        memcpy(&move2, line.argmove, sizeof(_Tmove));
        pvv.clear();
        string pvvTmp;
        for(int t = 0; t < line.cmove; t++) {
            pvvTmp.clear();
            pvvTmp += decodeBoardinv(line.argmove[t].type, line.argmove[t].from, getSide());
            if(pvvTmp.length() != 4) {
                pvvTmp += decodeBoardinv(line.argmove[t].type, line.argmove[t].to, getSide());
            }
            pvv += pvvTmp;
            if(t == 1) {
                ponderMove = pvvTmp;
            }
            pvv += " ";
        };
        memcpy(&resultMove, &move2, sizeof(_Tmove));
        incKillerHeuristic(resultMove.from, resultMove.to, 0x800);
        ftime(&end1);
        TimeTaken = _time::diffTime(end1, start1);
        totMoves += getTotMoves();
        if(!pvv.length()) {
            break;
        }
        sc = resultMove.score ; /// 100;;
        if(resultMove.score > _INFINITE - 100) {
            sc = 0x7fffffff;
        }
        /*   if ( abs ( sc ) > _INFINITE )
           {
               cout << "info score mate 1 depth " << ( int ) mply << " nodes " << totMoves << " time " << TimeTaken << " pv " << pvv << endl;
           }
           else
           {
               cout << "info score cp " << sc << " depth " << ( int ) mply << " nodes " << totMoves << " time " << TimeTaken << " pv " << pvv << endl;
           }
        */
        if(forceCheck) {
            forceCheck = false;
            setRunning(1);
        } else if(abs(sc) > _INFINITE) {
            forceCheck = true;
            setRunning(2);
        }
        if(mply >= maxDepth - 1) {
            break;
        }
    }
    if(forceCheck && getRunning()) {
        while(forceCheck && getRunning());
        /* if ( abs ( sc ) > _INFINITE )
         {
             cout << "info score mate 1 depth " << ( int ) mply << " nodes " << totMoves << " time " << TimeTaken << " pv " << pvv << endl;
         }
         else
         {
             cout << "info score cp " << sc << " depth " << ( int ) mply << " nodes " << totMoves << " time " << TimeTaken << " pv " << pvv << endl;
         }*/
    }
    resultMove.capturedPiece = (resultMove.side ^ 1) == WHITE ? getPieceAt<WHITE> (POW2[resultMove.to]) : getPieceAt<BLACK> (POW2[resultMove.to]);
    string bestmove = decodeBoardinv(resultMove.type, resultMove.from, resultMove.side);
    if(!(resultMove.type & (KING_SIDE_CASTLE_MOVE_MASK | QUEEN_SIDE_CASTLE_MOVE_MASK))) {
        bestmove += decodeBoardinv(resultMove.type, resultMove.to, resultMove.side);
        if(resultMove.promotionPiece != -1) {
            bestmove += tolower(FEN_PIECE[(uchar) resultMove.promotionPiece]);
        }
    }
//   cout << "bestmove " << bestmove;
//cout << endl << flush;
    return bestmove;
}

