#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <iostream>
#include <math.h>
#include "texturepath.h"

#define NUM 19					// Number of links

#define PI 3.141592653

static dWorldID world;
static dSpaceID space;
static dGeomID  ground;
static dJointGroupID contactgroup;
static int flag = 0;
dsFunctions fn;

int loopstep = 1;
int loopcount = 0;

//		SERVOS			{Base,Leg,Foot}
static double servos[6][3] =   {{0.0, -0.50, -0.750,},
								{0.0, -0.50, -0.750,},
								{0.0, -0.50, -0.750,},
								{0.0, -0.50, -0.750,},
								{0.0, -0.50, -0.750,},
								{0.0, -0.50, -0.750}};


static double THETA[NUM] = { 0.0,	0.0, -0.50, -0.750,		
									0.0, -0.50, -0.750,	
									0.0, -0.50, -0.750,
									0.0, -0.50, -0.750,
									0.0, -0.50, -0.750,
									0.0, -0.50, -0.750		};		// Target joint angles[rad]

static double l[NUM]  =	   { 0.10,	1.00, 1.00, 1.00,	
									1.00, 1.00, 1.00,	
									1.00, 1.00, 1.00,
									1.00, 1.00, 1.00,
									1.00, 1.00, 1.00,
									1.00, 1.00, 1.00	};		// Length of links[m]

static double r[NUM]  =	   { 0.80,	0.04, 0.04, 0.04,	
									0.04, 0.04, 0.04,
									0.04, 0.04, 0.04,
									0.04, 0.04, 0.04,
									0.04, 0.04, 0.04,
									0.04, 0.04, 0.04	};		// Radius of links[m]


typedef struct {
	dBodyID body;
	dGeomID geom;
} Object;

//dWorldID    world;			// A dynamic world
Object		link[NUM];			// Links　link[0] is a base
dJointID    joint[NUM];			// Joints    joint[0] is a fixed joint between a base and a ground


static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
  //printf("Hit");
  const int N = 10;
  dContact contact[N];

  int isGround = ((ground == o1) || (ground == o2));

  int n =  dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));

  if (isGround)  {
    if (n >= 1) flag = 1;
    else        flag = 0;
    for (int i = 0; i < n; i++) {
      contact[i].surface.mode = dContactBounce;
      contact[i].surface.mu   = 200.0;
      contact[i].surface.bounce     = 0.0; // (0.0~1.0) restitution parameter
      contact[i].surface.bounce_vel = 0.0; // minimum incoming velocity for bounce
      dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
      dJointAttach (c,dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));
    }
  }
}


void control() 
{ 
	/***  P control  ****/
	static int step = 0;									// Steps of simulation    
	double k1 =  10.0,  fMax  = 500.0;						// k1: proportional gain,  fMax：Max torque[Nm]

	printf("\r%6d:",step++);
	for (int j = 1; j < NUM; j++) 
	{
		double tmpAngle = dJointGetHingeAngle(joint[j]);	// Present angle[rad]
		double z = THETA[j] - tmpAngle;						// z: residual=target angle - present angle
		dJointSetHingeParam(joint[j], dParamVel, k1*z);		// Set angular velocity[m/s]
		dJointSetHingeParam(joint[j], dParamFMax, fMax);	// Set max torque[N/m]
	}
}

void start() 
{ 
	/*** Initialize drawing API ***/
	float xyz[3] = {  3.04, 1.28, 0.76};						// View point x, y, z　[m]
	float hpr[3] = { -160.0, 4.50, 0.00};						// View direction(heading, pitch, roll)　[°]
	dsSetViewpoint(xyz,hpr);									// Set view point and direction
}

void command(int cmd)
{
	switch(cmd)
	{
	case 'q':
		THETA[1] += 0.05;
		break;
	case 'w':
		THETA[1] -= 0.05;
		break;
	case 'e':
		THETA[2] += 0.05;
		break;
	case 'r':
		THETA[2] -= 0.05;
		break;
	case 't':
		THETA[3] += 0.05;
		break;
	case 'y':
		THETA[3] -= 0.05;
		break;
	}
	
    if (THETA[1] <   - M_PI)    THETA[1] =  - M_PI;
    if (THETA[1] >     M_PI)    THETA[1] =    M_PI;
    if (THETA[2] <  -2*M_PI/3)  THETA[2] =  - 2*M_PI/3;
    if (THETA[2] >   2*M_PI/3)  THETA[2] =    2*M_PI/3;
    if (THETA[3] <  -2*M_PI/3)  THETA[3] =  - 2*M_PI/3;
    if (THETA[3] >   2*M_PI/3)  THETA[3] =    2*M_PI/3;


	
	switch(cmd)
	{
	case '1':
		// Lift Legs
		servos[1][1] += 0.50;
		servos[1][2] += 0.50;
		servos[3][1] += 0.50;
		servos[3][2] += 0.50;
		servos[5][1] += 0.50;
		servos[5][2] += 0.50;
		break;
	case '2':
		// Move Body Forward
		servos[0][0] += 0.25;
		servos[2][0] += 0.25;
		servos[4][0] -= 0.25;
		break;
	case '3':
		// Move Legs Forward
		servos[1][0] -= 0.25;
		servos[3][0] += 0.25;
		servos[5][0] += 0.25;
		break;
	case '4':
		// Drop Legs
		servos[1][1] -= 0.50;
		servos[1][2] -= 0.50;
		servos[3][1] -= 0.50;
		servos[3][2] -= 0.50;
		servos[5][1] -= 0.50;
		servos[5][2] -= 0.50;
		break;
	case '5':
		// Lift Legs
		servos[0][1] += 0.25;
		servos[0][2] += 0.25;
		servos[2][1] += 0.25;
		servos[2][2] += 0.25;
		servos[4][1] += 0.25;
		servos[4][2] += 0.25;
		break;
	case '6':
		// Move Body Forward
		servos[1][0] += 0.25;
		servos[3][0] -= 0.25;
		servos[5][0] -= 0.25;
		break;
	case '7':
		// Move Legs Forward
		servos[0][0] -= 0.25;
		servos[2][0] -= 0.25;
		servos[4][0] += 0.25;
		break;
	case '8':
		// Drop Legs
		servos[0][1] -= 0.25;
		servos[0][2] -= 0.25;
		servos[2][1] -= 0.25;
		servos[2][2] -= 0.25;
		servos[4][1] -= 0.25;
		servos[4][2] -= 0.25;
		break;
	case '9':
		// Move Body Forward
		servos[0][0] += 0.25;
		servos[2][0] += 0.25;
		servos[4][0] -= 0.25;
		break;
	case '0':
		// Move Legs Forward
		servos[1][0] -= 0.25;
		servos[3][0] += 0.25;
		servos[5][0] += 0.25;
		break;
	}

	THETA[4] = servos[0][0]; THETA[7] = servos[1][0]; THETA[10] = servos[2][0]; THETA[13] = servos[3][0]; THETA[16] = servos[4][0]; THETA[1] = servos[5][0];		// base
	THETA[5] = servos[0][1]; THETA[8] = servos[1][1]; THETA[11] = servos[2][1]; THETA[14] = servos[3][1]; THETA[17] = servos[4][1]; THETA[2] = servos[5][1];		// leg
	THETA[6] = servos[0][2]; THETA[9] = servos[1][2]; THETA[12] = servos[2][2]; THETA[15] = servos[3][2]; THETA[18] = servos[4][2]; THETA[3] = servos[5][2];		// foot


	/*
	Leg 1		Leg 2		Leg3		Leg 4		Leg 5		Leg 6
	THETA[4] = THETA[7] = THETA[10] = THETA[13] = THETA[16] = THETA[1];		// base
	THETA[5] = THETA[8] = THETA[11] = THETA[14] = THETA[17] = THETA[2];		// leg
	THETA[6] = THETA[9] = THETA[12] = THETA[15] = THETA[18] = THETA[3];		// foot
	*/
}

// Simulation loop
void simLoop(int pause) 
{
  control();
  
  dSpaceCollide(space,0,&nearCallback);

  dWorldStep(world,0.01);

  dJointGroupEmpty(contactgroup);

  // Draw a robot
  dsSetColor(0.0,0.0,1.0);					// Set color (r, g, b), In this case white is set

  dsDrawCylinderD(dBodyGetPosition(link[0].body), dBodyGetRotation(link[0].body), l[0], r[0]);
  
  for (int i=1; i < NUM; i++)
  {
	  dsDrawCapsuleD(dBodyGetPosition(link[i].body), dBodyGetRotation(link[i].body), l[i], r[i]);
  }


	if(loopcount > 100)
	{
		loopcount = 0;
		switch(loopstep)
		{
		case 1:
			// Lift Legs
			servos[1][1] += 0.50;
			servos[1][2] += 0.50;
			servos[3][1] += 0.50;
			servos[3][2] += 0.50;
			servos[5][1] += 0.50;
			servos[5][2] += 0.50;
			loopstep = 2;
			break;
		case 2:
			// Move Body Forward
			servos[0][0] += 0.25;
			servos[2][0] += 0.25;
			servos[4][0] -= 0.25;
			loopstep = 3;
			break;
		case 3:
			// Move Legs Forward
			servos[1][0] -= 0.25;
			servos[3][0] += 0.25;
			servos[5][0] += 0.25;
			loopstep = 4;
			break;
		case 4:
			// Drop Legs
			servos[1][1] -= 0.50;
			servos[1][2] -= 0.50;
			servos[3][1] -= 0.50;
			servos[3][2] -= 0.50;
			servos[5][1] -= 0.50;
			servos[5][2] -= 0.50;
			loopstep = 5;
			break;
		case 5:
			// Lift Legs
			servos[0][1] += 0.25;
			servos[0][2] += 0.25;
			servos[2][1] += 0.25;
			servos[2][2] += 0.25;
			servos[4][1] += 0.25;
			servos[4][2] += 0.25;
			loopstep = 6;
			break;
		case 6:
			// Move Body Forward
			servos[1][0] += 0.25;
			servos[3][0] -= 0.25;
			servos[5][0] -= 0.25;
			loopstep = 7;
			break;
		case 7:
			// Move Legs Forward
			servos[0][0] -= 0.25;
			servos[2][0] -= 0.25;
			servos[4][0] += 0.25;
			loopstep = 8;
			break;
		case 8:
			// Drop Legs
			servos[0][1] -= 0.25;
			servos[0][2] -= 0.25;
			servos[2][1] -= 0.25;
			servos[2][2] -= 0.25;
			servos[4][1] -= 0.25;
			servos[4][2] -= 0.25;
			loopstep = 1;
			break;
		case 9:
			// Move Body Forward
			servos[0][0] += 0.25;
			servos[2][0] += 0.25;
			servos[4][0] -= 0.25;
			break;
		case 0:
			// Move Legs Forward
			servos[1][0] -= 0.25;
			servos[3][0] += 0.25;
			servos[5][0] += 0.25;
			break;
		}
	}	
	else
	{
		loopcount++;
	}


	THETA[4] = servos[0][0]; THETA[7] = servos[1][0]; THETA[10] = servos[2][0]; THETA[13] = servos[3][0]; THETA[16] = servos[4][0]; THETA[1] = servos[5][0];		// base
	THETA[5] = servos[0][1]; THETA[8] = servos[1][1]; THETA[11] = servos[2][1]; THETA[14] = servos[3][1]; THETA[17] = servos[4][1]; THETA[2] = servos[5][1];		// leg
	THETA[6] = servos[0][2]; THETA[9] = servos[1][2]; THETA[12] = servos[2][2]; THETA[15] = servos[3][2]; THETA[18] = servos[4][2]; THETA[3] = servos[5][2];		// foot

}

int main(int argc, char *argv[]) 
{
  dsFunctions fn;

  dMatrix3 R;

  double x[NUM] = { 0.00,	0.90*cos(0.0), 1.90*cos(0.0), 2.90*cos(0.0),
							0.90*cos(PI/3), 1.90*cos(PI/3), 2.90*cos(PI/3),
							0.90*cos(2*PI/3), 1.90*cos(2*PI/3), 2.90*cos(2*PI/3),
							0.90*cos(PI), 1.90*cos(PI), 2.90*cos(PI),
							0.90*cos(4*PI/3), 1.90*cos(4*PI/3), 2.90*cos(4*PI/3),
							0.90*cos(5*PI/3), 1.90*cos(5*PI/3), 2.90*cos(5*PI/3)		};					// Center of gravity

  double y[NUM] = { 0.00,	0.90*sin(0.0), 1.90*sin(0.0), 2.90*sin(0.0),
							0.90*sin(PI/3), 1.90*sin(PI/3), 2.90*sin(PI/3),
							0.90*sin(2*PI/3), 1.90*sin(2*PI/3), 2.90*sin(2*PI/3),
							0.90*sin(PI), 1.90*sin(PI), 2.90*sin(PI),
							0.90*sin(4*PI/3), 1.90*sin(4*PI/3), 2.90*sin(4*PI/3),
							0.90*sin(5*PI/3), 1.90*sin(5*PI/3), 2.90*sin(5*PI/3)		};			

  double z[NUM]	= { 2.05,	2.02, 2.02, 2.02,
							2.02, 2.02, 2.02,
							2.02, 2.02, 2.02,
							2.02, 2.02, 2.02,
							2.02, 2.02, 2.02,
							2.02, 2.02, 2.02	};


  double m[NUM] = {10.00,	2.00, 2.00, 2.00,
							2.00, 2.00, 2.00,
							2.00, 2.00, 2.00,
							2.00, 2.00, 2.00,
							2.00, 2.00, 2.00,
							2.00, 2.00, 2.00	};					// mass



  double anchor_x[NUM] = { 0.00,	0.40*cos(0.0), 1.40*cos(0.0), 2.40*cos(0.0),
									0.40*cos(PI/3), 1.40*cos(PI/3), 2.40*cos(PI/3),
									0.40*cos(2*PI/3), 1.40*cos(2*PI/3), 2.40*cos(2*PI/3),
									0.40*cos(PI), 1.40*cos(PI), 2.40*cos(PI),
									0.40*cos(4*PI/3), 1.40*cos(4*PI/3), 2.40*cos(4*PI/3),
									0.40*cos(5*PI/3), 1.40*cos(5*PI/3), 2.40*cos(5*PI/3)		};			// anchors of joints

  double anchor_y[NUM] = { 0.00,	0.40*sin(0.0), 1.40*sin(0.0), 2.40*sin(0.0),
									0.40*sin(PI/3), 1.40*sin(PI/3), 2.40*sin(PI/3),
									0.40*sin(2*PI/3), 1.40*sin(2*PI/3), 2.40*sin(2*PI/3),
									0.40*sin(PI), 1.40*sin(PI), 2.40*sin(PI),
									0.40*sin(4*PI/3), 1.40*sin(4*PI/3), 2.40*sin(4*PI/3),
									0.40*sin(5*PI/3), 1.40*sin(5*PI/3), 2.40*sin(5*PI/3)		};			

  double anchor_z[NUM] = { 2.05,	2.02, 2.02, 2.02,
									2.02, 2.02, 2.02,
									2.02, 2.02, 2.02,
									2.02, 2.02, 2.02,
									2.02, 2.02, 2.02,
									2.02, 2.02, 2.02	};


  double axis_x[NUM]  = { 0.00,		0.00, 0.00, 0.00,
									0.00, -sin(PI/3), -sin(PI/3),
									0.00, -sin(PI/3), -sin(PI/3),
									0.00, 0.00, 0.00,
									0.00, sin(PI/3), sin(PI/3),
									0.00, sin(PI/3), sin(PI/3)	};					// axises of joints

  double axis_y[NUM]  = { 0.00,		0.00, 1.00, 1.00,
									0.00, cos(PI/3), cos(PI/3),
									0.00, -cos(PI/3), -cos(PI/3),
									0.00, -1.00, -1.00,
									0.00, -cos(PI/3), -cos(PI/3),
									0.00, cos(PI/3), cos(PI/3),	};

  double axis_z[NUM]  = { 1.00,		1.00, 0.00, 0.00,
									1.00, 0.00, 0.00,
									1.00, 0.00, 0.00,
									1.00, 0.00, 0.00,
									1.00, 0.00, 0.00,
									1.00, 0.00, 0.00	};


  fn.version = DS_VERSION;  fn.start   = &start;   fn.step   = &simLoop;
  fn.command = &command;
  fn.path_to_textures = "../../drawstuff/textures";

  dInitODE();										// Initialize ODE
  world = dWorldCreate();							// Create a world
  space = dHashSpaceCreate(0);						// Create space
  contactgroup = dJointGroupCreate(0);

  dWorldSetGravity(world, 0, 0, -9.81);

  // Create a ground
  ground = dCreatePlane(space,0,0,1,0);

  for (int i = 0; i < NUM; i++) 
  {
    dMass mass;
    link[i].body = dBodyCreate(world);
    dBodySetPosition(link[i].body, x[i], y[i], z[i]);	// Set a position

	if(i > 0)
	{
		dRFromAxisAndAngle(R, 0, 1, 0, 3.141592653 / 2);
		dBodySetRotation(link[i].body, R);
	}

	
	if((i > 3 && i < 7) || (i > 12 && i < 16))
	{
		dRFromEulerAngles(R, PI/2, -PI/6 , 0);
		dBodySetRotation(link[i].body, R);
	}

	if((i > 6 && i < 10) || (i > 15 && i < 19))
	{
		dRFromEulerAngles(R, PI/2, PI/6 , 0);
		dBodySetRotation(link[i].body, R);
	}
	

    dMassSetZero(&mass);							// Set mass parameter to zero
    dMassSetCapsuleTotal(&mass,m[i],3,r[i],l[i]);   // Calculate mass parameter
    dBodySetMass(link[i].body, &mass);				// Set mass
  }






  for (int j = 1; j < NUM; j++) 
  {
	if(j == 1 || j == 4 || j == 7 || j == 10 || j == 13 || j == 16)
	{
		joint[j] = dJointCreateHinge(world, 0);										// Create a hinge joint
		dJointAttach(joint[j], link[0].body, link[j].body);						// Attach the joint
		dJointSetHingeAnchor(joint[j], anchor_x[j], anchor_y[j],anchor_z[j]);
		dJointSetHingeAxis(joint[j], axis_x[j], axis_y[j], axis_z[j]);
	}
	else
	{
		joint[j] = dJointCreateHinge(world, 0);										// Create a hinge joint
		dJointAttach(joint[j], link[j-1].body, link[j].body);						// Attach the joint
		dJointSetHingeAnchor(joint[j], anchor_x[j], anchor_y[j],anchor_z[j]);
		dJointSetHingeAxis(joint[j], axis_x[j], axis_y[j], axis_z[j]);
	}
  }


  for(int i=0; i < NUM; i++)
  {
	link[i].geom = dCreateCylinder(space,r[i],l[i]);
	dGeomSetBody(link[i].geom,link[i].body);
  }

  for (int j = 1; j < NUM; j++) 
  {
	dJointGetHingeAngle(joint[j]);
  }

  dsSimulationLoop(argc, argv, 800, 600, &fn);
  dCloseODE();
  return 0;
}