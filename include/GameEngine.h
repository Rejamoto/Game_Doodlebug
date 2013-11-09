#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "GameBoard.h"
#include "Ant.h"
#include "Doodlebug.h"

namespace
{
    bool checkIfEmptySides(const int& type, const Point& point);
    Point getOneSidePoint(const int& type, const Point& point, const int& side);
    void resetBreedingTime(const int& type, const Point& point);
    void isBreeding(const int& breedType, const Point& point);

    bool checkMovementIsValid(const int& type, const Point& point);
    void removeFromList(const int& removeThisType, const Point& point);
    void spawnInOrganisms(const int& amountDoodlebug, const int& amountAnts);

    void drawGrid();
}

namespace DoodlebugGame
{
    class GameEngine
    {
        public:
            GameEngine();
            GameEngine(int gridSize);
            void runGame();
    };
}

#endif // GAMEENGINE_H
