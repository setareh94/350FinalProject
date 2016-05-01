// SOURCE FOR MODEL FROM, changed most of the code
// http://www.nbb.cornell.edu/neurobio/land/OldStudentProjects/cs490-97to98/yen/
#include <math.h>
#include <stdlib.h>

#include "Player.h"
#ifndef Tan
#define Tan(th) tan(3.1415927/180*(th))
#endif
#ifndef ATan
#define ATan(th) atan(th)*180/3.1415927
#endif


  double frontOfHumanEye[3];
  double cameraShot[3];
  double aboveHumanEye[3];
  double rightOfHumanEye[3];
  double frontOfHuman[3];
  double aboveHuman[3];
  double rightOfHuman[3];


#define GL_PI 3.1415f // Define a constant for the value of PI

double bodyRotate = 0;
int inc       =   10;  // Ball increment

// mouse position variables, interface variables
GLfloat m_x1Pos = 0;
GLfloat m_y1Pos = 0;
GLfloat m_x1PosPrev = 0;
GLfloat m_y1PosPrev = 0;
GLfloat m_x2Pos = 0;
GLfloat m_y2Pos = 0;
GLfloat m_x2PosPrev = 0;
GLfloat m_y2PosPrev = 0;
int m_LeftButtonDown = 0;
int m_RightButtonDown = 0;



GLfloat humanColor[] = {255/255.0, 228/255.0, 181/255.0};

double animationScaleForawrd = 0; //Used for starting and starting animation
double animationScaleRight = 0;
double animationScaleRotate = 0;
const double ANIMATION_MAX = 20;

typedef struct coordinate coordinate;
struct coordinate
{
    double x;
    double y;
    double z;

    double array[3];
} ;
coordinate Coordinate;
void translate(double xAmmount, double yAmmount, double zAmmount)
{

    Coordinate.x += xAmmount;
    Coordinate.y = yAmmount;
    Coordinate.z = zAmmount;
}

void scale(double xAmmount, double yAmmount, double zAmmount)
{
    Coordinate.x *= xAmmount;
    Coordinate.y *= yAmmount;
    Coordinate.z *= zAmmount;
}

double * toArray()
{
    Coordinate.array[0] = Coordinate.x;
    Coordinate.array[1] = Coordinate.y;
    Coordinate.array[2] = Coordinate.z;
    return Coordinate.array;
}



void animateArm(HumanBody human, HumanMovement movement, Limb arm, double position)
{
    double animationScaleTotal = 	sqrt(pow(animationScaleForawrd/ANIMATION_MAX, 2) +
                                         pow(animationScaleRight/ANIMATION_MAX, 2));
    animationScaleTotal = (animationScaleTotal > 1) ? 1 : animationScaleTotal;
    
    arm.xLimbRot = (position * 45 - 5) * animationScaleTotal;
    arm.xElbowRot = (65 - position * 15 + 40 * animationScaleTotal)  * animationScaleTotal;
}

void animateLeg(HumanBody human, HumanMovement movement, Limb leg, double position, int posScale)
{
    double newRotation = ATan((animationScaleRight)/
                              (animationScaleForawrd + .01));
    int maxRotationSpeed = 5;
    if(movement.rightMovement == 0)
    {
        maxRotationSpeed = 8;
    }
    if(fabs(human.wholeBodyRotate - newRotation) > maxRotationSpeed)
    {
        if(newRotation < human.wholeBodyRotate)
        {
            human.wholeBodyRotate -= maxRotationSpeed;
        }
        else if(newRotation > human.wholeBodyRotate)
        {
            human.wholeBodyRotate += maxRotationSpeed;
        }
    }
    else
    {
        human.wholeBodyRotate = newRotation;
    }
    //printf("whole body rotation %f\n", human->wholeBodyRotate);
    //Forward Movement
    
    double animationScaleTotal = 	sqrt(pow(animationScaleForawrd/ANIMATION_MAX, 2) +
                                         pow(animationScaleRight/ANIMATION_MAX, 2));
    animationScaleTotal = (animationScaleTotal > 1) ? 1 : animationScaleTotal;
    leg.xLimbRot = (position * 35 ) * animationScaleTotal;
    leg.yLimbRot = 180;
    leg.xElbowRot = (position * 40 + 20 * movement.forwardMovement + 20) * animationScaleTotal;
    
    human.xBackRot =  animationScaleTotal * 15 + 5 *
    movement.forwardMovement;
    
    human.yBackRot = 20 * posScale *
    animationScaleForawrd/ANIMATION_MAX *
    animationScaleRotate/ANIMATION_MAX;
    leg.zLimbRot = -human.yBackRot * .8;
    
    // Handle rotation alone
    if(animationScaleRotate != 0 && animationScaleTotal == 0)
    {
        newRotation -= animationScaleRotate;
        if((posScale > 0 && animationScaleRotate < 0) ||
           (posScale < 0 && animationScaleRotate > 0))
        {
            //cos(acos(position)*2) makes the movement faster
            leg.xLimbRot = (cos(acos(position) *2) * 5/8563
                            +
                            + + 5) *
            fabs(animationScaleRotate)/ANIMATION_MAX;
            leg.xElbowRot = (cos(acos(position) *2) * 3 + 3) *
            fabs(animationScaleRotate)/ANIMATION_MAX;
        }
    }
}

void calculateAnimationScale(double scale, int movement, uint32_t elapsedTimeMS)
{
    double change = 1*elapsedTimeMS/(ANIMATION_MAX);
    if((scale > 0 && movement < 0) || (scale < 0 && movement > 0))
    {
        // if they are changing directions, double speed
        change = 2*elapsedTimeMS/(ANIMATION_MAX);
    }
    int idealScale = ANIMATION_MAX * movement;
    
    if(scale < (idealScale - change))
    {
        scale += change;
    }
    else if(scale > (idealScale + change))
    {
        scale -= change;
    }
    else
    {
        scale = idealScale;
    }
}

void animatePlayer(HumanBody human, HumanMovement movement, uint32_t elapsedTimeMS)
{
    if(elapsedTimeMS > 200)
    {
        // If it froze, don't move too far
        elapsedTimeMS = 200;
    }
    double angle = Cos(movement.currentTime/2.5);
    calculateAnimationScale(animationScaleForawrd,
                            movement.forwardMovement,
                            elapsedTimeMS);
    calculateAnimationScale(animationScaleRight,
                            movement.rightMovement,
                            elapsedTimeMS);
    calculateAnimationScale(animationScaleRotate,
                            movement.rightRotate,
                            elapsedTimeMS);
    
    animateArm(human, movement, human.rightArm, angle);
    animateArm(human, movement, human.leftArm, -angle);
    animateLeg(human, movement, human.rightLeg, angle, 1);
    animateLeg(human, movement, human.leftLeg, -angle, -1);
    CheckRestrictions(human);
    movePlayer(movement, elapsedTimeMS);
}

void movePositionArray(double * positionArray,
                       double * movementArray,
                       double movementRate,
                       uint32_t elapsedTimeMS)
{
    positionArray[0] += movementArray[0] * (movementRate*elapsedTimeMS/1000.0);
    positionArray[1] += movementArray[1] * (movementRate*elapsedTimeMS/1000.0);
    positionArray[2] += movementArray[2] * (movementRate*elapsedTimeMS/1000.0);
}
double dmod(double input, double mod)
{
    while(input >= mod)
    {
        input -= mod;
    }
    while(input < 0)
    {
        input += mod;
    }
    return input; 
}

void movePlayer(HumanMovement movement, uint32_t elapsedTimeMS)
{
    int moved = 0;
    //
    double rotationRate = 90.0; // Deg/Sec
    double movementRate = 10.0;
    if(movement.rightRotate != 0)
    {
        moved = 1;
        movement.horizontalAngle
        -= movement.rightRotate * (rotationRate*elapsedTimeMS/1000.0);
        movement.horizontalAngle = dmod(movement.horizontalAngle, 360);
    }
    if(movement.upRotate != 0)
    {
        moved = 1;
        movement.verticalAngle
        -= movement.upRotate * (rotationRate*elapsedTimeMS/1000.0);
        // Make sure we don't look over the top
        if(movement.verticalAngle < -70)
        {
            movement.verticalAngle = -70;
        }
        // Make sure we don't look through the ground
        else if(movement.verticalAngle > 45)
        {
            movement.verticalAngle = 45;
        }
    }
    
    if(movement.rightMovement != 0)
    {
        moved = 1;
        double * rightMovement = setRightOfHuman(movement);
        movePositionArray(movement.position,
                          rightMovement,
                          movementRate * animationScaleRight/ANIMATION_MAX,
                          elapsedTimeMS);
        
    }
    
    if(animationScaleForawrd != 0)
    {
        moved = 1;
        double * forwardMovement = setFrontOfHuman(movement);
        movePositionArray(movement.position,
                          forwardMovement,
                          movementRate * animationScaleForawrd/ANIMATION_MAX,
                          elapsedTimeMS);
    }
    
    if(movement.jumpMovement != 0)
    {
        moved = 1;
        double * upMovement = setAboveHuman(movement);
        movePositionArray(movement.position,
                          upMovement,
                          movementRate * movement.jumpMovement,
                          elapsedTimeMS);
    }
}

void drawPlayer(HumanBody human,HumanMovement movement)
{
    glPushMatrix();
    glRotated(90,1,0,0);
    glScaled(human.scalingFactor, human.scalingFactor, human.scalingFactor);
    glRotated(movement.horizontalAngle + 180 + human.wholeBodyRotate, 0, 1, 0);
    //movement.horizontalAngle + 180 + human->wholeBodyRotate
    glColor3fv(humanColor);
    
    // model Human
    ModelBody(human);
    glPopMatrix();
}

void CheckLimbRestrictions(Limb limb)
{
    if (limb.xLimbRot < -70) limb.xLimbRot = -70;// can't swings limbs back too far
    if (limb.xLimbRot > 180) limb.xLimbRot = 180; // can't swings limbs up too far.
    if (limb.yLimbRot < -60) limb.yLimbRot = -60; // can't rotate limbs inwards
    if (limb.xElbowRot <0) limb.xElbowRot = 0; // can't double joint elbow
    if (limb.xElbowRot >170) limb.xElbowRot = 170; // can't bend past biceps
}

void CheckRestrictions(HumanBody H)
{
    // Human motion angle restrictions
    CheckLimbRestrictions(H.leftArm);
    CheckLimbRestrictions(H.rightArm);
    CheckLimbRestrictions(H.rightLeg);
    CheckLimbRestrictions(H.leftLeg);
    if (H.xBackRot < -30) H.xBackRot = -30; // can't bend back at waist too far
    if (H.xBackRot > 100) H.xBackRot = 100; // can't bend forwards too far.
    if (H.yBackRot < -60) H.yBackRot = -60; // can't tilt left too far
    if (H.yBackRot > 60) H.yBackRot = 60; // can't tilt right too far
    if (H.zBackRot < -60) H.zBackRot = -60; // can't twist left too far
    if (H.zBackRot > 60) H.zBackRot = 60; // can't twist right too far
    if (H.xNeckRot <-30) H.xNeckRot = -30; // can't tilt too far back
    if (H.xNeckRot > 60) H.xNeckRot = 60; // can't tilt head too far forward.
    if (H.yNeckRot <-30) H.yNeckRot = -30; // can't turn head too far
    if (H.yNeckRot > 30) H.yNeckRot = 30; //
}
/*
 *  Draw vertex in polar coordinates with normal
 */
coordinate convertFromSpherical(double th, double ph, double r)
{
    coordinate rectangular;
    rectangular.x = r*Sin(th)*Cos(ph);
    rectangular.y = r*Cos(th)*Cos(ph);
    rectangular.z =         r*Sin(ph);
    return rectangular;
    
}
void Vertex(double th,double ph, double r)
{
    coordinate rectangular = convertFromSpherical(th, ph, r);
    
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3dv(toArray());
    glVertex3dv(toArray());
}

void ovoid(double x, double y, double z, double rx, double ry, double rz, double color[3],
           unsigned int texture, int enableTextures)
{
    int th,ph;
    if(enableTextures)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, enableTextures);
    }
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x, y, z); // I like z going up
    glScaled(rx,ry,rz);
    
    //  Set specular color to white
    //float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    if(color != NULL)
    {
        glColor3dv(color);
    }
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc)
    {
        glBegin(GL_QUAD_STRIP);
        for (th=0;th<=360;th+=2*inc)
        {
            glTexCoord2f(((ph+90)%180)/180.0,(th%360)/360.0);
            Vertex(th,ph,1.0);
            glTexCoord2f(((ph+90+inc)%180)/180.0,(th%360)/360.0);
            Vertex(th,ph+inc,1.0);
        }
        glEnd();
    }
    //  Undo transofrmations
    glPopMatrix();
    if(enableTextures)
    {
        glDisable(GL_TEXTURE_2D);
    }
}
void ball(double x, double y, double z, double r, double color[3], unsigned int texture, int enableTextures)
{
    ovoid(x, y, z, r, r, r, color, texture, enableTextures);
}

void baller(double x,double y,double z,double r, double color[3])
{
    ball(x, y, z, r, color, 0, 0);
}
void drawHand(HumanBody H)
{
    glPushMatrix();
    glColor3f(1,1,1);
    baller(0, 0, 0, 3, NULL);
    glPopMatrix();
}

void drawFoot(HumanBody H)
{
    glPushMatrix();
    glRotated(-90,1,0,0);
    glColor3fv(humanColor);
    gluSphere(H.obj, H.JointRad, H.faces, H.faces);				// ankle
    glRotatef(H.xToeRot, -1.0f, 0.0f, 0.0f);						// toe "joint" on floor
    glTranslatef(0.0f, 0.0f, -H.lFoot);
    gluCylinder(H.obj, H.BoneRad, H.BoneRad, H.lFoot, 10, 10);
    gluSphere(H.obj, H.JointRad, H.faces, H.faces);				// toe
    glPopMatrix();
}

void modelLimb(Limb limb, HumanBody *H, void (*drawEnd) (HumanBody), Side side)
{
    glPushMatrix();
    glRotatef(limb.yLimbRot, 0.0f, 0.0f, 1.0f);
    glRotatef(limb.xLimbRot, -1.0f, 0.0f, 0.0f);
    // shoulder joint
    double zLimbRot;
    if(side == left)
        
        
        
    {
        zLimbRot = -limb.zLimbRot;
    }
    else
    {
        zLimbRot = limb.zLimbRot;
    }
    glRotatef(zLimbRot, 0.0f, 1.0f, 0.0f);
    glColor3fv(humanColor);
    gluCylinder(H->obj, H->BoneRad, H->BoneRad, limb.length/2, H->faces, H->faces);
    glColor3fv(humanColor);
    gluSphere(H->obj, H->JointRad, H->faces, H->faces);
    glTranslatef(0.0, 0.0, limb.length/2);
    glRotatef(limb.xElbowRot, -1.0, 0.0, 0.0);						// elbow joint
    
    gluCylinder(H->obj, H->BoneRad, H->BoneRad, limb.length/2, H->faces, H->faces);
    glPushMatrix();
    glTranslated(0,0,limb.length/2);
    drawEnd(*H);
    glPopMatrix();
    glColor3fv(humanColor);
    gluSphere(H->obj, H->JointRad, H->faces, H->faces);
    glPopMatrix();
}

void modelLeg(Limb limb, HumanBody H, Side side)
{
    glTranslatef(0,  -H.lSpine -H.rightLeg.length,0);
    glPushMatrix();
    
    glRotatef(H.xAnkleRot, -1.0f, 0.0f, 0.0f);					// ankle joint
    
    glTranslated(0, limb.length, 0);
    glRotatef(90, 1.0, 0.0, 0.0);
    
    modelLimb(limb, &H, &drawFoot, side);
    
    glTranslatef(0.0f, 0.0f, limb.length);
    glRotatef(H.xHipRot, -1.0f, 0.0f, 0.0f);						// hip joint
    glRotatef(90, 1.0f, 0.0f, 0.0f);								// manual reset to origin
    glTranslatef(H.wPelvis/2, H.lSpine, 0.0f);
    glPopMatrix();
}


void ModelTorso(HumanBody H)
{
    // torso
    //glPushMatrix();
    glColor3fv(humanColor);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0, -H.lSpine, -H.wPelvis/2);
    gluCylinder(H.obj, H.BoneRad, H.BoneRad, H.wPelvis, 10, 10);
    glTranslatef(0, 0, H.wPelvis/2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glRotatef(H.yBackRot/2, 1.0f, 0.0f, 0.0f);						//twisting torso at waist
    glRotatef(-H.xBackRot/2, 0.0f, 1.0f, 0.0f);					//bending torso at waist
    glRotatef(H.zBackRot/2, 0.0f, 0.0f, 1.0f);						//twisting torso at waist
    gluCylinder(H.obj, H.BoneRad, H.BoneRad, H.lSpine/2, 10, 10);
    glTranslatef(0.0, 0.0, H.lSpine/2);
    glRotatef(H.yBackRot/2, 1.0f,
              
              
              0.0f, 0.0f);						//twisting torso at waist
    glRotatef(-H.xBackRot/2, 0.0f, 1.0f, 0.0f);					//bending torso at waist
    glRotatef(H.zBackRot/2, 0.0f, 0.0f, 1.0f);						//twisting torso at waist
    gluCylinder(H.obj, H.BoneRad, H.BoneRad, H.lSpine/2 + 5, 10, 10);
    gluSphere(H.obj, H.JointRad, H.faces, H.faces);
    glTranslatef(0.0, 0.0, -H.lSpine/2);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(-0.0, H.lSpine + 5, 0.0);							//return to neck level
    //glPopMatrix();
    // end torso
}

void modelHeadAndNeck(HumanBody
                      H)
{
    // neck joint
    glPushMatrix();
    glRotatef(H.xNeckRot, 1.0f, 0.0f, 0.0f);
    glRotatef(H.yNeckRot, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, H.rHead/2, 0.0f);
    
    //Circle for Head
    //-H->rHead * .75
    ovoid(0,
          H.rHead*.9,
          0,
          H.rHead*.75,
          H.rHead ,
          H.rHead*.75,
          NULL, 0, 0);
    glRotated(90, 1, 0, 0);
    gluCylinder(H.obj, H.BoneRad, H.BoneRad, H.rHead/2, 10, 10);
    glPopMatrix();
    
    // end Head
    
    // end neck
}

void ModelBody(HumanBody H)
{
    glPushMatrix();							// save matrix before modelling.
    
    // build lower body
    // build with toes first. enable tiptoes.
    
    // left leg
    glPushMatrix();
    glTranslatef(H.wPelvis/2, 0, 0);
    modelLeg(H.leftLeg, H, left);
    glPopMatrix();									// undo hip displacement from toe movement
    // end left leg
    
    // right leg
    glPushMatrix();
    glTranslatef(-H.wPelvis/2
                 
                 , 0, 0);
    modelLeg(H.rightLeg, H, right);
    glPopMatrix();
    // end right leg
    
    ModelTorso(H);
    
    modelHeadAndNeck(H);

    
    // shoulders and back
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0, 0.0f);
    glTranslatef(0.0f, 0.0f, -H.wBack/2);
    gluCylinder(H.obj, H.BoneRad, H.BoneRad, H.wBack, 10, 10);
    glPopMatrix();
    // end shoulders and back
    
    // right arm
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(-H.wBack/2, 0.0, 0.0);
    modelLimb(H.rightArm, &H, &drawHand, right);
    glPopMatrix();
    // end right arm
    
    // left arm
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(H.wBack/2, 0.0, 0.0);
    modelLimb(H.leftArm, &H, &drawHand, left);
    glPopMatrix(); 
    // end left arm
    
    glPopMatrix(); // end model
}

void initLimb(Limb limb, double length)
{
    limb.xLimbRot	= 0;// results in no movement...
    limb.yLimbRot	= 0
    ;
    limb.zLimbRot	= 0;
    limb.xElbowRot	= 1; // Cannot start w/ all joint angles at 0;
    limb.length = length; 
}


HumanBody HumanInit(double height)
{
    
    HumanBody *hum = malloc(sizeof(HumanBody));
    
    initLimb(hum->leftArm, 40); 
    initLimb(hum->rightArm, 40); 
    initLimb(hum->leftLeg, 60); 
    initLimb(hum->rightLeg, 60); 
    hum->wholeBodyRotate = 0; 
    hum->xHipRot	= 0;
    hum->xKneeRot	= 0;
    hum->xAnkleRot	= 0;
    hum->xToeRot	= 0;
    hum->xNeckRot	= 0;
    hum->yNeckRot	= 0;
    hum->xBackRot	= 0;
    hum->yBackRot	= 0;
    hum->zBackRot	= 0;
    hum->JointRad	= 3;
    hum->BoneRad	= 2;
    hum->rHead		= 10;
    hum->wPelvis	= 20;
    hum->lSpine		= 40;
    hum->wBack		= 40;
    hum->lFoot		= 10;
    hum->faces		= 5; // low res polygons for speed
    
    hum->height = height; 
    hum->scalingFactor 
    = height/((hum->lSpine + hum->rightLeg.length));
    printf("scaling factor %f\n", hum->scalingFactor); 
    
    hum->obj		= gluNewQuadric(); // new var for quadric object.
    
    
    return *hum;
}



