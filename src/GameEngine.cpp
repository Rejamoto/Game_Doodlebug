#include "../include/GameEngine.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include "../include/MoveRandom.h"

namespace
{
    std::mt19937 generate(std::chrono::system_clock::now().time_since_epoch().count());

    static std::vector<Ant> ant_Population;
    static std::vector<Doodlebug> doodlebug_Population;
    static TheGrid::GameBoard gameboard;

    // Check if any sides are Empty and/or if Doodlebug move over Ant
    bool checkIfEmptySides(const int& type, const Point& point){
        int xTemp(point.getX()), yTemp(point.getY());

        if( type == 1 ){      // Check if Doodlebug can move over empty space or Ant
            if( (xTemp < gameboard.getGrid_X() && xTemp >= 0) && (yTemp < gameboard.getGrid_Y() && yTemp >= 0) ){
                if( (xTemp + 1) < gameboard.getGrid_X() && (gameboard(xTemp + 1, yTemp) == 0 || gameboard(xTemp + 1, yTemp) == 2) )        // Check if right side is empty or Ant
                    return true;
                else if( (xTemp - 1) >= 0 && (gameboard(xTemp - 1, yTemp) == 0 || gameboard(xTemp - 1, yTemp) == 2) )  // Check if left side is empty or Ant
                    return true;
                else if( (yTemp + 1) < gameboard.getGrid_Y() && (gameboard(xTemp, yTemp + 1) == 0 || gameboard(xTemp, yTemp + 1) == 2) )   // Check if bottom is empty or Ant
                    return true;
                else if( (yTemp - 1) >= 0 && (gameboard(xTemp, yTemp - 1) == 0 || gameboard(xTemp, yTemp - 1) == 2) )  // Check if top is empty or Ant
                    return true;
                else
                    return false;       // False if no sides are empty
            }
        }
        else{
            if( (xTemp < gameboard.getGrid_X() && xTemp >= 0) && (yTemp < gameboard.getGrid_Y() && yTemp >= 0) ){
                if( (xTemp + 1) < gameboard.getGrid_X() && gameboard(xTemp + 1, yTemp) == 0 )        // Check if right side is empty
                    return true;
                else if( (xTemp - 1) >= 0 && gameboard(xTemp - 1, yTemp) == 0 )  // Check if left side is empty
                    return true;
                else if( (yTemp + 1) < gameboard.getGrid_Y() && gameboard(xTemp, yTemp + 1) == 0 )   // Check if bottom is empty
                    return true;
                else if( (yTemp - 1) >= 0 && gameboard(xTemp, yTemp - 1) == 0 )  // Check if top is empty
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

        if( (xTemp < gameboard.getGrid_X() && xTemp >= 0) && (yTemp < gameboard.getGrid_Y() && yTemp >= 0) ){
            if( type == 1 ){      // Check if Doodlebug can move over empty space or Ant
                int marks = 0;
                bool checks[4] = { false };

                if((xTemp + 1) < gameboard.getGrid_X() && gameboard(xTemp + 1, yTemp) == 2){        // Check if right side is Ant
                    marks++;
                    checks[0] = true;
                }
                if((xTemp - 1) >= 0 && gameboard(xTemp - 1, yTemp) == 2){  // Check if left side is Ant
                    marks++;
                    checks[1] = true;
                }
                if((yTemp + 1) < gameboard.getGrid_Y() && gameboard(xTemp, yTemp + 1) == 2 ){   // Check if bottom is Ant
                    marks++;
                    checks[2] = true;
                }
                if( (yTemp - 1) >= 0 && gameboard(xTemp, yTemp - 1) == 2 ){  // Check if top is Ant
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

            if( side == 0 && (xTemp + 1) < gameboard.getGrid_X() && gameboard(xTemp + 1, yTemp) == 0 )        // Check if right side is empty
                return Point((xTemp + 1), yTemp); // Return with X + 1 Point
            else if( side == 1 && (xTemp - 1) >= 0 && gameboard(xTemp - 1, yTemp) == 0 )  // Check if left side is empty
                return Point((xTemp - 1), yTemp); // Return with X - 1 Point
            else if( side == 2 && (yTemp + 1) < gameboard.getGrid_Y() && gameboard(xTemp, yTemp + 1) == 0 )   // Check if bottom is empty
                return Point(xTemp, (yTemp + 1)); // Return with Y + 1 Point
            else if( side == 3 && (yTemp - 1) >= 0 && gameboard(xTemp, yTemp - 1) == 0 )  // Check if top is empty
                return Point(xTemp, (yTemp - 1)); // Return with Y - 1 Point
            else
                return point;       // Old point if no sides are empty
        }
        return point;
    }
    // End of getOneSidePoint function



    // Check if movement can be done
    bool checkMovementIsValid(const int& type, const Point& point){
        if(type == gameboard(point.getX(), point.getY()))        // Cannot move if same organism is occupying the space
            return false;
        if(type == 2 && gameboard(point.getX(), point.getY()) == 1)      // Ant cannot move over Doodlebug
            return false;
        if(type == 1 && gameboard(point.getX(), point.getY()) == 2)      // Doodlebug can move over Ant
            return true;
        return true;        // Organism can move over empty block
    }
    // End of checkMovementIfValid function



    // Spawn Organism into empty blocks
    void spawnInOrganisms(const int& amountDoodlebug, const int& amountAnts){
        for(int i = 0; i < amountDoodlebug; i++){       // Generate Doodlebugs in empty blocks
            int xTemp, yTemp;

            do{                                     // Loops until an empty block is found
                xTemp = (generate() % gameboard.getGrid_X());
                yTemp = (generate() % gameboard.getGrid_Y());
            }while(gameboard.getGridBlock(xTemp, yTemp) != 0);

            gameboard(xTemp, yTemp, 1);
            doodlebug_Population.push_back(Doodlebug(Point(xTemp, yTemp)));
        }

        for(int i = 0; i < amountAnts; i++){        // Generate Ants in empty blocks
            int xTemp, yTemp;

            do{                                     // Loops until an empty block is found
                xTemp = (generate() % gameboard.getGrid_X());
                yTemp = (generate() % gameboard.getGrid_Y());
            }while(gameboard.getGridBlock(xTemp, yTemp) != 0);

            gameboard(xTemp, yTemp, 2);
            ant_Population.push_back(Ant(Point(xTemp, yTemp)));
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

            Point tempPoint = getOneSidePoint(breedType, point, randomNum);     // Gets the new point for the Organism to spawn in

            if(checkMovementIsValid(breedType, tempPoint)){     // If location is valid, spawn Organism to new location
                if(breedType == 1 && gameboard(tempPoint.getX(), tempPoint.getY()) == 2)     // Remove Ant if Doodlebug spawns on same location
                    removeFromList(2, tempPoint);

                if(breedType == 1)      // Adds new Doodlebug/Ant to list
                    doodlebug_Population.push_back(Doodlebug(tempPoint));
                else if(breedType == 2)
                    ant_Population.push_back(Ant(tempPoint));

                gameboard(tempPoint.getX(), tempPoint.getY(), breedType);       // Set Organism on Grid
                break;
            }
            else
                checks[randomNum] = true;       // Flag number if location is invalid
        }
    }
    // End of isBreeding function



    // Remove Organism by location
    void removeFromList(const int& removeThisType, const Point& point){
        if(removeThisType == 1){
            for(unsigned int i = 0; i < doodlebug_Population.size(); i++)       // Find and remove Doodlebug by location
                if((doodlebug_Population[i]).getDoodleLocation() == point){
                    doodlebug_Population.erase(doodlebug_Population.begin() + i);
                    gameboard(point.getX(), point.getY(), 0);
                }
        }
        else if(removeThisType == 2){
            for(unsigned int i = 0; i < ant_Population.size(); i++)         // Find and remove Ant by location
                if(ant_Population[i].getAntLocation() == point)
                    ant_Population.erase(ant_Population.begin() + i);
        }
    }
    // End of removeFromList function



    // Resets breeding time from Vector lists
    void resetBreedingTime(const int& type, const Point& point){
        if(type == 1){
            for(unsigned int i = 0; i < doodlebug_Population.size(); i++)       // Find and remove Doodlebug by location
                if(doodlebug_Population[i].getDoodleLocation() == point)
                    doodlebug_Population[i].isBreeding();
        }
        else if(type == 2){
            for(unsigned int i = 0; i < ant_Population.size(); i++)         // Find and remove Ant by location
                if(ant_Population[i].getAntLocation() == point)
                    ant_Population[i].isBreeding();
        }
    }
    // End of resetBreedingTime function
}

namespace DoodlebugGame
{
    GameEngine::GameEngine() : GameEngine(10) { }
    GameEngine::GameEngine(int gridSize) { gameboard.createNewGrid(gridSize); }

    void GameEngine::runGame() {
        int amount = gameboard.getGrid_X() * gameboard.getGrid_Y();
        int amountDoodle(0), totalTurns(0);
        amountDoodle = ((amount < 75) ? 4 : (amount * 0.1));
        spawnInOrganisms(amountDoodle, amount * 0.4);

        gameboard.drawGrid(totalTurns, ant_Population.size(), doodlebug_Population.size());
        Sleep(1000);

        do{
            std::vector<Doodlebug>::iterator tempIt;


/*             // Movement behavior
 *             for(tempIt = doodlebug_Population.begin(); tempIt != doodlebug_Population.end(); ++tempIt) {
 *                  tempIt->setDoodleMovement(new MoveRandom());
 *                  tempIt->moving();
 *             }
 *             // Ends Movement behavior
 */



            // Doodlebug movement
            //for(unsigned int i = 0; i < doodlebug_Population.size(); i++){      // Loops through all Doodlebugs
            for(tempIt = doodlebug_Population.begin(); tempIt != doodlebug_Population.end(); ++tempIt) {
                bool checks[4] = { false };
                Point oldPoint(tempIt->getDoodleLocation());

                tempIt->setDoodleMovement(new MoveRandom());
                tempIt->moving();

                while(checkIfEmptySides(1, oldPoint)){      // Check if possible to move at all
                    int randomNum(generate() % 4);

                    while(checks[randomNum] && (checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false))      // Check if randomNum is not flagged
                        randomNum = generate() % 4;

                    Point tempPoint = getOneSidePoint(1, oldPoint, randomNum);      // Gets one of four sides Point

                    if(!(oldPoint == tempPoint)){       // If old point is not same as new point
                        if(checkMovementIsValid(1, tempPoint)){     // Check if new point is valid
                            tempIt->moving(tempPoint);      // Set new location

                            if(gameboard(tempPoint.getX(), tempPoint.getY()) == 2){      // Check if Doodlebug is eating an Ant
                                tempIt->eatAnt();
                                removeFromList(2, tempPoint);
                            }
                            else
                                tempIt->noEat();

                            gameboard(oldPoint.getX(), oldPoint.getY(), 0);
                            gameboard(tempPoint.getX(), tempPoint.getY(), 1);
                            break;
                        }
                    }

                    checks[randomNum] = true;
                }
                Sleep(50);
            }
            // Ends Doodlebug movement




            std::vector<Doodlebug> tempDoodleList = doodlebug_Population;

            // Doodlebug dies
            for(unsigned int i = 0; i < tempDoodleList.size(); i++)
                if(!tempDoodleList[i].isAlive())
                    removeFromList(doodlebug_Population[i].getDoodleType(), doodlebug_Population[i].getDoodleLocation());
            // Ends Doodlebug deaths

            // Doodlebug Breeds
            if(!(doodlebug_Population.size() <= 0)){
                tempDoodleList = doodlebug_Population;
                for(tempIt = tempDoodleList.begin(); tempIt != tempDoodleList.end(); ++tempIt)
                    if(tempIt->isBreeding()){
                        resetBreedingTime(1, tempIt->getDoodleLocation());
                        if(checkIfEmptySides(1, tempIt->getDoodleLocation()))
                            isBreeding(1, tempIt->getDoodleLocation());
                    }
            }
            // Ends Breed



            gameboard.drawGrid(totalTurns, ant_Population.size(), doodlebug_Population.size());
            Sleep(500);



            std::vector<Ant>::iterator tempAIt;

/*             for(tempAIt = ant_Population.begin(); tempAIt != ant_Population.end(); ++tempAIt) {
 *                 tempAIt->setAntMovement(new MoveRandom());
 *                 tempAIt->moving();
 *             }
 */



            // Ant movement
            //for(unsigned int i = 0; i < ant_Population.size(); i++){
            for(tempAIt = ant_Population.begin(); tempAIt != ant_Population.end(); ++tempAIt) {
                bool checks[4] = { false };
                Point oldPoint(tempAIt->getAntLocation());

                tempAIt->setAntMovement(new MoveRandom());
                tempAIt->moving();

                while(checkIfEmptySides(2, oldPoint)){
                    int randomNum(generate() % 4);

                    while(checks[randomNum] && (checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false))
                        randomNum = generate() % 4;

                    Point tempPoint = getOneSidePoint(2, oldPoint, randomNum);

                    if(!(oldPoint == tempPoint)){
                        if(checkMovementIsValid(2, tempPoint)){
                            tempAIt->moving(tempPoint);

                            gameboard(oldPoint.getX(), oldPoint.getY(), 0);
                            gameboard(tempPoint.getX(), tempPoint.getY(), 2);
                            break;
                        }
                    }

                    checks[randomNum] = true;
                }
                Sleep(50);
            }



            // Ant Breeds
            if(!(ant_Population.size() <= 0)){
                std::vector<Ant> tempAntList = ant_Population;
                for(tempAIt = tempAntList.begin(); tempAIt != tempAntList.end(); ++tempAIt)
                    if(tempAIt->isBreeding()){
                        resetBreedingTime(2, tempAIt->getAntLocation());
                        if(checkIfEmptySides(2, tempAIt->getAntLocation()))
                            isBreeding(2, tempAIt->getAntLocation());
                    }
            }



            gameboard.drawGrid(totalTurns, ant_Population.size(), doodlebug_Population.size());
            Sleep(500);
            totalTurns++;
        }while(ant_Population.size() > 0 && doodlebug_Population.size() > 0);

        std::cout << ((ant_Population.size() <= 0) ? "Doodlebugs won!" : "Ants won!") << std::endl;
        totalTurns = 0;
    }
}
