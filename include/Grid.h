#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "Doodlebug.h"
#include "Ant.h"
#include "Point.h"
#include "windows.h"

namespace
{
    std::vector<Doodlebug> listOfDoodlebugs;
    std::vector<Ant> listOfAnts;

    bool checkIfEmptySides(const int& type, const Point& point);
    Point getOneSidePoint(const int& type, const Point& point, const int& side);
    void resetBreedingTime(const int& type, const Point& point);
    void isBreeding(const int& breedType, const Point& point);

    bool checkMovementIsValid(const int& type, const Point& point);
    void removeFromList(const int& removeThisType, const Point& point);
    void spawnInOrganisms(const int& amountDoodlebug, const int& amountAnts);

    void drawGrid();
}

namespace TheGrid
{
    class Grid
    {
        public:
            Grid();
            Grid(int gridSize);
            Grid(int width, int height);
            virtual ~Grid();

            void runGame();
    };
}

#endif // GRID_H
