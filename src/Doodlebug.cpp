#include "../include/Doodlebug.h"
#include "../include/MoveRandom.h"

Doodlebug::Doodlebug() : Organism() {}
Doodlebug::Doodlebug(const Point& point) : Organism(DOODLEBUG_TYPE, DOODLEBUG_BREED_TIME, point), hunger(DOODLEBUG_HUNGER_LIMIT) {}

int Doodlebug::getDoodleType() const { return getType(); }
Point Doodlebug::getDoodleLocation() const { return getPoint(); }
bool Doodlebug::isBreeding()
{
    if(breeding()){
        resetBreed();
        return true;
    }
    return false;
}
bool Doodlebug::isAlive() const { return (hunger > 0); }

void Doodlebug::moving() {
    breedTurn();
    movingOrganism();
}
void Doodlebug::moving(const Point& newLocation) { setPoint(newLocation); moving(); }

void Doodlebug::noEat() { hunger--; }
void Doodlebug::eatAnt() { hunger = DOODLEBUG_HUNGER_LIMIT; }

void Doodlebug::setDoodleMovement(MoveRandom* mb) { setMovement(mb); }
