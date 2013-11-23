#ifndef SEEKANT_H
#define SEEKANT_H

#include "MoveBehavior.h"
#include "GameBoard.h"
#include "Point.h"

class SeekAnt : public MoveBehavior
{
    public:
        SeekAnt() {}
        virtual ~SeekAnt() {}
        void moving(TheGrid::GameBoard& gameboard, Point* p, const int& type);
};

#endif // SEEKANT_H
