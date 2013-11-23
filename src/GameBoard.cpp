#include "../include/GameBoard.h"

namespace TheGrid
{
    GameBoard::GameBoard() : GameBoard(5, 5) {}
    GameBoard::GameBoard(int gridSize) : GameBoard(gridSize, gridSize) {}
    GameBoard::GameBoard(int xGrid, int yGrid) : x(xGrid), y(yGrid) {
        grid = new int*[x];
        for(int i = 0; i < x; i++)
            grid[i] = new int[y];

        for(int i = 0; i < x; i++)
            for(int j = 0; j < y; j++)
                grid[i][j] = 0;
    }
    GameBoard::~GameBoard() { delete[] grid; }

    int GameBoard::getGridBlock(const int _x, const int _y) const { return grid[_x][_y]; }
    int GameBoard::getGrid_X() const { return x; }
    int GameBoard::getGrid_Y() const { return y; }
    int** GameBoard::getGridArray() const { return grid; }

    void GameBoard::setGridBlock(const int _x, const int _y, const int type) { grid[_x][_y] = type; }

    void GameBoard::createNewGrid() {
        createNewGrid(5);
    }
    void GameBoard::createNewGrid(const int _size) {
        createNewGrid(_size, _size);
    }
    void GameBoard::createNewGrid(const int _x, const int _y) {
        delete[] grid;

        x = _x;
        y = _y;
        grid = new int*[x];
        for(int i = 0; i < x; i++)
            grid[i] = new int[y];

        for(int i = 0; i < x; i++)
            for(int j = 0; j < y; j++)
                grid[i][j] = 0;
    }

    int GameBoard::operator()(Point* point){
        return grid[point->getX()][point->getY()];
    }
    int GameBoard::operator()(const int _x, const int _y) const {
        return grid[_x][_y];
    }
    void GameBoard::operator()(const int _x, const int _y, const int _breed) {
        grid[_x][_y] = _breed;
    }

    void GameBoard::drawGrid(const int turns, const int ants, const int doodlebugs) {
        system("cls");
        std::cout << "Turn " << turns << std::endl;
        for(int i = 0; i < x; i++){
            for(int j = 0; j < y; j++){
                if(grid[i][j] == 0)
                    std::cout << ".";
                else if(grid[i][j] == 1)
                    std::cout << "X";
                else if(grid[i][j] == 2)
                    std::cout << "O";
            }
            std::cout << std::endl;
        }
        std::cout << "A: " << ants << " D: " << doodlebugs << std::endl;
    }
}
