/*
 *  Setareh Lotfi
 *  Katie Fotion
 *  Hayden Clevenger
 *  4/3/2016
 *
 *  This assignment is the culmination of everything we learned this semester
 *
 *  Usage:      The usage will be outlined in detail during the presentation of the project
 *              at turn in. However, one can walk around the scene using W, A, S, D, can jump using 
 *              the return key. Can mount the bike when within a proper distance using the space bar,
 *              can throw a ball using the mouse click and can look around using the mouse.
 *
 *  Overview:   The purpose of this assignment is to demonstrate knowledge in areas like texturing, 
 *              lighting, robot movement, parametric curves (ball throwing) in 3D, and other useful 
 *              OpenGL concepts.
 *
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Lighting.h"
#include "Robot.h"

#define PI 3.1415
#define NUM_BALLS 100

float phi=0.0, theta=3*PI/2.0, step = 0.25;   //for moving camera

//mouse coordinates for looking around
float oldX, oldY;

// Variables for throwing the balls
float rotation[NUM_BALLS];
float prev_xz[NUM_BALLS];
float y_start[NUM_BALLS], v0[NUM_BALLS], degree[NUM_BALLS];
int bounce_count[NUM_BALLS];
//ball posiiton
float ballPos[NUM_BALLS][3];
//ball movmement counter (see idle)
int ball_timer[NUM_BALLS];
int ball_counter = 0;   //keep track of which ball is thrown

float m=0.0;    //movement of clouds

float w = 800, h = 600; // Width and Height of the window

//for gluLookAt function
float eye[3] = {1.5f, 1.0f, 14.0f}; //eye vector
float at[3] = {0.0f, 1.0f, -1.0f};  //at vector

//double eyeIdealPosition[3] = {-10, 10, -10};
double eyePosition[3];
double eyeFocus[3];

float walkSpeed = 1.0f; // Speed of walking (gets faster on bike)
float roty=0.0;         // for rotating bike
float transx=0.0, transz=0.0;   // for translating the bike

bool bike = false, extremes = false, hit[NUM_BALLS], jump=false, down=false;
bool rotateLeft = false;
bool rotateRight = false;
const int BufferSize = 10;

GLuint BufferName[BufferSize];

unsigned int texture;
char filename[200];

enum
{
    LEAF_VERTEX = 0,
    LEAF_TEX = 1,
    CYLINDER_FULL_VERTEX = 2,
    CYLINDER_FULL_TEX = 3,
    CYLINDER_END_VERTEX = 4,
    CYLINDER_END_TEX = 5,
    SQUARE_VERTEX = 6,
    
};

GLfloat vertic[][3]={{-1.5,0.0,1.5},{-1.5,0.0,-1.5},{1.5,0.0,-1.5},{1.5,0.0,1.5},   //ground  3
    {0.5,0.0,-0.3},{0.5,0.0,-1.4},{1.4,0.0,-1.4},{1.4,0.0,-0.3},   //house 1 7
    {0.5,0.0,-0.3},{0.5,0.0,-1.4},{0.5,1.0,-1.4},{0.5,1.0,-0.3}, //house 2 11
    {0.5,0.0,-0.3},{0.5,1.0,-0.3},{1.4,1.0,-0.3},{1.4,0.0,-0.3},   //house 3 15
    {1.4,0.0,-1.4},{1.4,1.0,-1.4},{1.4,1.0,-0.3},{1.4,0.0,-0.3}, //house 4 19
    {0.5,0.0,-1.4},{0.5,1.0,-1.4},{1.4,1.0,-1.4},{1.4,0.0,-1.4},  //house 5  23
    {0.5,1.0,-1.4},{0.5,1.0,-0.3},{0.5,1.75,-0.85}, //house left roof     26
    {1.4,1.0,-0.3},{0.5,1.0,-0.3},{0.5,1.75,-0.85},{1.4,1.75,-0.85},  //house front roof  30
    {1.4,1.0,-0.3},{1.4,1.0,-1.4},{1.4,1.75,-0.85}, //house right roof                 33
    {1.4,1.0,-1.4},{0.5,1.0,-1.4},{0.5,1.75,-0.85},{1.4,1.75,-0.85},  //house back roof   37
    {1.1,0.0,-0.275},{0.8,0.0,-0.275},{0.8,0.7,-0.275},{1.1,0.7,-0.275},  //house front door 41
    {0.75,0.35,-0.275},{0.55,0.35,-0.275},{0.55,0.7,-0.275},{0.75,0.7,-0.275},  //house front left window 45
    {1.35,0.35,-0.275},{1.15,0.35,-0.275},{1.15,0.7,-0.275},{1.35,0.7,-0.275},  //house front right window 49
    {1.5,0.025,1.5},{-1.6,0.025,1.5},{-1.6,0.025,0.75},{1.5,0.025,0.75},  //road 53
    {-0.3,0.04,1.2},{-0.8,0.04,1.2},{-0.8,0.04,1.05},{-0.3,0.04,1.05},  //road strip 57
    {0.7,0.04,1.2},{0.2,0.04,1.2},{0.2,0.04,1.05},{0.7,0.04,1.05},  //road strip 61
    {1.5,0.04,1.2},{1.2,0.04,1.2},{1.2,0.04,1.05},{1.5,0.04,1.05},  //road strip 65
    {-1.3,0.04,1.2},{-1.6,0.04,1.2},{-1.6,0.04,1.05},{-1.3,0.04,1.05},  //road strip 69
    {-0.3,0.03,0.0},{-1.2,0.03,0.0},{-1.2,0.03,-1.05},{-0.3,0.03,-1.05},  //road strip 73
    {-0.3,0.0,-0.1},{-0.3,0.5,-0.1},{-0.4,0.25,-0.1},{-0.4,0.0,-0.1},  //pool carrier 77
    {-0.3,0.0,-0.3},{-0.3,0.5,-0.3},{-0.4,0.25,-0.3},{-0.4,0.0,-0.3},  //pool carrier 81
};

GLfloat colors[][3]={{1.0,1.0,1.0},{0.0,0.0,0.0},{0.89019,0.38823,0.03921},{0.57254,0.44705,0.01176},{0.88235,0.0,0.03921},{0.00784,0.54509,0.0},
    {0.4588,0.6784,0.2509},{0.88235,0.67843,0.2},{0.35294117,0.3568627,0.3647059},{0.0,0.0,1.0}}; //0.white,1.black,2.orange,3.yellowish,4.red,5.green,6.car,7.orange,8.grey

// Read texture image file
unsigned int LoadTex(char *s)
{
    unsigned int Texture;
    FILE* img;
    img = fopen(s,"rb");
    unsigned long bWidth = 0;
    unsigned long bHeight = 0;
    unsigned long size = 0;
    
    fseek(img,18,SEEK_SET);
    fread(&bWidth,4,1,img);
    fread(&bHeight,4,1,img);
    fseek(img,0,SEEK_END);
    size = ftell(img) - 54;
    unsigned char *data = (unsigned char*)malloc(size);
    fseek(img,54,SEEK_SET);
    fread(data,size,1,img);
    fclose(img);
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bWidth, bHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    if (data)
        free(data);
    return Texture;
}

/* Initate the matrix mode and viewPort for the window */
void init(void)
{
    glMatrixMode(GL_PROJECTION);
    
    // Reset Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set the correct perspective.
    gluPerspective(40, w/h, 0.1, 100.0);
    
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
    
    // Allow for texturing
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    texture = LoadTex(filename);
    
    // Set mouse to center
    glutWarpPointer(w/2, h/2);
    oldX = w/2;
    oldY = h/2;
}

/* Create the scene particals
 * Trees, house, clouds, ground, and sun
 */
/* Creates 4 different types of trees */
static void tree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 double trunk_h, double trunk_r,
                 double leaves_h, double leaves_r,
                 int type)
{
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,1,0,0);
    glScaled(dx,dy,dz);
    
    double d = (1.0/32) * (2*M_PI);
    
    // Draw Trunk
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.8f,0.4f,0.2f);
    for (int i = 0; i <= 32; i++) {
        // Generate a pair of points, on top and bottom of the strip.
        glNormal3d( cos(d*i), 0, sin(d*i));  // Normal for BOTH points.
        glVertex3d( trunk_r*cos(d*i), trunk_h, trunk_r*sin(d*i));  // Top point.
        glVertex3d( trunk_r*cos(d*i), 0, trunk_r*sin(d*i));  // Bottom point.
    }
    glEnd();
    
    // Draw Leaves (Side)
    glBegin(GL_QUAD_STRIP);
    glColor3f(0,1.0,0);
    for (int i = 0; i <= 32; i++) {
        glNormal3d( cos(d*i), leaves_h, sin(d*i));
        // Type 2: Upside narrow
        if (type == 2)
            glVertex3d( leaves_r*0.5f*cos(d*i), trunk_h+leaves_h,
                       leaves_r*0.5f*sin(d*i));
        // Type 4: Cone style
        else if (type == 4)
            glVertex3d( 0, trunk_h+leaves_h, 0);
        else
            glVertex3d( leaves_r*cos(d*i), trunk_h+leaves_h,
                       leaves_r*sin(d*i));
        
        if (type == 3)
            glVertex3d( leaves_r*0.5f*cos(d*i), trunk_h, leaves_r*0.5f*sin(d*i));
        else
            glVertex3d( leaves_r*cos(d*i), trunk_h, leaves_r*sin(d*i));
    }
    glEnd();
    
    // Type 4, Cone, does not need to draw top of leaves
    if (type != 4) {
        // Draw Leaves (Top)
        glBegin(GL_POLYGON);
        glColor3f(0,1.0,0);
        for (int i = 0; i <= 32; i++) {
            // Type 2: Upside narrow
            if (type == 2)
                glVertex3d( leaves_r*0.5f*cos(d*i), trunk_h+leaves_h,
                           leaves_r*0.5f*sin(d*i));
            else
                glVertex3d( leaves_r*cos(d*i), trunk_h+leaves_h,
                           leaves_r*sin(d*i));  // Top point.
        }
        glEnd();
    }
    
    // Draw Leaves (Bottom)
    glBegin(GL_POLYGON);
    glColor3f(0,0.1f,0);
    for (int i = 0; i <= 32; i++) {
        // Type 2: Upside narrow
        if (type == 3)
            glVertex3d( leaves_r*0.5f*cos(d*i), trunk_h,
                       leaves_r*0.5f*sin(d*i));
        else
            glVertex3d( leaves_r*cos(d*i), trunk_h, leaves_r*sin(d*i));
    }
    glEnd();
    
    //  Undo transofrmations
    glPopMatrix();
}

void sun(){
    glPushMatrix();
    glColor3f(1.0, 0.5, 0.0);
    glTranslatef(0.5,5.7,-8.5);
    glutSolidSphere(0.8,30,30);
    glPopMatrix();
}

// Texture the ground
void texGround()
{
    // Enable for texturing
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(-20.0+(i*10.0), 0.0, -20.0+(j*10.0));
            glTexCoord2f(1.0, 0.0); glVertex3f(-10.0+(i*10.0), 0.0, -20.0+(j*10.0));
            glTexCoord2f(1.0, 1.0); glVertex3f(-10.0+(i*10.0), 0.0, -10.0+(j*10.0));
            glTexCoord2f(0.0, 1.0); glVertex3f(-20.0+(i*10.0), 0.0, -10.0+(j*10.0));
            glEnd();
        }
    }
    
    // Disable texturing
    glDisable(GL_TEXTURE_2D);
}

void cloud(){
    
    //Cloud 1
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-1.3+m,4.5,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.7,20,20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-1.7+m,4.5,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.4,20,20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-0.9+m,4.5,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.4,20,20);
    glPopMatrix();
    glFlush();
    
    //cloud 2
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-2.3+m,3.5,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.7,20,20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-2.7+m,3.5,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.4,20,20);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(-1.9+m,3.5,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.4,20,20);
    glPopMatrix();
    glFlush();
}


void squars(int a,int b,int c,int d)
{
    glBegin(GL_POLYGON);
    glVertex3fv(vertic[a]);
    glVertex3fv(vertic[b]);
    glVertex3fv(vertic[c]);
    glVertex3fv(vertic[d]);
    glEnd();
}

void triangle(int a,int b,int c)
{
    glBegin(GL_POLYGON);
    glVertex3fv(vertic[a]);
    glVertex3fv(vertic[b]);
    glVertex3fv(vertic[c]);
    glEnd();
}

void house()
{
    glColor3fv(colors[2]);  //house base
    squars(4,5,6,7);
    glColor3fv(colors[2]);  //house left wall
    squars(8,9,10,11);
    glColor3fv(colors[3]); //house center wall
    squars(12,13,14,15);
    glColor3fv(colors[2]); //house right wall
    squars(16,17,18,19);
    glColor3fv(colors[3]); //house back wall
    squars(20,21,22,23);
    glColor3fv(colors[2]); //house left roof
    triangle(24,25,26);
    glColor3fv(colors[4]); //house front roof
    squars(27,28,29,30);
    glColor3fv(colors[2]); //house right roof
    triangle(31,32,33);
    glColor3fv(colors[4]); //house back roof
    squars(34,35,36,37);
    glColor3fv(colors[6]); //house front door
    squars(38,39,40,41);
    
    //house left window
    squars(42,43,44,45);
    glColor3fv(colors[6]); //house right window
    glNormal3fv(vertic[46]);
    glNormal3fv(vertic[47]);
    glNormal3fv(vertic[48]);
    glNormal3fv(vertic[49]);
    squars(46,47,48,49);
    
    
    glColor3fv(colors[0]); //house smoke outlet
    glPushMatrix();
    glTranslatef(0.7,1.375,-0.575);
    glScalef(0.25,1.0,0.25);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glColor3fv(colors[3]); //tree trunk 1
    glPushMatrix();
    glTranslatef(-1.0,0.35,-1.3);
    glScalef(0.2,1.5,0.2);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3fv(colors[5]);
    glTranslatef(-1.0,0.7,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.5,20,20);
    glPopMatrix();
    
    glColor3fv(colors[3]); //tree trunk 2
    glPushMatrix();
    glTranslatef(-0.4,0.35,-1.3);
    glScalef(0.2,1.5,0.2);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3fv(colors[5]);
    glTranslatef(-0.4,0.7,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.5,20,20);
    glPopMatrix();
    
    glColor3fv(colors[3]); //tree trunk 3
    glPushMatrix();
    glTranslatef(0.2,0.35,-1.3);
    glScalef(0.2,1.5,0.2);
    glutSolidCube(0.5);
    glPopMatrix();
    
    glPushMatrix();
    glColor3fv(colors[5]);
    glTranslatef(0.2,0.7,-1.3);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(0.5,20,20);
    glPopMatrix();
}

void Scene(int light)
{
    Light(light);

}
void displayLight()
{
    //  Draw light position as sphere (still no lighting here)
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslated(Lpos[0],Lpos[1],Lpos[2]);
    glPopMatrix();
}

void switchAxisToXYZ()
{
    glRotated(90,0,1,0);
    glRotated(90,1,0,0);
    glScaled(0.5,0.5,0.5);
    glTranslatef(0.0, 0.0, -2.4*sin(phi));
}

void displayPlayerViewport()
{
    glPushMatrix();
    glLoadIdentity();
    switchAxisToXYZ();
    
    glTranslated(1,0,0);
    Scene(1);

    glPushMatrix();
    glScaled(3.0,3.0,3.0);
    glRotated(90, 0.0, 0.0, 1.0);
    glRotatef(10, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -1.5, 0.53);

    glRotatef(phi*90/PI, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -sin(phi), -sin(phi));

    if (rotateLeft) {
        glRotated(90, 0, 0, 1);
        drawRobot();
    }
    else if(rotateRight){
        glRotated(-90, 0, 0, 1);
        drawRobot();
    }
    else{
        drawRobot();
    }
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}


/* Idle function for animations */
void idle()
{
    //ball things
    for(int i = 0; i < NUM_BALLS; i++) {
        if(ball_timer[i] < 1000000) {
            
            if(ballPos[i][1] < 0.08 && !hit[i]) {
                ball_timer[i] = 0;
                y_start[i] = ballPos[i][1];
                
                v0[i] = 5.0*v0[i]/6.0;
                prev_xz[i] = 0.0;
                bounce_count[i]++;
                hit[i] = true;
            }
            if(ballPos[i][1] > 0.08 && hit[i])
                hit[i] = false;
                
            if(bounce_count[i] < 5) {
            
                float t = ball_timer[i]/100.0;
            
                //Math to throw the ball
                float dist_xz = v0[i]*t*cos(degree[i]);
                float xz_step = fabs(prev_xz[i] - dist_xz);
                prev_xz[i] = dist_xz;
            
                ballPos[i][0] -= xz_step*sin(rotation[i]);
                ballPos[i][2] -= xz_step*cos(rotation[i]);
            
                ballPos[i][1] = v0[i]*t*sin(degree[i])-4.9*t*t+y_start[i];
            }
            
            //increment time variable
            ball_timer[i]++;
        }
    }
    
    //cloud things
    if(m < 30) {
        m +=0.005;
    }
    else {
        m = -30;
    }
    
    //jump
    if(jump && eye[1]<3.0 && !down) {
        eye[1] += 0.3;
        at[1] += 0.3;
    }
    else if(jump && eye[1] > 3.0 && !down)
        down = true;
    else if(jump && down && eye[1]>1.0) {
        eye[1] -= 0.3;
        at[1] -= 0.3;
    }
    else {
        jump = false;
        down = false;
    }
    
    //redraw
    glutPostRedisplay();
    
}

void initSquare()
{
    float squareVertex[2*4] = {0,0,0,1,1,1,1,0};
    glGenBuffers(1, &BufferName[SQUARE_VERTEX]);
    glBindBuffer(GL_ARRAY_BUFFER, BufferName[SQUARE_VERTEX]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2*4, squareVertex, GL_STATIC_DRAW);
    // Reset
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initShapes()
{
    /* Allocate and assigns buffers*/
    initSquare();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6,0.6,1.0,1.0);
    
    // Reset transformations
    glLoadIdentity();
    
    // Set the camera
    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0);
    
    glColor3f(1.0, 1.0, 1.0);
    
    glPushMatrix();

    //Draw the scene
    texGround();
    cloud();
    sun();
    
    glPushMatrix();
    
    // If bike is activated, rotate and translate accordingly (don't draw person)
    if(bike) {
        roty = -theta;
        glTranslatef(eye[0]+9.0, 0.4, eye[2]-1.0);
    }
    // Otherwise just translate it to where it was left last and draw person
    else {
//unfinished person animation
//        displayPlayerViewport();
        glTranslatef(transx, 0.0, transz);
    }
    
    // Draw bike
    glTranslatef(-9.0, 0.0, 1.0);
    glRotatef(roty*180/PI, 0.0, 1.0, 0.0);
    glTranslatef(9.0, -0.3, -1.0);
    displayBike();
    glPopMatrix();
    
    // Draw the ball if necessary
    for(int i = 0; i < NUM_BALLS; i++) {
        if(ball_timer[i] < 100000 && ballPos[i][1] > 0.0) {
            glPushMatrix();
            glTranslatef(ballPos[i][0], ballPos[i][1], ballPos[i][2]);
            glColor3f(1.0, 1.0, 1.0);
            glutSolidSphere(0.05, 50, 50);
            glPopMatrix();
        }
    }
    
    // Draw 9 houses on the screen
    for(int i = -1; i < 2; i++)
        for(int j=-1; j < 2; j++) {
            glPushMatrix();
            glTranslatef(i*5.0,0,j * 5.0);
            house();
            glPopMatrix();
        }
    
    //Draw Different Trees
    tree(-3.0,0.05f,8.0 ,0.8,0.5,0.8, 30, 1.9f,0.2f,2.0f,1.0f, 1);
    tree(6,0,1.5 ,0.5,0.5,0.5, 0, 1.0f,0.3f,4.0f,1.0f, 2);
    tree(-1.0f, 0,6.0f ,0.6,0.6,0.6, 0, 0.7f,0.3f,1.5f,1.0f, 3);
    tree(2.1f, 0, 6.5f ,0.5,0.5,0.5, 0.2, 1.0f,0.3f,2.0f,1.0f, 1);
    tree(2.1f,0,-1.0f ,0.5,0.5,0.5, 0.2, 1.0f,0.3f,2.0f,1.0f, 1);
    for(int i = -2; i < 1; i++)
        for(int j=-2; j < 1; j++) {
            glPushMatrix();
            glTranslatef(i*5.0,0,j * 5.0);
            tree(-2.5f,0,-2.0f ,0.5,0.5,0.5, 0, 0.3f,0.3f,3.0f,1.0f,4);
            glPopMatrix();
        }
    glPopMatrix();
    
    glutSwapBuffers();
}

/* 
 Mouse function
 Throws ball
 */
void mouse(int btn, int state, int x, int y)
{
    //for ball throwing
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        ball_timer[ball_counter] = 0;
        ballPos[ball_counter][0] = eye[0];
        ballPos[ball_counter][1] = eye[1];
        ballPos[ball_counter][2] = eye[2];
        rotation[ball_counter] = -theta-(PI/2.0);
        prev_xz[ball_counter] = 0.0;
        bounce_count[ball_counter] = 0.0;
        y_start[ball_counter]=0.5;
        v0[ball_counter] = 6.0;
        degree[ball_counter] = (PI/6.0 < -phi*2.0) ? -phi*2.0 : PI/29.0;
        hit[ball_counter] = false;

        throwBall(200);
        
        ball_counter++;
        ball_counter = ball_counter % NUM_BALLS;
    }
   // if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP)
     //   throwBall(-200);
}

/* 
 Keyboard function
 w : move forward
 s : move backward
 a : strafe left
 d : strafe right
 space: mount bike (if within distance)
 enter: jump
 esc: quit
 */
void keyboard(unsigned char key, int x, int z)
{
    if((key == 'w') || (key == 'W')) {
        
        eye[0] -= step*sin(-theta - (PI/2.0))*walkSpeed;
        at[0] -= step*sin(-theta - (PI/2.0))*walkSpeed;
        eye[2] -= step*cos(-theta - (PI/2.0))*walkSpeed;
        at[2] -= step*cos(-theta - (PI/2.0))*walkSpeed;
        moveTight(8, 1);
        rotateLeft = false;
        rotateRight = false;

    }
    
    else if((key == 's') || (key == 'S')) {
        
        eye[0] += step*sin(-theta - (PI/2.0))*walkSpeed;
        at[0] += step*sin(-theta - (PI/2.0))*walkSpeed;
        eye[2] += step*cos(-theta - (PI/2.0))*walkSpeed;
        at[2] += step*cos(-theta - (PI/2.0))*walkSpeed;
        moveTight(-8, 1);
        rotateLeft = false;
        rotateRight = false;

    }
    
    else if((key == 'a') || (key == 'A')) {
        
        eye[0] -= step*sin(-theta)*walkSpeed;
        at[0] -= step*sin(-theta)*walkSpeed;
        eye[2] -= step*cos(-theta)*walkSpeed;
        at[2] -= step*cos(-theta)*walkSpeed;
        moveTight(8, 1);
        rotateLeft = true;
        rotateRight = false;
    }
    
    else if((key == 'd') || (key == 'D')) {
        
        eye[0] += step*sin(-theta)*walkSpeed;
        at[0] += step*sin(-theta)*walkSpeed;
        eye[2] += step*cos(-theta)*walkSpeed;
        at[2] += step*cos(-theta)*walkSpeed;
        moveTight(8, 1);
        rotateLeft = false;
        rotateRight = true;

    }

    else if(key == 32) {
        if(!bike && fabs(eye[0]+9.0-transx)<4.0 && fabs(eye[2]-1.0-transz)<4.0) {
            walkSpeed = 3.0f;
            bike = true;
        }
        else if(bike){
            walkSpeed = 1.0f;
            bike = false;
            transx = eye[0]+9.0;
            transz = eye[2]-1.0;
        }
    }
    
    else if(key == 13 && !bike) {
        jump = true;
    }
    
    else if(key == 27) {
        exit(0);
    }
    
    glutPostRedisplay();
}

/*
 Motion function
 captures mouse movement and allows user to look around scene
 */
void motion(int x, int y) {
    //makes cursor invisible
    glutSetCursor(GLUT_CURSOR_NONE);
    
    float changeX = fabsf(oldX-x)/80;
    float changeY = fabsf(oldY-y)/80;
    
    //calculate difference and move camera
    if((oldX - x) < 0) {    //mouse moved to right
        theta += changeX;
    }
    else if((oldX - x) > 0) {   //mouse moved to left
        theta -= changeX;
    }
    if((oldY - y) > 0) {    //mouse moved up
        if(phi - changeY > -PI/5.0)
            phi -= changeY;
    }
    else if((oldY - y) < 0) {   //mouse moved down
        if(phi + changeY < PI/5.0)
            phi += changeY;
    }
    
    // Recalculate at values
    at[0] = eye[0] + cos(theta);
    at[1] = eye[1] - sin(phi);
    at[2] = eye[2] + cos(phi)*sin(theta);

    //save old mouse coords
    oldX = x;
    oldY = y;
    if(x > w || x < 0.1) {
        glutWarpPointer(w/2, y);
        oldX = w/2;
    }
    if(y > h || y < 0.1) {
        glutWarpPointer(x, h/2);
        oldY = h/2;
    }
    
    glutPostRedisplay();
}

/*
 Reshape function
 */
void reshape(int width, int height) {
    //update width and height values
    w = width;
    h = height;
    
    //Set the viewport
    glViewport(0, 0, w, h);
    
    //Set the matrix mode to projection
    glMatrixMode(GL_PROJECTION);
    
    //Load the identity matrix
    glLoadIdentity();
    
    gluPerspective(60.0, w/h, 0.1, 100.0);
    
    //Set the matrix mode to model view
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("\nThe program should be called as follows:\n\n");
        printf("sceneRendering grassFileName\nPlease try again.\n\n");
        exit(1);
    }
    
    strcpy(filename, argv[1]);
    
    // Initialize the program
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(w, h);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Final Project");
    
    init();
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(motion);
    glEnable(GL_DEPTH_TEST);
    
    glutMainLoop();
    
    return 1;
}