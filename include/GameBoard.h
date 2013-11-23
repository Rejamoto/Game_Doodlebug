#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <iostream>
#include "Point.h"
#include "windows.h"

namespace TheGrid
{
    class GameBoard
    {
        public:
            GameBoard();
            GameBoard(int gridSize);
            GameBoard(int width, int height);
            virtual ~GameBoard();

            int getGridBlock(const int _x,const int _y) const;
            int getGrid_X() const;
            int getGrid_Y() const;
            int** getGridArray() const;

            void setGridBlock(const int _x, const int _y, const int type);

            void createNewGrid();
            void createNewGrid(const int _size);
            void createNewGrid(const int _x, const int _y);

            int operator()(Point* point);
            int operator()(const int _x, const int _y) const;
            void operator()(const int _x, const int _y, const int _breed);

            void drawGrid(const int turns, const int ants, const int doodlebugs);
        private:
            int** grid;
            int x, y;
    };
}

#endif // GAMEBOARD_H
