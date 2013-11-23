#ifndef MOVERANDOM_H
#define MOVERANDOM_H

#include "MoveBehavior.h"
#include "Point.h"
#include "GameBoard.h"

class MoveRandom : public MoveBehavior
{
    public:
        MoveRandom() {}
        virtual ~MoveRandom() {}
        void moving(TheGrid::GameBoard& gameboard, Point* p, const int& type);
};

#endif // MOVERANDOM_H
