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
extern int light;  //  Lighting
extern int moveLight;  // Move Light
extern int one;  // Unit value
extern int distance;  // Light distance
extern int ballinc;  // Ball increment
extern int smooth;  // Smooth/Flat shading
extern int local;  // Local Viewer Model
extern int emission;  // Emission intensity (%)
extern int ambient;  // Ambient intensity (%)
extern int diffuse;  // Diffuse intensity (%)
extern int specular;  // Specular intensity (%)
extern int shininess;  // Shininess (power of two)
extern float shinyvec[1];  // Shininess (value)

extern double zh ;  // Light azimuth
extern float ylight;  // Elevation of light
extern float Lpos[4];   // Light position

// Global Color Constant
extern double grassColor[3];
extern double dirtColor[3];

void Light(int light);
#endif