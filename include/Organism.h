#ifndef ORGANISM_H
#define ORGANISM_H

#include "point.h"
#include "MoveBehavior.h"
#include "GameBoard.h"

class Organism
{
    public:
        Organism();
        Organism(int type, int timeTillBreed, Point* point);
        Organism(int type, int timeTillBreed, Point* point, MoveBehavior* mb);

        bool breeding() const;
        int getType() const;

        Point getPoint() const;
        Point* getPointer() const;

        void setType(int type);
        void setPoint(const Point& point);
        void resetBreed();
        void breedTurn();

        void setMovement(MoveBehavior* mb);
        void movingOrganism(TheGrid::GameBoard& gb, Point* p);
    private:
        int type, breedPoints, breedTime;
        Point* point;
        MoveBehavior* movement;
};

#endif // ORGANISM_H
