//
//  Lighting.c
//  sceneRendering
//
//  Created by Setareh Lotfi on 5/10/16.
//  Copyright © 2016 Setareh Lotfi. All rights reserved.
//

#include "Lighting.h"
#include <GLUT/GLUT.h>
#include <math.h>

extern double eyePosition[3];

// Light Values
int distance  =   200;  // Light distance

double zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
float        Lpos[4];   // Light position

/*
 *  Set light
 *    light>0 bright
 *    light<0 dim
 *    light=0 off
 */
void Light(int light)
{
    //  Set light position
    Lpos[0] = distance*cos(zh);
    Lpos[1] = ylight;
    Lpos[2] = distance*sin(zh);
    Lpos[3] = 1;
    
    //  Enable lighting
    if (light != 0)
    {
        float Low[]  = {0.1,0.1,0.1,1.0};
        float Med[]  = {0.3,0.3,0.3,1.0};
        float High[] = {1.0,1.0,1.0,1.0};
        //  Enable lighting with normalization
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0,GL_POSITION,Lpos);
        glLightfv(GL_LIGHT0,GL_AMBIENT,light>0?Med:Low);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light>0?High:Med);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_NORMALIZE);
    }
}
