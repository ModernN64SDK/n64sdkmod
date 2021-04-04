/*
 *  walk_around.c - implements simple motion model
 *
 *  The joystick is for turning left, right and moving back and forth. 
 *  Up and down on the crosshair tilts the view up and down; left and 
 *  right on the crosshair 'sidestep' to the left and right. The six 
 *  'fire buttons' accomplish the same suite of movements.
 */
#include <ultra64.h>

/*
 * local includes: 
 */
#include "app.h"
#include "walk_around.h"
#include "vector.h"
#include "matrix.h"

/*
 * The slop factor is the amount of slop we allow in the joystick
 * before registering a movement.
 */
#define SLOP_FACTOR 2

/*
 *  Globals
 */
int MyMode = 0;

/*
 *  Controller globals
 */
static OSMesgQueue contMessageQ;
static OSMesg   dummyMessage;
static OSContStatus statusdata[MAXCONTROLLERS];
static OSContPad controllerdata[MAXCONTROLLERS];
static int      controller;

static int      firstcall = 1;

static int      lastx;
static int      lasty;
static int      lastbutton = 0;
static int      press = 0;
static int      press_up = 0;
static int      press_down = 0;

/*
 *  empirically determined
 */
#define deltarot		0.0001f
#define deltatrans		0.1f
#define stickdeltarot	0.0002f
#define stickdeltarotyaw 0.0001f
#define stickdeltatrans	0.05f

/*
 * User's eye in world coords
 */
static vec3     eyept;

/*
 * Used to generate incRot Mtx
 */
static float    phi;

/*
 * Used to generate incRot Mtx
 */
static float    axis[3];

/*
 *  translation increments
 */
static float    dx = 0,
                dy = 0,
                dz = 0;

/*
 * rotation increments
 */
static float    rx = 0,
                ry = 0,
                rz = 0;

/*
 * incremental rotation matrix
 */
static Mtx      incRot;

/*
 * incremental translation matrix
 */
static Mtx      incTrans;
static Mtx      incMat;

/*
 * viewing matrix and inverse
 */
static FMatrix  viewMat;

/*
static FMatrix  invMat;
*/

/*
 * last frame's viewing mat
 */
static Mtx      lastViewMat;

/*
 *  user's initial position
 */
static float    initial_pos[6] =
{0, 0, 200, 0, 0, 0};

/*
 *
 * Return the lowest number controller connected to system
 */
int
walkAroundInit(float x, float y, float z, float xa, float ya, float za)
{
	int             i;
	u8		pattern;
	
	initial_pos[0] = x;
	initial_pos[1] = y;
	initial_pos[2] = z;
	initial_pos[3] = xa;
	initial_pos[4] = ya;
	initial_pos[5] = za;

	osCreateMesgQueue(&contMessageQ, &dummyMessage, 1);
	osSetEventMesg(OS_EVENT_SI, &contMessageQ, (OSMesg) 0);

	osContInit(&contMessageQ, &pattern, &statusdata[0]);


	for (i = 0; i < MAXCONTROLLERS; i++) {
		if ((pattern & (1 << i)) &&
			!(statusdata[i].errno & CONT_NO_RESPONSE_ERROR)) {
			osContStartReadData(&contMessageQ);
			controller = i;
			return i;
		}
	}
	controller = -1;
	return -1;
}

/*
 *  Update controller info
 */
static void
readControllers(void)
{
	OSContPad      *pad;

	if (osRecvMesg(&contMessageQ, &dummyMessage, OS_MESG_NOBLOCK) == 0) {
		osContGetReadData(controllerdata);
		osContStartReadData(&contMessageQ);
	}

	pad = &controllerdata[controller];

	press = lastbutton ^ pad->button;
	press_up = lastbutton & press;
	press_down = pad->button & press;

	lastbutton = pad->button;
	lastx = pad->stick_x;
	lasty = pad->stick_y;
}

/*
 *  implement 'walk around' motion model
 *
 * Now map the controller to movements. The joystick is for
 * turning left, right and moving back and forth. Up and down on the
 * crosshair tilts the view up and down; left and right on the crosshair
 * 'sidestep' to the left and right. 
 *
 */
void
walkAround(Dynamic *dynamicp, int *up, int *down)
{

	if(controller >= 0)
		readControllers();

	/*
	 * The 'Start' button stop  the camera.
	 */
	if(firstcall) {
		eyept[0] = initial_pos[0];
		eyept[1] = initial_pos[1];
		eyept[2] = initial_pos[2];
		guLookAt(&lastViewMat, initial_pos[0], initial_pos[1], initial_pos[2],
		initial_pos[3], initial_pos[4], initial_pos[5], 0, 1, 0);
		firstcall = 0;
	}
	if (press_down & CONT_START) {
		dx = dy = dz = 0.0;
		rx = ry = rz = 0.0;
	}
	if (lastx > SLOP_FACTOR) {
		rz = (float) (lastx - SLOP_FACTOR) * stickdeltarot;
	} else if (lastx < -SLOP_FACTOR) {
		rz = (float) (lastx + SLOP_FACTOR) * stickdeltarot;
	}

	if (lasty > SLOP_FACTOR) {
		rx = (float)(lasty - SLOP_FACTOR) * stickdeltarot;
	} else if (lasty < -SLOP_FACTOR) {
	 	rx = (float)(lasty + SLOP_FACTOR) * stickdeltarot;
	}
	if (lastbutton & CONT_UP) {
		dz += deltatrans;
	}
	if (lastbutton & CONT_DOWN) {
		dz -= deltatrans;
	} 
	if (lastbutton & CONT_LEFT) {
		ry -= deltarot;
	} 
	if (lastbutton & CONT_RIGHT) {
		ry += deltarot;
	} 
	if (press_down & CONT_A) {
	}
	if (press_down & CONT_B) {
	}
	if (press_down & CONT_C) {
	}
	if (press_down & CONT_D) {
	}
	if (press_down & CONT_E) {
	}
	if (press_down & CONT_L) {
		MyMode++;
		if(MyMode > 5)
			MyMode = 0;
	}
	if (press_down & CONT_R) {
		MyMode--;
		if(MyMode < 0)
			MyMode = 5;
	}
	phi = sqrtf(rx*rx + ry*ry + rz*rz);

	if (phi == 0) {
		makeIdentMtx(&incRot);	
	} else {
		axis[0] = rx / phi;
		axis[1] = ry / phi;
		axis[2] = rz / phi;
		NORMALIZE_VEC3(axis);
		guRotate(&incRot, phi * 360.0 / 6.283,
				 axis[0], axis[1], axis[2]);
	}

	guTranslate(&incTrans, dx, dy, dz);
	multMtx(&incMat, &incRot, &incTrans);
	multMtx(&dynamicp->viewing, &lastViewMat, &incMat);
	copyMtx(&lastViewMat, &dynamicp->viewing);

	/*
	 * obtain eye point in world space 
	 */
	guMtxL2F(viewMat, &dynamicp->viewing);

	*up = press_up; *down = press_down;

/*
 * floatMatInvert(invMat, viewMat);
 * eyept[0] = invMat[3][0];
 * eyept[1] = invMat[3][1];
 * eyept[2] = invMat[3][2];
 */
}
