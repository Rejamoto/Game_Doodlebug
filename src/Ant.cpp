#include "../include/Ant.h"
#include "../include/MoveRandom.h"

Ant::Ant() {}
Ant::Ant(const Point& point) : Organism(ANT_TYPE, ANT_BREED_TIME, point) {}

int Ant::getAntType() const { return getType(); }
Point Ant::getAntLocation() const { return getPoint(); }
bool Ant::isBreeding()
{
    if(breeding()){
        resetBreed();
        return true;
    }
    return false;
}
void Ant::moving() {
    breedTurn();
    movingOrganism();
}
void Ant::moving(const Point& newLocation) { setPoint(newLocation); moving(); }

void Ant::setAntMovement(MoveRandom* mb) { setMovement(mb); }
