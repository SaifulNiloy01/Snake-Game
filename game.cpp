#include <windows.h>
#include <bits/stdc++.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <ctime>
#include "game.h"

using namespace std;

void unit(int,int); //unit of 1 square
int random(int,int);

bool length_inc=false; //snake body length increase
bool seedflag = false; // generate food position randomely
extern int score;
extern bool game_over;
bool food=false;
int rows=0,columns=0;
int direction = RIGHT; //when the game started at first the snake will go right
int foodx,foody; //food position
int posx[MAX+1]= {4,3,2,1,0,-1,-1}; // MAX = 50; block position; x-axis value; decrement because block position right to left of x-axis
int posy[MAX+1]= {10,10,10,10,10,10,10}; //block position of y-axix is same for all block when game started
int length=7;

void initGrid(int x,int y) { // the row and column of a grid will be 1 unit
    columns=x;
    rows=y;
}

// draw all grids
void draw_grid() {
    for(int i=0; i<columns; i++) {
        for(int j=0; j<rows; j++) {
            unit(i,j); // call unit function for drawing grids (danger & safe area)
        }
    }
}

// draw the snake (head + body)
void draw_snake() {
    for(int i=length-1; i>0; i--) { // length = 7; left to right of the snake body; first left block = 0 [index]
        // draw the whole snake; moving the snake
        posx[i]=posx[i-1]; // 0 goes to 1
        posy[i]=posy[i-1];
    }
    for(int i=0; i<length; i++) {
        glColor3f(0.0,0.0,0.0); //snake body color is black
        if(i==0) {
            glColor3f(1.0,0.0,0.0); // head color is red

            // Move the snake by keyboard key
            switch(direction) {
            case UP:
                posy[i]++;
                break;
            case DOWN:
                posy[i]--;
                break;
            case RIGHT:
                posx[i]++;
                break;
            case LEFT:
                posx[i]--;
                break;
            }

            //Game over condition
            if(posx[i]==0||posx[i]==columns-1||posy[i]==0||posy[i]==rows-1) // touch danger red area
                game_over=true; // game over
            else if(posx[i]==foodx && posy[i]==foody) { // eat food
                food=false; // food gone & food will be drawn in another position

                // score condition
                score++;
                cout << "Score: " << score << endl;
                if(length<=MAX) // MAX = 50; snake body increase if smaller than MAX
                    length_inc=true;
                if(length==MAX) // if lenght is equal to 50 then snake body not increase anymore but can continue the game
                    MessageBox(NULL,"You Win\nYou can still keep playing but the snake will not grow.","Awsome",0);
            }

            // if head of the snake touch the body then game over
            // if index of head is equal to any index of the body then game over
            for(int j=1; j<length; j++) {
                if(posx[j]==posx[0] && posy[j]==posy[0]) //pos[0] is head position
                    game_over=true;
            }
        }

        // draw snake by quads
        glBegin(GL_QUADS);
        glVertex2d(posx[i],posy[i]);
        glVertex2d(posx[i]+1,posy[i]);
        glVertex2d(posx[i]+1,posy[i]+1);
        glVertex2d(posx[i],posy[i]+1);
        glEnd();
    }

    //increament the snake length
    if(length_inc) { // if length_inc is true then increase the length
        length++;
        length_inc=false;
    }
}

// Draw food
void draw_food() {
    if(!food) { // if food is true
        foodx=random(2,columns-2); // except the danger red area; call random function
        foody=random(2,rows-2);
        food=true; // stop draw food
    }
    for(;;) { // food color will change randomely untill snake eat the food
        double red = rand() % 2; // value will be 0 or 1
        double green = rand() % 2;
        double blue = rand() % 2;
        glColor3f(red,green,blue); // random color
        break; // break for draw the food now
    }
    // draw the food using quads
    glBegin(GL_QUADS);
    glVertex2d(foodx,foody);
    glVertex2d(foodx+1,foody);
    glVertex2d(foodx+1,foody+1);
    glVertex2d(foodx,foody+1);
    glEnd();
}

// make grids
void unit(int x,int y) {
    glLoadIdentity();
    if(x==0||x==columns-1||y==0||y==rows-1) { // border (danger area)
        glLineWidth(4.0);
        glColor3f(1.0,0.0,0.0); // red
    } else {
        glColor3f(0.0,1.0,0.0); //safe area; green
        glLineWidth(1.0);
    }
    glBegin(GL_LINES);
    glVertex2d(x,y);
    glVertex2d(x+1,y);
    glVertex2d(x+1,y);
    glVertex2d(x+1,y+1);
    glVertex2d(x+1,y+1);
    glVertex2d(x,y+1);
    glVertex2d(x,y+1);
    glVertex2d(x,y);
    glEnd();
}

// generate food position randomely; two random technique used
int random(int _min,int _max) {
    if(!seedflag) { // if seedflag == true
        srand(time(NULL));
        seedflag=false; // food position is generated so stop seedflag
    } else
        seedflag=true;
    return _min+rand()%(_max-_min); // random position generate for food
}
