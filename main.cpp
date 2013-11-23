#include <windows.h>
#include <gl/gl.h>

#include <vector>
#include <algorithm>
#include <chrono>

#include <pthread.h>
#include <mutex>

#include "include/GameInfo.h"
#include "include/GameEngine.h"
#include "include/MoveRandom.h"
#include "include/SeekAnt.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
void runGame();
void nextTurn(struct gameLoop *g);

void *looping(void *arg);

struct threadKey{
    pthread_t gameThread;
    pthread_mutex_t lockGame;
} GameKey;

struct gameLoop{
    struct threadKey tk;
};

namespace GameFunctions
{
    std::mt19937 generate(std::chrono::system_clock::now().time_since_epoch().count());

    static std::vector<Ant> ant_Population;
    static std::vector<Doodlebug> doodlebug_Population;
    static TheGrid::GameBoard gameboard;
    int totalTurns(0);

    // Check if any sides are Empty and/or if Doodlebug move over Ant
    bool checkIfEmptySides(const int& type, const Point& point){
        int xTemp(point.getX()), yTemp(point.getY());

        if( (xTemp < gameboard.getGrid_X() && xTemp >= 0) && (yTemp < gameboard.getGrid_Y() && yTemp >= 0) ){
            if( (xTemp + 1) < gameboard.getGrid_X() && (gameboard(xTemp + 1, yTemp) == 0 || gameboard(xTemp + 1, yTemp) == ((type == 1) ? 2 : 0)) )         // Check if right side is empty or Ant
                return true;
            else if( (xTemp - 1) >= 0 && (gameboard(xTemp - 1, yTemp) == 0 || gameboard(xTemp - 1, yTemp) == ((type == 1) ? 2 : 0)) )                       // Check if left side is empty or Ant
                return true;
            else if( (yTemp + 1) < gameboard.getGrid_Y() && (gameboard(xTemp, yTemp + 1) == 0 || gameboard(xTemp, yTemp + 1) == ((type == 1) ? 2 : 0)) )    // Check if bottom is empty or Ant
                return true;
            else if( (yTemp - 1) >= 0 && (gameboard(xTemp, yTemp - 1) == 0 || gameboard(xTemp, yTemp - 1) == ((type == 1) ? 2 : 0)) )                       // Check if top is empty or Ant
                return true;
            else
                return false;       // False if no sides are empty
        }
        return false;
    }
    // End of checkIfEmptySides function



    // Create new point of one of four sides at current location
    Point getOneSidePoint(const int& type, const Point& point, const int& side){
        int xTemp(point.getX()), yTemp(point.getY());

        if( (xTemp < gameboard.getGrid_X() && xTemp >= 0) && (yTemp < gameboard.getGrid_Y() && yTemp >= 0) ){
            if( type == 1 ){                                                                        // Check if Doodlebug can move over empty space or Ant
                int marks = 0;
                bool checks[4] = { false };

                if((xTemp + 1) < gameboard.getGrid_X() && gameboard(xTemp + 1, yTemp) == 2){        // Check if right side is Ant
                    marks++;
                    checks[0] = true;
                }
                if((xTemp - 1) >= 0 && gameboard(xTemp - 1, yTemp) == 2){                           // Check if left side is Ant
                    marks++;
                    checks[1] = true;
                }
                if((yTemp + 1) < gameboard.getGrid_Y() && gameboard(xTemp, yTemp + 1) == 2 ){       // Check if bottom is Ant
                    marks++;
                    checks[2] = true;
                }
                if( (yTemp - 1) >= 0 && gameboard(xTemp, yTemp - 1) == 2 ){                         // Check if top is Ant
                    marks++;
                    checks[3] = true;
                }

                if(marks > 0){
                    int randomNum;
                    while(checks[0] == true || checks[1] == true || checks[2] == true || checks[3] == true){
                        randomNum = generate() % 4;

                        if(randomNum == 0 && checks[randomNum])
                            return Point((xTemp + 1), yTemp);           // Return with X + 1 Point
                        else if(randomNum == 1 && checks[randomNum])    // Check if left side is Ant
                            return Point((xTemp - 1), yTemp);           // Return with X - 1 Point
                        else if(randomNum == 2 && checks[randomNum])    // Check if bottom is Ant
                            return Point(xTemp, (yTemp + 1));           // Return with Y + 1 Point
                        else if(randomNum == 3 && checks[randomNum])    // Check if top is Ant
                            return Point(xTemp, (yTemp - 1));           // Return with Y - 1 Point
                    }
                }
            }

            if( side == 0 && (xTemp + 1) < gameboard.getGrid_X() && gameboard(xTemp + 1, yTemp) == 0 )          // Check if right side is empty
                return Point((xTemp + 1), yTemp);                                                               // Return with X + 1 Point
            else if( side == 1 && (xTemp - 1) >= 0 && gameboard(xTemp - 1, yTemp) == 0 )                        // Check if left side is empty
                return Point((xTemp - 1), yTemp);                                                               // Return with X - 1 Point
            else if( side == 2 && (yTemp + 1) < gameboard.getGrid_Y() && gameboard(xTemp, yTemp + 1) == 0 )     // Check if bottom is empty
                return Point(xTemp, (yTemp + 1));                                                               // Return with Y + 1 Point
            else if( side == 3 && (yTemp - 1) >= 0 && gameboard(xTemp, yTemp - 1) == 0 )                        // Check if top is empty
                return Point(xTemp, (yTemp - 1));                                                               // Return with Y - 1 Point
            else
                return point;                                                                                   // Old point if no sides are empty
        }
        return point;
    }
    // End of getOneSidePoint function



    // Check if movement can be done
    bool checkMovementIsValid(const int& type, const Point& point){
        if(type == gameboard(point.getX(), point.getY()))                   // Cannot move if same organism is occupying the space
            return false;
        if(type == 2 && gameboard(point.getX(), point.getY()) == 1)         // Ant cannot move over Doodlebug
            return false;
        if(type == 1 && gameboard(point.getX(), point.getY()) == 2)         // Doodlebug can move over Ant
            return true;
        return true;                                                        // Organism can move over empty block
    }
    // End of checkMovementIfValid function



    // Spawn Organism into empty blocks
    void spawnInOrganisms(const int& amountDoodlebug, const int& amountAnts){
        for(int i = 0; i < amountDoodlebug; i++){                                                   // Generate Doodlebugs in empty blocks
            int xTemp, yTemp;

            do{                                                                                     // Loops until an empty block is found
                xTemp = (generate() % gameboard.getGrid_X());
                yTemp = (generate() % gameboard.getGrid_Y());
            }while(gameboard.getGridBlock(xTemp, yTemp) != 0);

            gameboard(xTemp, yTemp, 1);

            doodlebug_Population.push_back(Doodlebug(new Point(xTemp, yTemp), new SeekAnt()));
        }

        for(int i = 0; i < amountAnts; i++){                                                        // Generate Ants in empty blocks
            int xTemp, yTemp;

            do{                                                                                     // Loops until an empty block is found
                xTemp = (generate() % gameboard.getGrid_X());
                yTemp = (generate() % gameboard.getGrid_Y());
            }while(gameboard.getGridBlock(xTemp, yTemp) != 0);

            gameboard(xTemp, yTemp, 2);
            ant_Population.push_back(Ant(new Point(xTemp, yTemp), new MoveRandom()));
        }
    }
    // End of spawnInOrganisms function



    // Breed new Organisms
    void isBreeding(const int& breedType, const Point& point){
        int randomNum;
        bool checks[4] = { false };

        while(checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false){                                // Loops till it breeds
            randomNum = generate() % 4;

            while(checks[randomNum] && (checks[0] == false || checks[1] == false || checks[2] == false || checks[3] == false))      // Loops till it find Number that was not selected before
                randomNum = generate() % 4;

            Point tempPoint = getOneSidePoint(breedType, point, randomNum);                                                         // Gets the new point for the Organism to spawn in

            if(checkMovementIsValid(breedType, tempPoint)){                                                                         // If location is valid, spawn Organism to new location
                if(breedType == 1 && gameboard(tempPoint.getX(), tempPoint.getY()) == 2)                                            // Remove Ant if Doodlebug spawns on same location
                    GameFunctions::removeFromList(2, tempPoint);

                if(breedType == 1)                                                                                                  // Adds new Doodlebug/Ant to list
                    doodlebug_Population.push_back(Doodlebug(new Point(tempPoint), new SeekAnt()));
                else if(breedType == 2)
                    ant_Population.push_back(Ant(new Point(tempPoint), new MoveRandom()));

                gameboard(tempPoint.getX(), tempPoint.getY(), breedType);                                                           // Set Organism on Grid
                break;
            }
            else
                checks[randomNum] = true;                                                                                           // Flag number if location is invalid
        }
    }
    // End of isBreeding function



    // Remove Organism by location
    void removeFromList(const int& removeThisType, const Point& point){
        if(removeThisType == 1){
            for(unsigned int i = 0; i < doodlebug_Population.size(); i++)           // Find and remove Doodlebug by location
                if((doodlebug_Population[i]).getDoodleLocation() == point){
                    doodlebug_Population.erase(doodlebug_Population.begin() + i);
                    gameboard(point.getX(), point.getY(), 0);
                }
        }
        else if(removeThisType == 2){
            for(unsigned int i = 0; i < ant_Population.size(); i++)                 // Find and remove Ant by location
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
            for(unsigned int i = 0; i < ant_Population.size(); i++)             // Find and remove Ant by location
                if(ant_Population[i].getAntLocation() == point)
                    ant_Population[i].isBreeding();
        }
    }
    // End of resetBreedingTime function


}


using namespace GameFunctions;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "Doodlebug Game";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "Doodlebug Game",
                          "Doodlebug Game",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          256,
                          256,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    gameboard.createNewGrid(GAMEBOARD_SIZE);

    runGame();

    struct gameLoop gL;
    gL.tk = GameKey;

    int check = pthread_create(&gL.tk.gameThread, NULL, looping, &gL);

    //if(!check) pthread_join(gL.tk.gameThread, NULL);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else if(doodlebug_Population.size() != 0 && ant_Population.size() != 0)
        {
            /* OpenGL animation code goes here */

            int** tempGrid = gameboard.getGridArray();

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glPushMatrix();

            glBegin(GL_QUADS);

            for(int y = 0; y < gameboard.getGrid_Y(); y++) {
                for(int x = 0; x < gameboard.getGrid_X(); x++) {
                    switch(tempGrid[x][y]) {
                    case(0):
                        glColor3f(0.1f, 0.1f, 0.1f);
                        break;
                    case(1):
                        glColor3f(0.75f, 0.0f, 0.0f);
                        break;
                    case(2):
                        glColor3f(0.0f, 0.75f, 0.0f);
                        break;
                    }


                    glVertex2f(-0.975f + (x/10.0), 0.975f - (y/10.0));
                    glVertex2f(-0.925f + (x/10.0), 0.975f - (y/10.0));
                    glVertex2f(-0.925f + (x/10.0), 0.925f - (y/10.0));
                    glVertex2f(-0.975f + (x/10.0), 0.925f - (y/10.0));
                }
            }

            glEnd();

            glPopMatrix();

            SwapBuffers(hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}






void runGame() {
    int amount = gameboard.getGrid_X() * gameboard.getGrid_Y();
    int amountDoodle(0);
    amountDoodle = ((amount < 75) ? 4 : (amount * 0.1));
    spawnInOrganisms(amountDoodle, amount * 0.4);
}

void nextTurn(struct gameLoop *gameL) {
    std::vector<Doodlebug>::iterator tempIt;

    pthread_mutex_lock(&gameL->tk.lockGame);

    // Movement behavior
    for(tempIt = doodlebug_Population.begin(); tempIt != doodlebug_Population.end(); ++tempIt) {
        tempIt->movingDoodlebug(gameboard, tempIt->getLocation());

        Point* tempPoint = tempIt->getLocation();
        if(gameboard(tempPoint) == 2){
            tempIt->eatAnt();
            removeFromList(2, Point(tempPoint->getX(), tempPoint->getY()));
        }
        else
            tempIt->noEat();
        gameboard(tempPoint->getX(), tempPoint->getY(), 1);
    }
    // Ends Movement behavior


    // Doodlebug dies
    std::vector<Doodlebug> tempDoodleList = doodlebug_Population;

    for(unsigned int i = 0; i < tempDoodleList.size(); i++) {
        if(!tempDoodleList[i].isAlive())
            removeFromList(doodlebug_Population[i].getDoodleType(), doodlebug_Population[i].getDoodleLocation());
    }
    // Ends Doodlebug deaths


    // Doodlebug Breeds
    if(!(doodlebug_Population.size() <= 0)){
        tempDoodleList = doodlebug_Population;
        for(tempIt = tempDoodleList.begin(); tempIt != tempDoodleList.end(); ++tempIt)
            if(tempIt->isBreeding()){
                //resetBreedingTime(1, tempIt->getDoodleLocation());
                if(checkIfEmptySides(1, tempIt->getDoodleLocation()))
                        isBreeding(1, tempIt->getDoodleLocation());
            }
    }
    // Ends Breed

    std::vector<Ant>::iterator tempAIt;

    // Ant movement
    for(tempAIt = ant_Population.begin(); tempAIt != ant_Population.end(); ++tempAIt) {
        tempAIt->movingAnt(gameboard, tempAIt->getLocation());
    }
    // Ends Movement behavior


    // Ant Breeds
    if(!(ant_Population.size() <= 0)){
        std::vector<Ant> tempAntList = ant_Population;
        for(tempAIt = tempAntList.begin(); tempAIt != tempAntList.end(); ++tempAIt)
            if(tempAIt->isBreeding()){
                //resetBreedingTime(2, tempAIt->getAntLocation());
                if(checkIfEmptySides(2, tempAIt->getAntLocation()))
                    isBreeding(2, tempAIt->getAntLocation());
            }
    }
    // Ends Breed

    //totalTurns++;

    pthread_mutex_unlock(&gameL->tk.lockGame);
}

void *looping(void *arg){
    struct gameLoop *gameL = (gameLoop *)arg;

    while(true){
        if(doodlebug_Population.size() == 0 && ant_Population.size() == 0) break;

        nextTurn(gameL);

        Sleep(GAME_SPEED);
    }

    return 0;
}
