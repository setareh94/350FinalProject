#ifndef PLAYER_H
#define PLAYER_H 1

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef Cos
#define Cos(th) cos(3.1415927/180*(th))
#endif
#ifndef Sin
#define Sin(th) sin(3.1415927/180*(th))
#endif
#ifndef Tan
#define Tan(th) tan(3.1415927/180*(th))
#endif


extern double frontOfHumanEye[3];
extern double cameraShot[3];
extern double aboveHumanEye[3];
extern double rightOfHumanEye[3];
extern double frontOfHuman[3];
extern double aboveHuman[3];
extern double rightOfHuman[3];


extern double bodyRotate;

//
//	variable declarations
//
typedef struct
{
    GLfloat xLimbRot;	// + is swinging forward at shoulder	[right]
    GLfloat yLimbRot;	// - is opening Limbs at shoulder.		[right]
    GLfloat zLimbRot;	// + is twisting Limb along axis1		[right];
    GLfloat xElbowRot;	// + is bending lower Limb at elbow		[right]
    GLfloat length;
} Limb;

typedef enum Side
{
    left,
    right
} Side;

// data structure for body information
typedef struct HumanBody{
    double scalingFactor;
    double height;
    Limb leftArm;
    Limb rightArm;
    Limb rightLeg;
    Limb leftLeg;
    GLfloat wholeBodyRotate;
    GLfloat xElbowRot2;	// + is bending lower Limb at elbow		[left]
    GLfloat xHipRot;	// + is bending legs up
    GLfloat xKneeRot;	// + is bending at knees
    GLfloat xAnkleRot;	// + is tilting at ankles with feet planted.
    GLfloat xToeRot;	// + is flexing toes up
    GLfloat xNeckRot;	// + is tilting head forward
    GLfloat yNeckRot;	// + is turning head ccw
    GLfloat xBackRot;	// + is tilting torso forward
    GLfloat yBackRot;	// + is tilting torso sideways
    GLfloat zBackRot;	// + is twisting torso ccw
    
    GLfloat JointRad;	// skeleton specs
    GLfloat BoneRad;
    GLfloat rHead;
    GLfloat lSpine;
    GLfloat wPelvis;
    
    //GLfloat lLeg;
    GLfloat wBack;
    GLfloat lFoot;
    
    GLint faces;		// polygon resolution
    GLUquadricObj *obj;	// quadric object init
    
} Human;


enum movementType
{
    standing,
    walking,
    running,
    jumping
};

typedef struct
{
    enum movementType moving;
    int rightMovement;
    int forwardMovement;
    int jumpMovement;
    int rightRotate;
    int upRotate;
    
    double position[3];
    double verticalAngle;
    double horizontalAngle;
    
    uint32_t currentTime; //MS
} HumanMovement;

void drawPlayer(Human human,HumanMovement movement);

HumanBody HumanInit(double height);
void CheckRestrictions(HumanBody H);
void ModelBody(HumanBody H);
void ModelShadow(HumanBody H);
void RenderScene(HumanBody H);
void movePlayer(HumanMovement movement, uint32_t elapsedTimeMS);
void animatePlayer(HumanBody human,HumanMovement movement, uint32_t elapsedTimeMS);




static inline double * getFrontOfHumanEye(HumanMovement movement)
{
    frontOfHumanEye[0] = Cos(movement.verticalAngle) *
    Cos(movement.horizontalAngle);
    frontOfHumanEye[1] = Sin(movement.verticalAngle);
    frontOfHumanEye[2] = Cos(movement.verticalAngle) *
    Sin(movement.horizontalAngle);
    return frontOfHumanEye;
};


static inline double * getCameraShot()
{
    return cameraShot;
}
static inline double * setCameraShot(HumanMovement movement, HumanBody *human)
{
    cameraShot[0] = Cos(movement.verticalAngle - 10) *
    Cos(movement.horizontalAngle  );
    cameraShot[1] = Sin(movement.verticalAngle - 10);
    cameraShot[2] = Cos(movement.verticalAngle - 10) *
    Sin(movement.horizontalAngle  );
    return cameraShot;
};

static inline double * setAboveHumanEye(HumanMovement movement)
{
    aboveHumanEye[0] = Cos(movement.verticalAngle + 90) *
    Cos(movement.horizontalAngle);
    aboveHumanEye[1] = Sin(movement.verticalAngle + 90);
    aboveHumanEye[2] = 0;
    return aboveHumanEye;
};

static inline double * setRightOfHumanEye(HumanMovement movement)
{
    rightOfHumanEye[0] = Cos(movement.verticalAngle) *
    Cos(movement.horizontalAngle + 90 );
    rightOfHumanEye[1] = 0;
    rightOfHumanEye[2] = Cos(movement.verticalAngle) *
    Sin(movement.horizontalAngle + 90);
    return rightOfHumanEye;
};


//Keeps the person on the ground
static inline double * setFrontOfHuman(HumanMovement movement)
{
    frontOfHuman[0] = Cos(movement.horizontalAngle);
    frontOfHuman[1] = 0;
    frontOfHuman[2] = Sin(movement.horizontalAngle);
    return frontOfHuman;
};

static inline double * setAboveHuman(HumanMovement movement)
{
    aboveHuman[0] = 0; //Cos(movement.verticalAngle + 90);
    aboveHuman[1] = 1; //Sin(movement.verticalAngle + 90);
    aboveHuman[2] = 0;
    return aboveHuman;
};

static inline double * setRightOfHuman(HumanMovement movement)
{
    rightOfHuman[0] = Cos(movement.horizontalAngle + 90 );
    rightOfHuman[1] = 0; 
    rightOfHuman[2] = Sin(movement.horizontalAngle + 90); 
    return rightOfHuman; 
};

#endif
