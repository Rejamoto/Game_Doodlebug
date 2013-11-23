#include "../include/Grid.h"

namespace
{
    int x, y, totalTurns;
    int** grid;

    std::mt19937 generate(std::chrono::system_clock::now().time_since_epoch().count());     // Random number generator

    // Check if any sides are Empty and/or if Doodlebug move over Ant
    bool checkIfEmptySides(const int& type, const Point& point){
        int xTemp(point.getX()), yTemp(point.getY());

        if( type == 1 ){      // Check if Doodlebug can move over empty space or Ant
            if( (xTemp < x && xTemp >= 0) && (yTemp < y && yTemp >= 0) ){
                if( (xTemp + 1) < x && (grid[xTemp + 1][yTemp] == 0 || grid[xTemp + 1][yTemp] == 2) )        // Check if right side is empty or Ant
                    return true;
                else if( (xTemp - 1) >= 0 && (grid[xTemp - 1][yTemp] == 0 || grid[xTemp - 1][yTemp] == 2) )  // Check if left side is empty or Ant
                    return true;
                else if( (yTemp + 1) < y && (grid[xTemp][yTemp + 1] == 0 || grid[xTemp][yTemp + 1] == 2) )   // Check if bottom is empty or Ant
                    return true;
                else if( (yTemp - 1) >= 0 && (grid[xTemp][yTemp - 1] == 0 || grid[xTemp][yTemp - 1] == 2) )  // Check if top is empty or Ant
                    return true;
                else
                    return false;       // False if no sides are empty
            }
        }
        else{
            if( (xTemp < x && xTemp >= 0) && (yTemp < y && yTemp >= 0) ){
                if( (xTemp + 1) < x && grid[xTemp + 1][yTemp] == 0 )        // Check if right side is empty
                    return true;
                else if( (xTemp - 1) >= 0 && grid[xTemp - 1][yTemp] == 0 )  // Check if left side is empty
                    return true;
                else if( (yTemp + 1) < y && grid[xTemp][yTemp + 1] == 0 )   // Check if bottom is empty
                    return true;
                else if( (yTemp - 1) >= 0 && grid[xTemp][yTemp - 1] == 0 )  // Check if top is empty
                    return true;
                else
                    return false;       // False if no sides are empty
            }
        }
        return false;       // False if point is out of range
    }
    // End of checkIfEmptySides function



    // Create new point of one of four sides at current location
    Point getOneSidePoint(const int& type, const Point& point, const int& side){
        int xTemp(point.getX()), yTemp(point.getY());

        if( (xTemp < x && xTemp >= 0) && (yTemp < y && yTemp >= 0) ){
            if( type == 1 ){      // Check if Doodlebug can move over empty space or Ant
                int marks = 0;
                bool checks[4] = { false };

                if((xTemp + 1) < x && grid[xTemp + 1][yTemp] == 2){        // Check if right side is Ant
                    marks++;
                    checks[0] = true;
                }
                if((xTemp - 1) >= 0 && grid[xTemp - 1][yTemp] == 2){  // Check if left side is Ant
                    marks++;
                    checks[1] = true;
                }
                if((yTemp + 1) < y && grid[xTemp][yTemp + 1] == 2 ){   // Check if bottom is Ant
                    marks++;
                    checks[2] = true;
                }
                if( (yTemp - 1) >= 0 && grid[xTemp][yTemp - 1] == 2 ){  // Check if top is Ant
                    marks++;
                    checks[3] = true;
                }

                if(marks > 0){
                    int randomNum;
                    while(checks[0] == true || checks[1] == true || checks[2] == true || checks[3] == true){
                        randomNum = generate() % 4;

                        if(randomNum == 0 && checks[randomNum])
                            return Point((xTemp + 1), yTemp); // Return with X + 1 Point
                        else if(randomNum == 1 && checks[randomNum])  // Check if left side is Ant
                            return Point((xTemp - 1), yTemp); // Return with X - 1 Point
                        else if(randomNum == 2 && checks[randomNum])   // Check if bottom is Ant
                            return Point(xTemp, (yTemp + 1)); // Return with Y + 1 Point
                        else if(randomNum == 3 && checks[randomNum])  // Check if top is Ant
                            return Point(xTemp, (yTemp - 1)); // Return with Y - 1 Point
                    }
                }
            }

            if( side == 0 && (xTemp + 1) < x && grid[xTemp + 1][yTemp] == 0 )        // Check if right side is empty
                return Point((xTemp + 1), yTemp); // Return with X + 1 Point
            else if( side == 1 && (xTemp - 1) >= 0 && grid[xTemp - 1][yTemp] == 0 )  // Check if left side is empty
                return Point((xTemp - 1), yTemp); // Return with X - 1 Point
            else if( side == 2 && (yTemp + 1) < y && grid[xTemp][yTemp + 1] == 0 )   // Check if bottom is empty
                return Point(xTemp, (yTemp + 1)); // Return with Y + 1 Point
            else if( side == 3 && (yTemp - 1) >= 0 && grid[xTemp][yTemp - 1] == 0 )  // Check if top is empty
                return Point(xTemp, (yTemp - 1)); // Return with Y - 1 Point
            else
                return point;       // Old point if no sides are empty
        }
        return point;
    }
    // End of getOneSidePoint function



    // Check if movement can be done
    bool checkMovementIsValid(const int& type, const Point& point){
        if(type == grid[point.getX()][point.getY()])        // Cannot move if same organism is occupying the space
            return false;
        if(type == 2 && grid[point.getX()][point.getY()] == 1)      // Ant cannot move over Doodlebug
            return false;
        if(type == 1 && grid[point.getX()][point.getY()] == 2)      // Doodlebug can move over Ant
            return true;
        return true;        // Organism can move over empty block
    }
    // End of checkMovementIfValid function



    // Resets breeding time from Vector lists
    void resetBreedingTime(const int& type, const Point& point){
        if(type == 1){
            for(unsigned int i = 0; i < listOfDoodlebugs.size(); i++)       // Find and remove Doodlebug by location
                if(listOfDoodlebugs[i].getDoodleLocation() == point)
                    listOfDoodlebugs[i].isBreeding();
        }
        else if(type == 2){
            for(unsigned int i = 0; i < listOfAnts.size(); i++)         // Find and remove Ant by location
                if(listOfAnts[i].getAntLocation() == point)
                    listOfAnts[i].isBreeding();
        }
    }
    // End of resetBreedingTime function



    // Remove Organism by location
    void removeFromList(const int& removeThisType, const Point& point){
        if(removeThisType == 1){
            for(unsigned int i = 0; i < listOfDoodlebugs.size(); i++)       // Find and remove Doodlebug by location
                if((listOfDoodlebugs[i]).getDoodleLocation() == point){
                    listOfDoodlebugs.erase(listOfDoodlebugs.begin() + i);
                    grid[point.getX()][point.getY()] = 0;
                }
        }
        else if(removeThisType == 2){
            for(unsigned int i = 0; i < listOfAnts.size(); i++)         // Find and remove Ant by location
                if(listOfAnts[i].getAntLocation() == point)
                    listOfAnts.erase(listOfAnts.begin() + i);
        }
    }
    // End of removeFromList function



    // Spawn Organism into empty blocks
    void spawnInOrganisms(const int& amountDoodlebug, const int& amountAnts){
        for(int i = 0; i < amountDoodlebug; i++){       // Generate Doodlebugs in empty blocks
            int xTemp = (generate() % x);
            int yTemp = (generate() % y);

            while(grid[xTemp][yTemp] != 0){         // Loops until an empty block is found
                xTemp = (generate() % x);
                yTemp = (generate() % y);
            }

            grid[xTemp][yTemp] = 1;
            listOfDoodlebugs.push_back(Doodlebug(new Point(xTemp, yTemp)));
        }

        for(int i = 0; i < amountAnts; i++){        // Generate Ants in empty blocks
            int xTemp = (generate() % x);
            int yTemp = (generate() % y);

            while(grid[xTemp][yTemp] != 0){         // Loops until an empty block is found
                xTemp = (generate() % x);
                yTemp = (generate() % y);
            }

            grid[xTemp][yTemp] = 2;
            listOfAnts.push_back(Ant(new Point(xTemp, yTemp)));
        }
    }
    // End of spawnInOrganisms function



    // Breed new Organisms
    void isBreeding(const int& breedType, const Point& point){
        int randomNum;
        bool checks[4] = { false };

        while(checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false){        // Loops till it breeds
            randomNum = generate() % 4;

            while(checks[randomNum] && (checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false))      // Loops till it find Number that was not selected before
                randomNum = generate() % 4;

            Point* tempPoint = new Point(getOneSidePoint(breedType, point, randomNum));     // Gets the new point for the Organism to spawn in

            if(checkMovementIsValid(breedType, *tempPoint)){     // If location is valid, spawn Organism to new location
                if(breedType == 1 && grid[tempPoint->getX()][tempPoint->getY()] == 2)     // Remove Ant if Doodlebug spawns on same location
                    removeFromList(2, *tempPoint);

                if(breedType == 1)      // Adds new Doodlebug/Ant to list
                    listOfDoodlebugs.push_back(Doodlebug(tempPoint));
                else if(breedType == 2)
                    listOfAnts.push_back(Ant(tempPoint));

                grid[tempPoint->getX()][tempPoint->getY()] = breedType;       // Set Organism on Grid
                break;
            }
            else
                checks[randomNum] = true;       // Flag number if location is invalid
        }
    }
    // End of isBreeding function



    // Draws Grid on console
    void drawGrid(){
        system("cls");
        std::cout << "Turn " << totalTurns << std::endl;
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
        std::cout << "A: " << listOfAnts.size() << " D: " << listOfDoodlebugs.size() << std::endl;
    }
    // End of drawGrid function
}

namespace TheGrid
{
    Grid::Grid() : Grid(5, 5) {}
    Grid::Grid(int gridSize) : Grid(gridSize, gridSize) {}
    Grid::Grid(int xGrid, int yGrid)
    {
        x = xGrid;
        y = yGrid;
        grid = new int*[x];
        for(int i = 0; i < x; i++)
            grid[i] = new int[y];
        for(int i = 0; i < x; i++)
            for(int j = 0; j < y; j++)
                grid[i][j] = 0;
    }
    Grid::~Grid() { delete[] grid; }

    void Grid::runGame()
    {
        // Scale amount of spawns by the size of grid
        int amount = x * y;
        int amountDoodle;
        amountDoodle = ((amount < 75) ? 4 : (amount * 0.1));
        spawnInOrganisms(amountDoodle, amount * 0.4);

        //drawGrid();

        do{
                //drawGrid();

            // *Doodlebug turn*

                // Doodlebug movement
                for(unsigned int i = 0; i < listOfDoodlebugs.size(); i++){      // Loops through all Doodlebugs
                    bool checks[4] = { false };
                    Point oldPoint(listOfDoodlebugs[i].getDoodleLocation());

                    while(checkIfEmptySides(1, oldPoint)){      // Check if possible to move at all
                        int randomNum(generate() % 4);

                        while(checks[randomNum] && (checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false))      // Check if randomNum is not flagged
                            randomNum = generate() % 4;

                        Point tempPoint = getOneSidePoint(1, oldPoint, randomNum);      // Gets one of four sides Point

                        if(!(oldPoint == tempPoint)){       // If old point is not same as new point
                            if(checkMovementIsValid(1, tempPoint)){     // Check if new point is valid
                                listOfDoodlebugs[i].moving(tempPoint);      // Set new location

                                if(grid[tempPoint.getX()][tempPoint.getY()] == 2){      // Check if Doodlebug is eating an Ant
                                    listOfDoodlebugs[i].eatAnt();
                                    removeFromList(2, tempPoint);
                                }
                                else
                                    listOfDoodlebugs[i].noEat();

                                grid[oldPoint.getX()][oldPoint.getY()] = 0;
                                grid[tempPoint.getX()][tempPoint.getY()] = 1;
                                break;
                            }
                        }

                        checks[randomNum] = true;
                    }
                }

                // Doodlebug dies
                std::vector<Doodlebug> tempDoodleList = listOfDoodlebugs;
                for(unsigned int i = 0; i < tempDoodleList.size(); i++)
                    if(!tempDoodleList[i].isAlive())
                        removeFromList(listOfDoodlebugs[i].getDoodleType(), listOfDoodlebugs[i].getDoodleLocation());

                // Doodlebug Breeds
                if(!(listOfDoodlebugs.size() <= 0)){
                    tempDoodleList = listOfDoodlebugs;
                    for(unsigned int i = 0; i < tempDoodleList.size(); i++)
                        if(tempDoodleList[i].isBreeding()){
                            resetBreedingTime(1, tempDoodleList[i].getDoodleLocation());
                            if(checkIfEmptySides(1, tempDoodleList[i].getDoodleLocation()))
                                isBreeding(1, tempDoodleList[i].getDoodleLocation());
                        }
                }

            // *Doodlebug turn ends*

            //drawGrid();
            //Sleep(500);   // uncomment

            // *Ant turn*

                // Ant movement
                for(unsigned int i = 0; i < listOfAnts.size(); i++){
                    bool checks[4] = { false };
                    Point oldPoint(listOfAnts[i].getAntLocation());


                    while(checkIfEmptySides(2, oldPoint)){
                        int randomNum(generate() % 4);

                        while(checks[randomNum] && (checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false))
                            randomNum = generate() % 4;

                        Point tempPoint = getOneSidePoint(2, oldPoint, randomNum);

                        if(!(oldPoint == tempPoint)){
                            if(checkMovementIsValid(2, tempPoint)){
                                listOfAnts[i].moving(tempPoint);

                                grid[oldPoint.getX()][oldPoint.getY()] = 0;
                                grid[tempPoint.getX()][tempPoint.getY()] = 2;
                                break;
                            }
                        }

                        checks[randomNum] = true;
                    }
                }

                // Ant Breeds
                if(!(listOfAnts.size() <= 0)){
                    std::vector<Ant> tempAntList = listOfAnts;
                    for(unsigned int i = 0; i < tempAntList.size(); i++)
                        if(tempAntList[i].isBreeding()){
                            resetBreedingTime(2, tempAntList[i].getAntLocation());
                            if(checkIfEmptySides(2, tempAntList[i].getAntLocation()))
                                isBreeding(2, tempAntList[i].getAntLocation());
                        }
                }


            // *Ant turn ends*

                // Total turns have passed
                totalTurns++;

                // Stop loop every second
                //drawGrid();
               // Sleep(500);   // uncomment

                if(listOfAnts.size() <= 0 || listOfDoodlebugs.size() <= 0){
                    drawGrid();
                    std::cout << ((listOfAnts.size() <= 0) ? "Doodlebugs won!" : "Ants won!") << std::endl;
                    totalTurns = 0;
                    break;
                }

        }while(true);
    }
}
