//
//  Lighting.h
//  sceneRendering
//
//  Created by Setareh Lotfi on 5/10/16.
//  Copyright © 2016 Setareh Lotfi. All rights reserved.
//

#ifndef Lighting_h
#define Lighting_h

// Light Values
extern int distance;  // Light distance

extern double zh ;  // Light azimuth
extern float ylight;  // Elevation of light
extern float Lpos[4];   // Light position

// Global Color Constant
extern double grassColor[3];
extern double dirtColor[3];

void Light(int light);
#endif