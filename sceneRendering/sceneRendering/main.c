/*
 *  Setareh Lotfi
 *  Katie Fotion
 *  Hayden Clevenger
 *  4/3/2016
 *
 *  This assignment is the culmination of everything we learned this semester
 *
 *  Usage:      //FILL IN USAGE//
 *
 *  Overview:   //FILL IN OVERVIEW//
 *
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include "Player.h"

float step = 0.25;   //for moving camera

float m=0.0;    //movement of clouds

float w = 800, h = 600; // Width and Height of the window

//for gluLookAt function
float eye[3] = {1.5f, 1.0f, 14.0f}; //eye vector
float at[3] = {0.0f, 1.0f, -1.0f};  //at vector

float angle = 0.0f; // angle for rotating triangle

//double eyeIdealPosition[3] = {-10, 10, -10};
double eyePosition[3];
double eyeFocus[3];

HumanBody human;
HumanMovement humanMovement;

enum View
{
    //eye,
    projection,
    orthoginal
};

void moveCamera()
{
    double * cameraOffset = setCameraShot(humanMovement, &human);
    
    //Eye behind human
    eyePosition[0] = - human.height*2*cameraOffset[0]; //humanMovement.position[0] - human->height*2*cameraOffset[0];
    eyePosition[1] = - human.height*2*cameraOffset[1] + human.height * .1;
    //humanMovement.position[1] - human->height*2*cameraOffset[1] + human->height * .1;
    eyePosition[2] = - human.height*2*cameraOffset[2] - human.height * .1; //humanMovement.position[2] - human->height*2*cameraOffset[2] - human->height * .1;
    //Focus on Human
    eyeFocus[0] = 0;  //humanMovement.position[0];
    eyeFocus[1] = + human.height * .1; //humanMovement.position[1] + human->height * .1;
    eyeFocus[2] = - human.height * .1; //humanMovement.position[2] - human->height * .1;
}

void initHuman(double height)
{
    human = HumanInit(height);
    humanMovement.rightMovement = 0;
    humanMovement.forwardMovement = 0;
    humanMovement.jumpMovement = 0;
    humanMovement.rightRotate = 0;
    humanMovement.currentTime = 0;
    humanMovement.upRotate = 0;
    humanMovement.position[0] = 0;  //-10;
    humanMovement.position[1] = height; //10;
    humanMovement.position[2] = 0; //-10;
    humanMovement.verticalAngle = 0;  //-10;
    humanMovement.horizontalAngle = -180; //45;
    humanMovement.moving = standing;
    setCameraShot(humanMovement, &human);
}


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



/* Initate the matrix mode and viewPort for the window */
void init(void)
{
    glMatrixMode(GL_PROJECTION);
    
    // Reset Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set the correct perspective.
    gluPerspective(60.0, w/h, 1.0, 100.0);
    
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
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
    glFlush();
}

//make the ground
void ground()
{
    int i, j;
    glLineWidth(1);
    for (i = 0; i < 20; i++)
    {
        glBegin(GL_LINES);
        glVertex3f(-10.0 + i, -1.0, 10.0);
        glVertex3f(-10.0 + i, -1.0, -10.0);
        glEnd();
    }
    for (j = 0; j < 20; j++)
    {
        glBegin(GL_LINES);
        glVertex3f(-10.0, -1.0, 10.0 - j);
        glVertex3f(10.0, -1.0, 10.0 - j);
        glEnd();
    }
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

/* Idele function for cloud animations */
void idle()
{
    m +=0.005;
    glutPostRedisplay();
    
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.6,0.6,1.0,1.0);
    // Reset transformations
    glLoadIdentity();
    // Set the camera
    
    gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0, 1, 0);
    
    glColor3f(1.0, 1.0, 1.0);
    
    //Draw the scene
    ground();
    cloud();
    sun();
   // drawPlayer(human, humanMovement);

    
    // Draw 9 houses on the screen
    for(int i = -1; i < 2; i++)
        for(int j=-1; j < 2; j++) {
            glPushMatrix();
            glTranslatef(i*5.0,0,j * 5.0);
            house();
            glPopMatrix();
        }
    
    //Draw Different Trees
    tree(-3.0,-0.2f,8.0 ,0.8,0.5,0.8, 30, 1.9f,0.2f,2.0f,1.0f, 1);
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
    glutSwapBuffers();
    
}

/* 
 Mouse function
 */
void mouse(int btn, int state, int x, int y)
{
    
    //do something with mouse - possibly throw balls
    
    glutPostRedisplay();
}

/* 
 Keyboard function
 w : move forward
 s : move backward
 a : strafe left
 d : strafe right
 */
void keyboard(unsigned char key, int x, int z)
{
    if((key == 'w') || (key == 'W')) {
        eye[2] -= step;
        at[2] -= step;
    }
    
    if((key == 's') || (key == 'S')) {
        eye[2] += step;
        at[2] += step;
    }
    
    if((key == 'a') || (key == 'A')) {
        eye[0] -= step;
        at[0] -= step;
    }
    
    if((key == 'd') || (key == 'D')) {
        eye[0] += step;
        at[0] += step;
    }
    
    if(key == 27) {
        exit(0);
    }
    
    init();
    glutPostRedisplay();
}

/* 
 Arrow Key Function
 Controls the at[] vector and direct where the user looks
 */
void special(int key,int x,int y)
{
    switch (key) {
        case GLUT_KEY_LEFT :
            //add functionality
            break;
        case GLUT_KEY_RIGHT :
            //add functinality
            break;
        case GLUT_KEY_UP :
            //add functionality
            break;
        case GLUT_KEY_DOWN :
            //add functionality
            break;
    }
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
    
    gluPerspective(60.0, w/h, 1.0, 100.0);
    
    //Set the matrix mode to model view
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
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
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glEnable(GL_DEPTH_TEST);
    
    
    glutMainLoop();
    
    return 1;
}