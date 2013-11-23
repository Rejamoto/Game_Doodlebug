#include "../include/MoveBehavior.h"
#include "../include/OrganismInfo.h"
#include <algorithm>
#include <chrono>

namespace Checks
{
    static std::mt19937 generate(std::chrono::system_clock::now().time_since_epoch().count());

    static bool checkIfEmptySides(TheGrid::GameBoard& gb, const Point* point, const int& type){
        int _x(gb.getGrid_X()), _y(gb.getGrid_Y());
        int xTemp(point->getX()), yTemp(point->getY());

        if( (xTemp < _x && xTemp >= 0) && (yTemp < _y && yTemp >= 0) ){
            if( (xTemp + 1) < _x && (gb(xTemp + 1, yTemp) == 0 || (gb(xTemp + 1, yTemp) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )             // Check if right side is empty
                return true;
            else if( (xTemp - 1) >= 0 && (gb(xTemp - 1, yTemp) == 0 || (gb(xTemp - 1, yTemp) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )        // Check if left side is empty
                return true;
            else if( (yTemp + 1) < _y && (gb(xTemp, yTemp + 1) == 0 || (gb(xTemp, yTemp + 1) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )        // Check if bottom is empty
                return true;
            else if( (yTemp - 1) >= 0 && (gb(xTemp, yTemp - 1) == 0 || (gb(xTemp, yTemp - 1) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )        // Check if top is empty
                return true;
            else
                return false;       // False if no sides are empty
        }
        return false;
    }

    static bool setPoint(TheGrid::GameBoard& gb, Point* point, const int& side, const int& type){
        int _x(gb.getGrid_X()), _y(gb.getGrid_Y());
        int xTemp(point->getX()), yTemp(point->getY());

        if( (xTemp < _x && xTemp >= 0) && (yTemp < _y && yTemp >= 0) ){
            if( side == 0 && (xTemp + 1) < gb.getGrid_X() && (gb(xTemp + 1, yTemp) == 0 || (gb(xTemp + 1, yTemp) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )        // Check if right side is empty
                point->setPoint((xTemp + 1), yTemp);
            else if( side == 1 && (xTemp - 1) >= 0 && (gb(xTemp - 1, yTemp) == 0 || (gb(xTemp - 1, yTemp) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )               // Check if left side is empty
                point->setPoint((xTemp - 1), yTemp);
            else if( side == 2 && (yTemp + 1) < gb.getGrid_Y() && (gb(xTemp, yTemp + 1) == 0 || (gb(xTemp, yTemp + 1) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )   // Check if bottom is empty
                point->setPoint(xTemp, (yTemp + 1));
            else if( side == 3 && (yTemp - 1) >= 0 && (gb(xTemp, yTemp - 1) == 0 || (gb(xTemp, yTemp - 1) == ((type == DOODLEBUG_TYPE) ? ANT_TYPE : 0))) )               // Check if top is empty
                point->setPoint(xTemp, (yTemp - 1));

            if( xTemp != point->getX() || yTemp != point->getY() ) {
                gb(xTemp, yTemp, 0);
                gb(point->getX(), point->getY(), type);
                return true;
            }
            return false;
        }
        return false;
    }
}
