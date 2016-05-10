//
//  Scooter.c
//  sceneRendering
//
//  Created by Katie on 5/1/16.
//  Copyright © 2016 Setareh Lotfi. All rights reserved.
//

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Scooter.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define   PI            3.14159
#define WIN_WIDTH      600
#define WIN_HEIGHT      600
#define CYCLE_LENGTH   3.3f
#define ROD_RADIUS      0.05f
#define NUM_SPOKES      20
#define SPOKE_ANGLE      18
#define RADIUS_WHEEL   1.0f
#define TUBE_WIDTH      0.08f
#define RIGHT_ROD      1.6f
#define RIGHT_ANGLE      48.0f
#define MIDDLE_ROD      1.7f
#define MIDDLE_ANGLE   106.0f
#define BACK_CONNECTOR   0.5f
#define LEFT_ANGLE      50.0f
#define WHEEL_OFFSET   0.11f
#define WHEEL_LEN      1.1f
#define TOP_LEN         1.5f
#define CRANK_ROD      0.7f
#define CRANK_RODS      1.12f
#define CRANK_ANGLE      8.0f
#define HANDLE_ROD      1.2f
#define FRONT_INCLINE   70.0f
#define HANDLE_LIMIT   70.0f

#define INC_STEERING   2.0f
#define INC_SPEED      0.05f
GLfloat pedalAngle, speed, steering;
//User view realted variables if needed
GLfloat camx,camy,camz;
GLfloat anglex,angley,anglez;




void ZCylinder(GLfloat radius,GLfloat length)
{
    GLUquadricObj *cylinder;
    cylinder=gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.0f);
    gluCylinder(cylinder,radius,radius,length,15,5);
    glPopMatrix();
}


void XCylinder(GLfloat radius,GLfloat length)
{
    glPushMatrix();
    glRotatef(90.0f,0.0f,1.0f,0.0f);
    ZCylinder(radius,length);
    glPopMatrix();
}
void drawTyre()
{
    int i;
    //   Draw The Rim
    glColor3f(1.0f,1.0f,1.0f);
    glutSolidTorus(0.06f,0.92f,4,30);
    //   Draw The Central Cylinder
    //   Length of cylinder  0.12f
    glColor3f(1.0f,1.0f,0.5f);
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-0.06f);
    ZCylinder(0.02f,0.12f);
    glPopMatrix();
    glutSolidTorus(0.02f,0.02f,3,20);
    

    
    //   Draw The Tyre
    glColor3f(0.0f,0.0f,0.0f);
    glutSolidTorus(TUBE_WIDTH,RADIUS_WHEEL,10,30);
    glColor3f(1.0f,0.0f,0.0f);
}
void drawSeat()
{
    /*********************************
     *   Draw the top of the seat
     **********************************/
    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, 1.0f, -0.5f);
    glVertex3f(   1.0f, 1.0f, -0.3f);
    glVertex3f( 1.0f, 1.0f,  0.3f);
    glVertex3f(-0.1f, 1.0f,  0.5f);
    glVertex3f(-0.5f, 1.0f,  1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-0.5f, 1.0f, -1.0f);
    glEnd();
    
    /**********************************
     *   Draw the bottom base part of the
     *   seat
     ************************************/
    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, -1.0f, -0.5f);
    glVertex3f(   1.0f, -1.0f, -0.3f);
    glVertex3f( 1.0f, -1.0f,  0.3f);
    glVertex3f(-0.1f, -1.0f,  0.5f);
    glVertex3f(-0.5f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-0.5f, -1.0f, -1.0f);
    glEnd();
    
    /**********************
     *   Draw the sides!
     ***********************/
    glBegin(GL_QUADS);
    glVertex3f(1.0f,1.0f,-0.3f);
    glVertex3f(1.0f,1.0f,0.3f);
    glVertex3f(1.0f,-1.0f,0.3f);
    glVertex3f(1.0f,-1.0f,-0.3f);
    
    glVertex3f(1.0f,1.0f,0.3f);
    glVertex3f(-0.1f,1.0f,0.5f);
    glVertex3f(-0.1f,-1.0f,0.5f);
    glVertex3f(1.0f,-1.0f,0.3f);
    
    glVertex3f(1.0f,1.0f,-0.3f);
    glVertex3f(-0.1f,1.0f,-0.5f);
    glVertex3f(-0.1f,-1.0f,-0.5f);
    glVertex3f(1.0f,-1.0f,-0.3f);
    
    glVertex3f(-0.1f,1.0f,0.5f);
    glVertex3f(-0.5f,1.0f,1.0f);
    glVertex3f(-0.5f,-1.0f,1.0f);
    glVertex3f(-0.1f,-1.0f,0.5f);
    
    glVertex3f(-0.1f,1.0f,-0.5f);
    glVertex3f(-0.5f,1.0f,-1.0f);
    glVertex3f(-0.5f,-1.0f,-1.0f);
    glVertex3f(-0.1f,-1.0f,-0.5f);
    
    glVertex3f(-0.5f,1.0f,1.0f);
    glVertex3f(-1.0f,1.0f,1.0f);
    glVertex3f(-1.0f,-1.0f,1.0f);
    glVertex3f(-0.5f,-1.0f,1.0f);
    
    glVertex3f(-0.5f,1.0f,-1.0f);
    glVertex3f(-1.0f,1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-0.5f,-1.0f,-1.0f);
    
    glVertex3f(-1.0f,1.0f,1.0f);
    glVertex3f(-1.0f,1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,1.0f);
    
    glEnd();
    
    
}
// Portions of this code have been borrowed from Brian Paul's Mesa
// distribution.
/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 */

void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
          GLint teeth, GLfloat tooth_depth )
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;
    const double pi = 3.14159264;
    
    r0 = inner_radius;
    r1 = outer_radius - tooth_depth/2.0;
    r2 = outer_radius + tooth_depth/2.0;
    
    da = 2.0*pi / teeth / 4.0;
    
    glShadeModel( GL_FLAT );
    
    glNormal3f( 0.0, 0.0, 1.0 );
    
    
    glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 );
    glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 );
    
    glEnd();
    
    
    glShadeModel( GL_SMOOTH );
    
    /* draw inside radius cylinder */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
        angle = i * 2.0*pi / teeth;
        glNormal3f( -cos(angle), -sin(angle), 0.0 );
        glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
        glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
    }
    glEnd();
    
}



// angleSum(a,b) = (a+b) MOD 2*PI
// a and b are two angles (radians)
//  both between 0 and 2*PI
GLfloat angleSum(GLfloat a, GLfloat b)
{
    a += b;
    if (a < 0) return a+2*PI;
    else if (a > 2*PI) return a-2*PI;
    else return a;
}

void drawFrame()
{
    glColor3f(1.0f,0.0f,0.0f);
    
    /********************************
     *   First draw the all the items
     *   at the center of the frame.
     *   Draw the bigger gear,the small
     *   cylinder acting as the socket
     *   for the pedals.Also DON'T
     *   forget to draw the two
     *   connecting cemtral rods
     *********************************/
    glPushMatrix();
    /******************************
     *   Allow me to draw the BIGGER
     *   gear and the socket cylinder
     *******************************/
    glPushMatrix();
    /***************************
     *   Let the gear have the
     *   green color
     ***************************/
    glColor3f(0.0f,1.0f,0.0f);
    
    /**************************
     *   The gear should be
     *   outside the frame !!!
     *   This is the bigger
     *   GEAR
     ***************************/
    glPushMatrix();
    glTranslatef(0.0f,0.0f,0.06f);
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    gear(0.08f,0.3f,0.03f,30,0.03f);
    glPopMatrix();
    /***************************
     *   Restore the color of the
     *   frame
     ****************************/
    glColor3f(1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,-0.2f);
    ZCylinder(0.08f,0.32f);
    glPopMatrix();
    /*****************************
     *   Lets first draw the
     *   rightmost rod of the frame
     *******************************/
    glRotatef(RIGHT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,RIGHT_ROD);
    
    /*******************************
     *   Now draw the centre rod of
     *   the frame which also supports
     *   the seat
     *********************************/
    glRotatef(MIDDLE_ANGLE-RIGHT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,MIDDLE_ROD);
    /********************************
     *   We have drawn the support.So
     *   let's draw the seat with a
     *   new color
     *********************************/
    glColor3f(1.0f,1.0f,0.0f);
    glTranslatef(MIDDLE_ROD,0.0f,0.0f);
    glRotatef(-MIDDLE_ANGLE,0.0f,0.0f,1.0f);
    glScalef(0.3f,ROD_RADIUS,0.25f);
    drawSeat();
    /**********************
     *   Restore the color !
     ************************/
    glColor3f(1.0f,0.0f,0.0f);
    glPopMatrix();
    /*********************************
     *   Draw the horizontal part of
     *   the frame.
     *********************************/
    
    /*********************************
     *   Draw the main single rod
     *   connecting the center of the
     *   frame to the back wheel of the
     *   cycle
     **********************************/
    glPushMatrix();
    glRotatef(-180.0f,0.0f,1.0f,0.0f);
    XCylinder(ROD_RADIUS,BACK_CONNECTOR);
    
    /***********************************
     *   Draw the two rods on the either
     *   side of the wheel
     *   These rods are part of the
     *   horizontal part of the cycle
     ************************************/
    glPushMatrix();
    glTranslatef(0.5f,0.0f,WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.5f,0.0f,-WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,RADIUS_WHEEL+TUBE_WIDTH);
    glPopMatrix();
    glPopMatrix();
    
    /************************************
     *   Draw the leftmost rods of the
     *   frame of the cycle
     *************************************/
    glPushMatrix();
    glTranslatef(-(BACK_CONNECTOR+RADIUS_WHEEL+TUBE_WIDTH),0.0f,0.0f);
    /********************************
     *   Transalted to the back wheel
     *   position.Why not draw the back
     *   wheel and also the gear ? :))
     **********************************/
    glPushMatrix();
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    drawTyre();
    glColor3f(0.0f,1.0f,0.0f);
    gear(0.03f,0.15f,0.03f,20,0.03f);
    glColor3f(1.0f,0.0f,0.0f);
    glPopMatrix();
    glRotatef(LEFT_ANGLE,0.0f,0.0f,1.0f);
   
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,WHEEL_LEN);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,WHEEL_LEN);
    glPopMatrix();
    
    /*****************************
     *   Draw the single rod of the
     *   same setup
     ******************************/
    glTranslatef(WHEEL_LEN,0.0f,0.0f);
    XCylinder(ROD_RADIUS,CRANK_ROD);
    
    /*****************************
     *   Now Draw The topmost
     *   Horizontal rod
     *****************************/
    glTranslatef(CRANK_ROD,0.0f,0.0f);
    glRotatef(-LEFT_ANGLE,0.0f,0.0f,1.0f);
    XCylinder(ROD_RADIUS,TOP_LEN);
    
       glTranslatef(TOP_LEN,0.0f,0.0f);
    glRotatef(-FRONT_INCLINE,0.0f,0.0f,1.0f);
    
    /******************************
     *   Draw the small rod that acts
     *   as the socket joining the
     *   frame and the handle
     ******************************/
    glPushMatrix();
    glTranslatef(-0.1f,0.0f,0.0f);
    XCylinder(ROD_RADIUS,0.45f);
    glPopMatrix();
    

    glPushMatrix();
    glRotatef(-steering,1.0f,0.0f,0.0f);
    /******************************
     *   Roll back to the height of
     *   the handle to draw it
     *******************************/
    glTranslatef(-0.3f,0.0f,0.0f);
    

    glPushMatrix();
    glRotatef(FRONT_INCLINE,0.0f,0.0f,1.0f);
    
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
    ZCylinder(ROD_RADIUS,HANDLE_ROD);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(1.0f,1.0f,0.0f);
    glTranslatef(0.0f,0.0f,-HANDLE_ROD/2);
    ZCylinder(0.07f,HANDLE_ROD/4);
    glTranslatef(0.0f,0.0f,HANDLE_ROD*3/4);
    ZCylinder(0.07f,HANDLE_ROD/4);
    glColor3f(1.0f,0.0f,0.0f);
    glPopMatrix();
    glPopMatrix();
    

    glPushMatrix();
    /****************************
     *   Draw the main big rod
     ****************************/
    XCylinder(ROD_RADIUS,CRANK_ROD);
    

    glTranslatef(CRANK_ROD,0.0f,0.0f);
    glRotatef(CRANK_ANGLE,0.0f,0.0f,1.0f);
    

    glPushMatrix();
    glTranslatef(0.0f,0.0f,WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,CRANK_RODS);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f,0.0f,-WHEEL_OFFSET);
    XCylinder(ROD_RADIUS,CRANK_RODS);
    glPopMatrix();
    /********************************
     *   Why not draw the wheel.
     *   The FRONT wheel to be precise
     *********************************/
    glTranslatef(CRANK_RODS,0.0f,0.0f);
    glRotatef(-2*pedalAngle,0.0f,0.0f,1.0f);
    drawTyre();
    glPopMatrix();
    glPopMatrix();   /*   End of the rotation of the handle effect   */
    glPopMatrix();
}



void displayBike(){
    glPushMatrix();
    glTranslatef(-9.0,0.5f,1.0);
    glRotatef(0.0,0.0f,1.0f,0.0f);
    glScalef(0.2, 0.2, 0.2);
    drawFrame();
    glPopMatrix();
    
    glPopMatrix();
    
}

