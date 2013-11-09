#ifndef ORGANISM_H
#define ORGANISM_H

#include "point.h"
#include "MoveBehavior.h"



class Organism
{
    public:
        Organism();
        Organism(int type, int timeTillBreed, Point point);
        virtual ~Organism();

        bool breeding() const;
        int getType() const;
        Point getPoint() const;

        void setType(int type);
        void setPoint(const Point& point);
        void resetBreed();
        void breedTurn();

        void setMovement(MoveBehavior* mb);
        void movingOrganism();
    private:
        int type, breedPoints, breedTime;
        Point point;
        MoveBehavior* movement;
};

#endif // ORGANISM_H
