#include <iostream>
#include <curses.h>
#include "TerminalScreen.hpp"
#include "BackgroundTimer.hpp"

int X = 100;
int Y = 50;

int input;

class mainMenu {
    public:
        void draw();
        void animation();
        void select_up();
        void select_down();
        void error_animation();
    private:
        int selection = 0;
        Timer animationt1;
        Timer animationt2;
};

terminal window;
Timer mainMenuTimer;
mainMenu menu1;

int main(void){
    window.init(X, Y);
    mainMenuTimer.set();
    menu1.draw();


    for(;;){
        input = getch();

        switch((char)input){
            case 'w' : menu1.select_up();window.updateDisplay();break;
            case 's' : menu1.select_down();window.updateDisplay();break;
            case 'a' : menu1.error_animation();window.updateDisplay();break;
            case 'd' : menu1.error_animation();window.updateDisplay();break;

            default : window.updateDisplay();break;
        }
    }

    endwin();

    return 0;
}

void mainMenu::draw(){
    window.drawRect(0, 0, X, Y, '+', white, green);

    window.printText(X / 2 - 9, Y / 4, "S N A K E  G A M E", green);
    window.printText(X / 2 - 2, Y / 2 + 1, "Play");
    window.printText(X / 2 - 4, Y / 2 + 2, "Settings");
    window.printText(X / 2 - 2, Y / 2 + 3, "Help");
    window.printText(X / 2 - 13, Y / 2 + Y / 4 + Y / 8, "Move in the menu with WASD", green, white);

    window.updateDisplay();
}

void mainMenu::select_down(){
    switch(selection){
        case 0 : window.printText(X / 2 - 2, Y / 2 + 1, "Play", black, white);selection = 1;break;
        case 1 : window.printText(X / 2 - 2, Y / 2 + 1, "Play", white, black); window.printText(X / 2 - 4, Y / 2 + 2, "Settings", black, white);selection = 2;break;
        case 2 : window.printText(X / 2 - 4, Y / 2 + 2, "Settings", white, black); window.printText(X / 2 - 2, Y / 2 + 3, "Help", black, white);selection = 3;break;
        case 3 : window.printText(X / 2 - 2, Y / 2 + 3, "Help", white, black);selection = 4;break;
    }

    return;
}

void mainMenu::select_up(){
    switch (selection) {
        case 1 : window.printText(X / 2 - 2, Y / 2 + 1, "Play", white, black);selection = 0;break;
        case 2 : window.printText(X / 2 - 4, Y / 2 + 2, "Settings", white, black); window.printText(X / 2 - 2, Y / 2 + 1, "Play", black, white);selection = 1;break;
        case 3 : window.printText(X / 2 - 2, Y / 2 + 3, "Help", white, black); window.printText(X / 2 - 4, Y / 2 + 2, "Settings", black, white);selection = 2;break;
        case 4 : window.printText(X / 2 - 2, Y / 2 + 3, "Help", black, white);selection = 3;break;
    }

    return;
}

void mainMenu::error_animation(){
    for(int i = 0;i <= 4;i++){
        window.drawRect(X / 2 - 6 - i, Y / 2 - i / 2, X / 2 + 6 + i, Y / 2 + 4 + i / 2, '=', white, red);
        window.updateDisplay();
        animationt2.set();
        while(animationt2.timePassed() <= 50);
        window.drawRect(X / 2 - 6 - i, Y / 2 - i / 2, X / 2 + 6 + i, Y / 2 + 4 + i / 2, ' ');
    }

    return;
}