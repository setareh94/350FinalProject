//
//  Robot.c
//  sceneRendering
//
//  Created by Setareh Lotfi on 5/11/16.
//  Copyright © 2016 Setareh Lotfi. All rights reserved.
//

#include "Robot.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <OpenGL/glut.h>
#endif

float a_trunk [2] = {0, 0};
float a_larm [4] = {15, 0, 1, 0};
float a_rarm [4] = {0, 0, 0, 0};
float a_lforearm [2] = {20, 1};
float a_rforearm [2] = {0, 0};
float a_lleg [2] = {20, 1};
float a_rleg [2] = {-20, 1};
float a_lthigh [2] = {20, 1};
float a_rthigh [2] = {10, 1};

GLubyte v_colors[][3]   = {
    {  0,  0,  0}, /* black  */
    {255,  0,  0}, /* red    */
    {255,255,  0}, /* yellow */
    {  0,255,  0}, /* green  */
    {  0,  0,255}, /* blue   */
    {255,  0,255}, /* violet */
    {255,255,255}, /* white  */
    {  0,255,255}};/* cyan   */

// Determines whether or not right arm is swinging forward
bool right = false;

void drawKnuckle()
{
    glPushMatrix();
    
    glColor3ubv(v_colors[5]);
    glScalef(0.15, 0.15, 0.15);
    glutSolidSphere(1, 16, 16);
    
    glPopMatrix();
}


void drawTrunk()
{
    glPushMatrix();
    glColor3ubv(v_colors[2]);
    glScalef(1.2, 1.7, 0.5);
    glutSolidCube(1);
    
    glPopMatrix();
}

void drawForeArm()
{
    glPushMatrix();
    
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.55, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

void drawArm()
{
    glPushMatrix();
    
    glColor3ubv(v_colors[2]);
    glRotatef(-30.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.55, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3ubv(v_colors[6]);
    glTranslatef(0.0, -0.5, 0.0);
    glScalef(0.15, 0.25, 0.15);
    glutSolidCube(1);
    
    glPopMatrix();
}

void drawLeftArm()
{
    glTranslatef(-0.75, 0.4, 0.0);
    glRotatef(-15.0, 0.0, 0.0, 1.0);
    
    glPushMatrix();
    
    drawKnuckle();
    
    glRotatef(a_larm[0], a_larm[2], 0, 0);
    glRotatef(a_larm[1], 0, 0, a_larm[3]);
    
    drawForeArm();
    
    glTranslatef(0.0, -1.1, 0.0);
    drawKnuckle();
    
    glRotatef(a_lforearm[0], a_lforearm[1], 0, 0);
    
    drawArm();
    
    glPopMatrix();
}

void drawRightArm()
{
    glTranslatef(1.4, 0.4, 0.0);
    glRotatef(30.0, 0.0, 0.0, 1.0);
    
    glPushMatrix();
    
    drawKnuckle();
    
    glRotatef(a_rarm[0], a_rarm[2], 0, 0);
    glRotatef(a_rarm[1], 0, 0, a_rarm[3]);
    
    drawForeArm();
    
    glTranslatef(0.0, -1.1, 0.0);
    drawKnuckle();
    
    glRotatef(a_rforearm[0], a_rforearm[1], 0, 0);
    
    drawArm();
    
    glPopMatrix();
}

void drawThigh()
{
    glPushMatrix();
    
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.65, 0.0);
    glPushMatrix();
    glScalef(0.3, 1, 0.3);
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
}

void drawLeg()
{
    glPushMatrix();
    
    glColor3ubv(v_colors[2]);
    glTranslatef(0.0, -0.65, 0.0);
    glPushMatrix();
    glScalef(0.3, 1, 0.3);
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3ubv(v_colors[6]);
    glTranslatef(0.0, -0.55, 0.05);
    glScalef(0.35, 0.15, 0.4);
    glutSolidCube(1);
    
    glPopMatrix();
}

void drawLeftLeg()
{
    glTranslatef(-1.3, -1.2, 0.0);
    glPushMatrix();
    
    drawKnuckle();
    
    glRotatef(a_lleg[0], a_lleg[1], 0, 0);
    
    drawThigh();
    
    glTranslatef(0.0, -1.3, 0.0);
    drawKnuckle();
    
    glRotatef(a_lthigh[0], a_lthigh[1], 0, 0);
    
    drawLeg();
    
    glPopMatrix();
}

void drawRightLeg()
{
    glTranslatef(0.65, -0.15, 0.0);
    
    glPushMatrix();
    
    drawKnuckle();
    
    glRotatef(a_rleg[0], a_rleg[1], 0, 0);
    
    drawThigh();
    
    glTranslatef(0.0, -1.3, 0.0);
    drawKnuckle();
    
    glRotatef(a_rthigh[0], a_rthigh[1], 0, 0);
    
    drawLeg();

    glPopMatrix();
}

void fullTrunk()
{
    glPushMatrix();
    glRotatef(a_trunk[0], a_trunk[1], 0, 0);
    drawTrunk();
    drawLeftArm();
    drawRightArm();
    drawLeftLeg();
    drawRightLeg();
    glPopMatrix();
}

void drawRobot()
{
    glPushMatrix();
    glRotated(-90.0, 0.0, 0.0, 0.0);
    glScalef(0.12, 0.12, 0.12);
    fullTrunk();
    glPopMatrix();
}

void moveTight(int a, int b){
    a_rarm[0] = 0;
    a_rarm[2] = 0;

    a_larm[0] -=a;
    a_larm[2] = b;
    a_larm[0] += a;
    a_larm[2] = b;
    
    a_lthigh[1] = b;
    a_rthigh[1] = b;
    
    a_lforearm[1] = b;
    a_rforearm[1] = b;
    
    if(right) {
        a_lforearm[0] +=a;
        a_rforearm[0] -= a;
        
        a_lthigh[0] += a;
        a_rthigh[0] -= a;
    }
    
    else {
        a_lforearm[0] -= a;
        a_rforearm[0] += a;
        
        a_lthigh[0] -= a;
        a_rthigh[0] += a;
    }
    
    if (a_lforearm[0] > 70 || a_lforearm[0] < -70){
        a_lforearm[0] =20;
        a_lforearm[1] = 1;
        a_rforearm[0] =20;
        a_rforearm[1] = 1;
        
        a_lthigh[0] = 0;
        a_lthigh[1] = 1;
        a_rthigh[0] = 0;
        a_rthigh[1] = 1;
        
        right = !right;
    }
}

void throwBall(int a){
    a_rarm[0] -= a;
    
    a_rarm[2] -= a;
}

