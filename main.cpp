#include <bits/stdc++.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include "game.h"

//Window size
#define ROWS 40.0
#define COLUMNS 40.0

std::ofstream ofile;
std::ifstream ifile;

using namespace std;

bool game_over=false; //at first game not over
extern int direction;
int score=0;

void init(); // initialize; clear color of the default window color // by default color is black
void display_callback(); //display score when game over
void input_callback(int,int,int); //which keyboard key is pressed
void reshape_callback(int,int); //change component size when maximize and minimize the window
void timer_callback(int);

int main(int argc,char**argv) {
    glutInit(&argc,argv); //initialize glut
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); //initialize glut display
    glutInitWindowPosition(400,100); //windows position when started
    glutInitWindowSize(650,650); // square window size
    glutCreateWindow("SNAKE GAME"); //create the window and display the text on top
    glutDisplayFunc(display_callback); //display score when game over
    glutReshapeFunc(reshape_callback); //help window minimize and maximize
    glutSpecialFunc(input_callback); //find which key is pressed
    glutTimerFunc(100,timer_callback,0);
    init();
    glutMainLoop();
    return 0;
}

void init() {
    glClearColor(1.0,1.0,1.0,0.0); //last parameter is the alpha value //set window color //now white (1,1,1,0)
    initGrid(COLUMNS,ROWS);
}

//Callbacks
void display_callback() {
    if(game_over) { //if game over
        ofile.open("score.dat",std::ios::trunc);
        ofile<<score<<std::endl;
        ofile.close();
        ifile.open("score.dat",std::ios::in);
        char a[4];
        ifile>>a;
        std::cout<<a;
        char text[50]= "Your score : ";
        strcat(text,a);
        MessageBox(NULL,text,"Game Over",0);
        exit(0);
    }
    glClear(GL_COLOR_BUFFER_BIT); //clear color buffer
    glLoadIdentity();
    draw_grid();
    draw_food();
    draw_snake();
    glutSwapBuffers();
}
void reshape_callback(int w, int h) { //w = width, h = height // windows width and height after resize
    glViewport(0,0,(GLfloat)w,GLfloat(h)); //0,0 is the left bottom corner
    glMatrixMode(GL_PROJECTION); //set projection inside the window
    glLoadIdentity(); //Load the projection now
    //-1.0, 1.0 is z-axis
    glOrtho(0.0,COLUMNS,0.0,ROWS,-1.0,1.0); //orthographic projection //Parameter: first->left most point of window, second->right most point of window, third->column is 40 (width)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //load all of this
}
void timer_callback(int) {
    glutPostRedisplay();
    glutTimerFunc(100,timer_callback,0); //recursion; after 100 milisecond returns; doesn't wait;
}

//which keyboard key is pressed
void input_callback(int key,int x,int y) { //x & y is mouse position
    switch(key) {
    // UP
    case GLUT_KEY_UP:
        if(direction!=DOWN)
            direction=UP;
        break;

    // Down
    case GLUT_KEY_DOWN:
        if(direction!=UP)
            direction=DOWN;
        break;

    //Right
    case GLUT_KEY_RIGHT:
        if(direction!=LEFT)
            direction=RIGHT;
        break;

    //Left
    case GLUT_KEY_LEFT:
        if(direction!=RIGHT)
            direction=LEFT;
        break;
    }
}
