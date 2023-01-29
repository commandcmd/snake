#include "../libraries/customTerminalBitMap.hpp"
#include "../libraries/BackgroundTimer.hpp"
#include <cstdlib> //used for random numbers later

//Defining the settings for the commands that gets input from the user used in the code:
//
//                    disabling waiting for user input
//                    |||||
//                    |||||  disabling echo
//                    |||||  |||||
//                    |||||  |||||  enabling terminal's raw mode
//                    |||||  |||||  ||||  
//                    |||||  |||||  ||||  enabling special characters (KEY_UP, KEY_PGDOWN, ENTER, TAB, ...)
//                    |||||  |||||  ||||  ||||  
//                    |||||  |||||  ||||  ||||  disabling signal interrupt (CTRL+C, CTRL+Z) [enabled now for testing purposes]
//                    |||||  |||||  ||||  ||||  |||||
#define getchSettings false, false, true, true, true

#define DIRECTION_UP    101 //
#define DIRECTION_DOWN  102 //Constants used to be saved into the "direction" variable
#define DIRECTION_LEFT  103 //They tell the code which direction the snake was requested to go into in a read-friendly way
#define DIRECTION_RIGHT 104 //

#define COORD_X 0 // These constants are used to point
#define COORD_Y 1 // to a certain type of the buffer's coordinate

using namespace terminal; //We're using the terminal's namespace of "customTerminalBitMap.hpp" to avoid having to write "terminal::" in front of every line that prints stuff on the terminal

//Namespace that contains all the functions which draw menus and stuff on the terminal
namespace draw {
    void startMenu(int selection){ //Function to draw the initial menu of the game:

        //We have two types of the menu, depending by the screen's resolution
        if(bm::length <= 62 || bm::height <= 18){//If the screen's resolution is less (or equal) than 62x18 characters:
            bm::fillScreen(' '); //Clear the screen

            bm::drawLine(0, 0, 0, bm::height - 1, '|', GREEN);                           //Draw some "|" characters at
            bm::drawLine(bm::length - 1, 0, bm::length - 1, bm::height - 1, '|', GREEN); //the sides of the window

            bm::drawLine(0, 0, bm::length - 1, 0, '=', GREEN);                           //Then draw some "=" chars at
            bm::drawLine(0, bm::height - 1, bm::length - 1, bm::height - 1, '=', GREEN); //the top and the bottom

            bm::printText(bm::length / 2 - 4, bm::height / 4, YELLOW, BLACK, "S N A K E");                                                         //
            bm::printText(bm::length / 2 - 4, bm::height / 4 + 3, (selection == 1 ? BLACK : WHITE), (selection == 1 ? WHITE : BLACK), "Play");     //Print all the words on screen centered and at a fraction of the height of the terminal
            bm::printText(bm::length / 2 - 4, bm::height / 4 + 4, (selection == 2 ? BLACK : WHITE), (selection == 2 ? WHITE : BLACK), "Settings"); //Then use the "selection" variable to determine wether to draw them inverted or not
            bm::printText(bm::length / 2 - 4, bm::height / 4 + 5, (selection == 3 ? BLACK : WHITE), (selection == 3 ? WHITE : BLACK), "About");    //

            if(selection > 0){
                bm::drawLine(bm::length / 2 - 5, bm::height / 4 + 3, bm::length / 2 - 5, bm::height / 4 + 5, ' '); //if the selection has been changed draw a space over all the previous parenthesys to clear them
                bm::drawLine(bm::length / 2 + 5, bm::height / 4 + 3, bm::length / 2 + 5, bm::height / 4 + 5, ' '); //

                bm::drawPixel(bm::length / 2 - 5, bm::height / 4 + 2 + selection, '[', CYAN); //And after clearing all the previous parenthesys draw the new ones over the selection
                bm::drawPixel(bm::length / 2 + 5, bm::height / 4 + 2 + selection, ']', CYAN); //
            }
        } else { //But if the resolution is bigger than the treshold:
            bm::fillScreen(' '); //Clear the screen

            bm::drawRect(bm::length / 2 - 30, bm::height / 2 - 8, bm::length / 2 + 30, bm::height / 2 + 8, '#', GREEN);//Draw the square with all '#'s at some coordinates which correspond to a fraction of the terminal height and width

            bm::printText(bm::length / 2 - 4, bm::height / 2 - 6, YELLOW, BLACK, "S N A K E");                                                     //
            bm::printText(bm::length / 2 - 4, bm::height / 2 - 1, (selection == 1 ? BLACK : WHITE), (selection == 1 ? WHITE : BLACK), "Play");     //Draw all the text at a certain distance from the center of the terminal
            bm::printText(bm::length / 2 - 4, bm::height / 2 + 0, (selection == 2 ? BLACK : WHITE), (selection == 2 ? WHITE : BLACK), "Settings"); //and highlight it depending on the selection
            bm::printText(bm::length / 2 - 4, bm::height / 2 + 1, (selection == 3 ? BLACK : WHITE), (selection == 3 ? WHITE : BLACK), "About");    //

            if(selection > 0){
                bm::drawLine(bm::length / 2 - 5, bm::height / 2 - 1, bm::length / 2 - 5, bm::height / 2 + 1, ' '); //if the selection has been changed draw a space over all the previous parenthesys to clear them
                bm::drawLine(bm::length / 2 + 5, bm::height / 2 - 1, bm::length / 2 + 5, bm::height / 2 + 1, ' '); //

                bm::drawPixel(bm::length / 2 - 5, bm::height / 2 - 2 + selection, '[', CYAN); //And after clearing all the previous parenthesys draw the new ones over the selection
                bm::drawPixel(bm::length / 2 + 5, bm::height / 2 - 2 + selection, ']', CYAN); //
            }
        }

        bm::updateDisplay(); //Write all the changes on the terminal
    }

    void aboutScr(){ //Function to draw the about screen of the game:
        //Again, this menu is different depending by the size of the terminal:
        if(bm::length <= 62 || bm::height <= 18){//If the terminal's size is less (or equal) than 62x18 characters:
            bm::fillScreen(' '); //Clear the screen
            
            bm::drawLine(0, 0, 0, bm::height - 1, '|', GREEN);                           //Draw '|'s at the sides of the window
            bm::drawLine(bm::length - 1, 0, bm::length - 1, bm::height - 1, '|', GREEN); //

            bm::drawLine(0, 0, bm::length - 1, 0, '=', GREEN);                           //Then draw '='s at the top and bottom of the window
            bm::drawLine(0, bm::height - 1, bm::length - 1, bm::height - 1, '=', GREEN); //

            bm::printText(bm::length / 2 - 9, 2, BRIGHT_YELLOW, BLACK, "SNAKE GAME - About");                    //
            bm::printText(bm::length / 2 - 13, bm::height / 2 + 1, YELLOW, BLACK, "c'mon aint this game cool?"); //And then just write all the cool text
            bm::printText(bm::length / 2 - 13, bm::height - 2, GRAY, BLACK, "Press any key to go back..");       //
        } else { //If the terminal's size is greater than the treshold then:
            bm::fillScreen(' '); //Clear the screen

            bm::drawRect(bm::length / 2 - 30, bm::height / 2 - 8, bm::length / 2 + 30, bm::height / 2 + 8, '#', GREEN); //Draw a rectangle with '#'s which size is a fraction of the terminal one

            bm::printText(bm::length / 2 - 9, bm::height / 2 - 7, BRIGHT_YELLOW, BLACK, "SNAKE GAME - About");   //
            bm::printText(bm::length / 2 - 13, bm::height / 2 + 1, YELLOW, BLACK, "c'mon aint this game cool?"); //Write all the cool text
            bm::printText(bm::length / 2 - 13, bm::height / 2 + 7, GRAY, BLACK, "Press any key to go back..");   //
        }

        bm::updateDisplay(); //Write all the changes to the terminal
    }

    void gameScr(){ //Simple function to write the game window:
        bm::fillScreen(' ');//Clear the screen

        bm::drawRect(0, 0, bm::length - 1, bm::height - 4, '#', GREEN); //Draw all around the screen a rectangle with '#'s
                                                                        //Right now the height of the rectangle is 4 characters less than the actual terminal height because i had to leave a bit of space for the debug window

        bm::updateDisplay();//Fetch the changes to the display
    }
}

//The following namespace contains the entire code for the game to work
namespace game {
    struct { //This struct contains all the constant settings for the game to work | these will soon be replaced by a setting menu
        const char snakeCharacter = '@'; //The character used to print the snake on the terminal

        const unsigned int updateTime = 1000; //Update time of the game in ms
        const unsigned int maxTiles = 4; //Maximum number of tiles on-screen (not still implemented)
        const unsigned int tileSizeIncrease = 1; //How much size does one tile increase to the snake
        const unsigned int startingLength = 4; //How long is the snake when the game starts

        const bool dieFromWalls = false; //Can the snake die from touching the walls?
        const bool randomWalls = false; //Do we wanna make the snake window a maze?
    } settings; //Call this struct "settings"

    unsigned int *buffer; //buffer for the entire window, it saves all the pixels of the snake's body
    unsigned int bufferArrayPosition; //Saves the array's X position we're currently at in the buffer

    unsigned int snakeLength; //Saves the current snake's length
    unsigned int direction; //Stores the current direction of the snake

    unsigned int *pointerOutput(unsigned int *pointer, int x = -1, int y = -1){ //Used to point to a certain address in the bidimensional array of the buffer.
        const int lengthY = 2;
        
        return (++pointer + x * lengthY + y);
    }

    void start(){
        //Error i'm trying to fix : when snake's starting length is greater than 2 the program starts having issues and when's greater than 3 the program segfaults.

        Timer gameTimer;// Creates and sets the game's timer used for timing
        gameTimer.set();//

        draw::gameScr(); //Calls the function to draw the game screen

        buffer = new unsigned int[bm::length * bm::height * 2]; //Assigns to the buffer's pointer a new unsigned int array
        bufferArrayPosition = 1; //Starting from the X position of 1
        snakeLength = settings.startingLength; //The initial snake's length is the initial starting length of the snake

        *(pointerOutput(buffer, bufferArrayPosition, COORD_X)) = bm::length / 2; //Setting the first position of the snake to half of the screen's resolution (which is gonna be a point centered at the middle of the screen)
        *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)) = bm::height / 2; //

        scr::clear(); //Clearing the terminal
        bm::drawPixel(*(pointerOutput(buffer, bufferArrayPosition, COORD_X)), *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)), settings.snakeCharacter); //Printing on the terminal the first variable in the buffer array (which we just set that to be a point in the middle of the screen)

        bm::printText(bm::length / 2 - 12, bm::height - bm::height / 3, GRAY, BLACK, "Press any key to start.."); //Print some text that tells the user to print any key on his keyboard to start running the game

        bm::updateDisplay(); //Fetching the changes just made to the terminal

        char lastInput = '\0';
        while(lastInput != KEY_UP && lastInput != KEY_DOWN && lastInput != KEY_LEFT && lastInput != KEY_RIGHT){
            lastInput = in::get_ch(getchSettings);
        }

        bm::drawLine(bm::length / 2 - 12, bm::height - bm::height / 3, bm::length / 2 + 12, bm::height - bm::height / 3, ' ');
        bm::updateDisplay();

        for(char input = '\0';true;input = in::get_ch(getchSettings)){
            if(input == '\0')input = lastInput;
            else lastInput = input;

            if(gameTimer.timePassed() <= game::settings.updateTime)continue;
            gameTimer.set();

            switch(input){
                case KEY_UP   : direction = DIRECTION_UP;break;
                case KEY_DOWN : direction = DIRECTION_DOWN;break;
                case KEY_LEFT : direction = DIRECTION_LEFT;break;
                case KEY_RIGHT: direction = DIRECTION_RIGHT;break;

                default : continue;
            }

            bufferArrayPosition++;

            switch(direction){
                case DIRECTION_UP : {
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_X)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_X));
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_Y)) - 1;
                    break;
                }

                case DIRECTION_DOWN : {
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_X)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_X));
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_Y)) + 1;
                    break;
                }

                case DIRECTION_LEFT : {
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_X)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_X)) - 1;
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_Y));
                    break;
                }

                case DIRECTION_RIGHT : {
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_X)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_X)) + 1;
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)) = *(pointerOutput(buffer, bufferArrayPosition - 1, COORD_Y));
                    break;
                }
            }
            
            for(unsigned int i = 0;i <= snakeLength && bufferArrayPosition - snakeLength > 0;i++){
                if(*(pointerOutput(buffer, i, COORD_X)) == *(pointerOutput(buffer, bufferArrayPosition - snakeLength + i, COORD_X)) && *(pointerOutput(buffer, i, COORD_Y)) == *(pointerOutput(buffer, bufferArrayPosition - snakeLength + i, COORD_Y)))continue;

                *(pointerOutput(buffer, i, COORD_X)) = *(pointerOutput(buffer, bufferArrayPosition - snakeLength + i, COORD_X));
                *(pointerOutput(buffer, i, COORD_Y)) = *(pointerOutput(buffer, bufferArrayPosition - snakeLength + i, COORD_Y));
            }
            bufferArrayPosition = snakeLength;

            scr::clear();
            bm::fillRect(1, 1, bm::length - 2, bm::height - 2, ' ', BLACK, BLACK);
            for(unsigned int i = 0;i <= bufferArrayPosition;i++){
                if(!bufferArrayPosition){
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_X)) = bm::length / 2;
                    *(pointerOutput(buffer, bufferArrayPosition, COORD_Y)) = bm::height / 2;
                }

                bm::printText(1, bm::height - 1, YELLOW, BLACK, "bufferArrayPosition = ", bufferArrayPosition);
                bm::printText(1, bm::height - 2, YELLOW, BLACK, "                                            buffer[", i, "][Y] = ", *(pointerOutput(buffer, i, COORD_Y)));
                bm::printText(1, bm::height - 3, YELLOW, BLACK, "drawing pixel at coords taken from buffer - buffer[", i, "][X] = ", *(pointerOutput(buffer, i, COORD_X)));
                bm::printText(bm::length - 32, bm::height - 3, GRAY, BLACK, "press any key to continue debug");

                bm::drawPixel(*(pointerOutput(buffer, i, COORD_X)), *(pointerOutput(buffer, i, COORD_Y)), settings.snakeCharacter, RED);
                bm::updateDisplay();

                in::get_ch();

                bm::drawPixel(*(pointerOutput(buffer, i, COORD_X)), *(pointerOutput(buffer, i, COORD_Y)), settings.snakeCharacter);
                bm::updateDisplay();
            }
            bm::updateDisplay();
        }

        delete [] buffer;

        return;
    }

    void setup(){
        return;
    }

    void about(){
        draw::aboutScr();
        in::get_ch(/*Default get_ch settings*/);

        return;
    }
}

int main(void){
    bm::init();

    bool breakLoop = false;
    int selection = 1;
    draw::startMenu(selection);

    char input;

    for(;;){
        for(input = 0;input == 0;input = in::get_ch(getchSettings));

        switch(input){
            case 'w':
            case 'W':
            case KEY_UP   : if(selection > 1)draw::startMenu(--selection);break;

            case 's':
            case 'S':
            case KEY_DOWN : if(selection < 3)draw::startMenu(++selection);break;

            case ENTER    : if(selection == 1)game::start(); else if(selection == 2)game::setup(); else if(selection == 3)game::about();draw::startMenu(selection);break;

            case CTRL_KEY('c') : breakLoop = true;break;
        }

        if(breakLoop)break;
    }

    bm::end();

    return 0;
}