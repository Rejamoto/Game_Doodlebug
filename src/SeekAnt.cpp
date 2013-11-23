#include "../include/SeekAnt.h"
#include "../include/OrganismInfo.h"
#include "MoveBehavior.cpp"

namespace
{
    bool setPoint(TheGrid::GameBoard& gb, Point* point, const int& side, const int& type){
        int _x(gb.getGrid_X()), _y(gb.getGrid_Y());
        int xTemp(point->getX()), yTemp(point->getY());

        if( (xTemp < _x && xTemp >= 0) && (yTemp < _y && yTemp >= 0) ){
            if(((xTemp + 1) < gb.getGrid_X() && gb(xTemp + 1, yTemp) == ANT_TYPE) || ((xTemp - 1) >= 0 && gb(xTemp - 1, yTemp) == ANT_TYPE) || ((yTemp + 1) < gb.getGrid_Y() && gb(xTemp, yTemp + 1) == ANT_TYPE) || ((yTemp - 1) >= 0 && gb(xTemp, yTemp - 1) == ANT_TYPE)) {
                if( side == 0 && (xTemp + 1) < gb.getGrid_X() && (gb(xTemp + 1, yTemp) == ANT_TYPE) )        // Check if right side is empty
                    point->setPoint((xTemp + 1), yTemp);
                else if( side == 1 && (xTemp - 1) >= 0 && (gb(xTemp - 1, yTemp) == ANT_TYPE) )               // Check if left side is empty
                    point->setPoint((xTemp - 1), yTemp);
                else if( side == 2 && (yTemp + 1) < gb.getGrid_Y() && (gb(xTemp, yTemp + 1) == ANT_TYPE) )   // Check if bottom is empty
                    point->setPoint(xTemp, (yTemp + 1));
                else if( side == 3 && (yTemp - 1) >= 0 && (gb(xTemp, yTemp - 1) == ANT_TYPE) )               // Check if top is empty
                    point->setPoint(xTemp, (yTemp - 1));
            }
            else {
                if( side == 0 && (xTemp + 1) < gb.getGrid_X() && (gb(xTemp + 1, yTemp) == 0) )        // Check if right side is empty
                    point->setPoint((xTemp + 1), yTemp);
                else if( side == 1 && (xTemp - 1) >= 0 && (gb(xTemp - 1, yTemp) == 0) )               // Check if left side is empty
                    point->setPoint((xTemp - 1), yTemp);
                else if( side == 2 && (yTemp + 1) < gb.getGrid_Y() && (gb(xTemp, yTemp + 1) == 0) )   // Check if bottom is empty
                    point->setPoint(xTemp, (yTemp + 1));
                else if( side == 3 && (yTemp - 1) >= 0 && (gb(xTemp, yTemp - 1) == 0) )               // Check if top is empty
                    point->setPoint(xTemp, (yTemp - 1));
            }

            if( xTemp != point->getX() || yTemp != point->getY() ) {
                gb(xTemp, yTemp, 0);
                //gb(point->getX(), point->getY(), type);
                return true;
            }
            return false;
        }
        return false;
    }
}

void SeekAnt::moving(TheGrid::GameBoard& gameboard, Point* p, const int& type) {
    //std::cout << "Random Move" << std::endl;

    bool flags[4] = { false };
    while(Checks::checkIfEmptySides(gameboard, p, type)){      // Check if possible to move at all
        int randomNum(Checks::generate() % 4);

        while(flags[randomNum] && (flags[0] == false || flags[1] == false || flags[2] == false || flags[3] == false))      // Check if randomNum is not flagged
            randomNum = Checks::generate() % 4;

        if(setPoint(gameboard, p, randomNum, type)) break;

        flags[randomNum] = true;
    }
}
