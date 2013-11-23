#ifndef MOVEBEHAVIOR_H
#define MOVEBEHAVIOR_H

#include "GameBoard.h"
#include "Point.h"

class MoveBehavior
{
    public:
        virtual void moving(TheGrid::GameBoard& gameboard, Point* p, const int& type) = 0;
};

#endif // MOVEBEHAVIOR_H
