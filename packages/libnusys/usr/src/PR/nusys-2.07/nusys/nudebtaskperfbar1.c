/*======================================================================*/
/*		NuSYS							*/
/*		nudebtaskperfbar1.c					*/
/*									*/
/*		Copyright (C) 1997, NINTENDO Co,Ltd.			*/
/*									*/
/*======================================================================*/
/* $Id: nudebtaskperfbar1.c,v 1.6 1999/07/09 06:17:24 ohki Exp $	*/
/*======================================================================*/
#ifdef NU_DEBUG
#ifndef	F3DEX_GBI_2
#define	F3DEX_GBI
#endif	/* F3DEX_GBI_2*/
#include <nusys.h>
#define	VFRAME		280
#define	BAR_WIDTH	2

#define X0		1
#define Y0		2

#define X1		(X0+VFRAME*1)
#define X2		(X0+VFRAME*2)
#define X3		(X0+VFRAME*3)
#define X4		(X0+VFRAME*4)
#define X5		(X0+VFRAME*5)
#define X6		(X0+VFRAME*6)
#define X7		(X0+VFRAME*7)

#define BAR_BETWEEN	BAR_WIDTH+1
#define Y1		(Y0+BAR_BETWEEN)
#define	Y2		(Y1+BAR_BETWEEN)
#define	Y3		(Y2+BAR_BETWEEN)
#define	Y4		(Y3+BAR_BETWEEN)
#define	Y5		(Y4+BAR_BETWEEN)
#define	Y6		(Y5+BAR_BETWEEN)

#define	FRAME_X0	(X0-1)
#define	FRAME_X1	(X7+1)
#define	FRAME_Y0	(Y0-2)
#define	FRAME_Y1	(Y6+1)
#define	FRAME_Y2	(Y0-1)

#define MARK_SIZE_X	8
#define MARK_SIZE_Y	12


extern unsigned char nuDebPerfMarker[];
extern unsigned short nuDebPerfMarkertlut[];

/* The vertex list of the frame of the performance bar */
static Vtx perfFrameVtx[] = {
    { FRAME_X0, FRAME_Y0,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    { FRAME_X1, FRAME_Y0,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    { FRAME_X1, FRAME_Y1,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    { FRAME_X0, FRAME_Y1,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0x80},
    
    {   X0, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    {   X1, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    {   X1,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    {   X0,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x40, 0x80},
    
    {   X1, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X2, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X2,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X1,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    
    {   X2, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X3, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X3,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X2,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    
    {   X3, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X4, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X4,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X3,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    
    {   X4, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X5, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X5,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X4,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    
    {   X5, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X6, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X6,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    {   X5,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0xc0},
    
    {   X6, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X7, FRAME_Y2,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X7,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
    {   X6,       Y6,   0,   0,   0,   0, 0x00, 0x00, 0x00, 0x80},
};

/* The vertex list of the audio and task bar */
static Vtx auTaskVtx[] = {
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    { 0, Y5+BAR_WIDTH,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0x00, 0xff, 0xff},
    { 0, Y5          ,  -1,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
};

/* The vertex list of graphic, RSP and task bar */
static Vtx gfxRspTaskVtx[] = {
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y4+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y4          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},

};

/* The vertex list of graphic, RDP and task bar */
static Vtx gfxRdpTaskVtx[] = {
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
    {    0, Y3+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0xff, 0xff, 0xff, 0xff},
    {    0, Y3          ,   0,   0,   0,   0, 0x00, 0xff, 0x00, 0xff},
};
static Vtx gfxRdpCmdVtx[] = {
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
    {    0, Y2+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0xff, 0x00, 0x00, 0xff},
    {    0, Y2          ,   0,   0,   0,   0, 0x00, 0x00, 0xff, 0xff},
};
static Vtx gfxRdpPipeVtx[] = {
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
    {    0, Y1+BAR_WIDTH,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0xff, 0xff, 0x00, 0xff},
    {    0, Y1          ,   0,   0,   0,   0, 0x00, 0xff, 0xff, 0xff},
};
static Vtx gfxRdpTmemVtx[] = {
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
    {    0, Y0+BAR_WIDTH,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0xff, 0x00, 0xff},
    {    0, Y0          ,   0,   0,   0,   0, 0x80, 0x00, 0xff, 0xff},
};
static Vp vp = {
    320*2, 240*2, G_MAXZ/2, 0,	/* scale */
    320*2, 240*2, G_MAXZ/2, 0,	/* translate */
};

static Gfx initRcpDl[] = {
    gsSPSegment(0x0, 0x0),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, NU_GFX_INIT_SCREEN_WD * 2, NU_GFX_INIT_SCREEN_HT * 2),
    gsSPClearGeometryMode(0xffffffff),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPPipelineMode(G_PM_NPRIMITIVE),
    gsDPSetColorDither(G_CD_BAYER),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
#define	G_CC_TEXT	0, 0, 0, PRIMITIVE, 0, 0, 0, TEXEL0

static Gfx markInitDl[] = {
    gsDPPipeSync(),
    gsDPSetTextureFilter(G_TF_POINT),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPSetTextureDetail(G_TD_CLAMP),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsSPTexture(0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPLoadTLUT_pal16(0, nuDebPerfMarkertlut),
    gsDPLoadTextureBlock_4b(nuDebPerfMarker, G_IM_FMT_CI,
			   80, 12,
			   0,
			   G_TX_WRAP , G_TX_WRAP,
			   G_TX_NOMASK, G_TX_NOMASK,
			   G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetRenderMode(G_RM_TEX_EDGE, G_RM_TEX_EDGE),
    gsSPClearGeometryMode(0xffffffff),
    gsDPPipeSync(),
    gsSPEndDisplayList()
};    

/* Draw frames of the performance meter */
static Gfx perfFrameDl[] = {
    gsSPVertex(&perfFrameVtx, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};


/* The scale value in the case of changing the display frmae number of the task bar */
static float frameScale[] = { 1.0, 0.5, 0.3333, 0.25, 0.2, 0.16667};

static Mtx	perf_mtx;
static Mtx	view_mtx;
static Mtx	line_mtx;
static Mtx	scale_mtx;
static Mtx	move_mtx;
static Gfx	glistBuf[2][512];
static u32	glistCnt = 0;

/*----------------------------------------------------------------------*/
/*	Display the performance meter					*/
/*	IN:	glist_ptr	The display list buffer			*/
/*		frameNum	The number of display frames (1-6)	*/
/*		y		The display position			*/
/*	RET:	None							*/
/*----------------------------------------------------------------------*/
void nuDebTaskPerfBar1(u32 frameNum, u32 y, u32 flag)
{


    u32		cnt;
    s64		taskTime;
    short 	StartX;
    short	EndX;
    u32		vtxIdx;
    float	posY;
    Gfx*	glistPtr;
    u32		viMode;
    u32		scale;
    
    glistPtr = glistBuf[glistCnt];
    /* Get current VI mode */
    viMode = osViGetCurrentMode();
    if(viMode < 8){
	scale = 1;
    } else if(viMode < 14){
	y /=2;
	scale = 2;
    }

    vp.vp.vscale[0] = vp.vp.vtrans[0] = 320*2*scale;
    vp.vp.vscale[1] = vp.vp.vtrans[1] = 240*2*scale;
    gSPViewport(glistPtr++, &vp);

    /* Set RSP and RDP */
    gSPDisplayList(glistPtr++, OS_K0_TO_PHYSICAL(initRcpDl));
    gDPSetColorImage(glistPtr++, G_IM_FMT_RGBA, G_IM_SIZ_16b,
		     NU_DEB_CON_WIDTH * scale,
		     osVirtualToPhysical(nuGfxCfb_ptr));

    /* Convert from upper-left because origin of the coordinate is in lower-left */
    posY =(240.0 - FRAME_Y1) - y;
    if(posY == 0.0) posY = 0.1;

     /* Limit the number of display frames, to make sure */
    if(frameNum > 5) frameNum = 5;

    guOrtho(&perf_mtx,
	    0.0F, (float)(NU_GFX_INIT_SCREEN_WD * scale),
	    0.0F, (float)(NU_GFX_INIT_SCREEN_HT * scale),
	    -1.0F, 10.0F, 1.0F);
    guLookAt(&view_mtx,
	     0.0, 0.0, 1.0,
	     0.0, 0.0, 0.0,
	     0.0, 1.0, 0.0);
    
    /* Change number of display frames by changing scale of X-axis direction */
    guScale(&scale_mtx, frameScale[frameNum]*(float)scale, (float)scale, 1.0);

    /* Move position from left-end to 20-dot on screen */
    guTranslate(&move_mtx, 20.0/frameScale[frameNum], posY, 0.0);
    gSPMatrix(glistPtr++, &perf_mtx, 
	    G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistPtr++, &view_mtx,
	      G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    guMtxIdent(&line_mtx);
    gSPMatrix(glistPtr++, OS_K0_TO_PHYSICAL(&line_mtx),
	      G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistPtr++, OS_K0_TO_PHYSICAL(&scale_mtx),
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
    gSPMatrix(glistPtr++, OS_K0_TO_PHYSICAL(&move_mtx),
	      G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    

    /* Draw outside frame of performance bar display */
    gSPDisplayList(glistPtr++, OS_K0_TO_PHYSICAL(perfFrameDl));
    perfFrameVtx[1].v.ob[0] = VFRAME * (frameNum + 1) + 2;
    perfFrameVtx[2].v.ob[0] = VFRAME * (frameNum + 1) + 2;

    /* Draw inside frame for number of display frames */
    for(cnt = 0; cnt < frameNum + 1; cnt++){
	vtxIdx = cnt * 4 + 4;
	gSP2Triangles(glistPtr++, vtxIdx+ 0, vtxIdx+1, vtxIdx+2, 0,
		      vtxIdx+2, vtxIdx+3, vtxIdx+0, 0);
    }

    gSPVertex(glistPtr++, auTaskVtx, 16, 0);
    StartX = X0;
    /* Display audio and task bars */
    for(cnt = 0; cnt < nuDebTaskPerfPtr->auTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	taskTime = nuDebTaskPerfPtr->auTaskTime[cnt].rspEnd
	    - nuDebTaskPerfPtr->auTaskTime[cnt].rspStart;
	EndX	=  (taskTime* VFRAME)/16666;
	auTaskVtx[vtxIdx+0].v.ob[0] = StartX;
	auTaskVtx[vtxIdx+1].v.ob[0] = StartX + EndX;
	auTaskVtx[vtxIdx+2].v.ob[0] = StartX + EndX;
	auTaskVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
	StartX += EndX;
    }

    /* Display graphic, RSP and task bars */
    gSPVertex(glistPtr++, gfxRspTaskVtx, 32, 0);
    StartX = X0;
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	taskTime =  nuDebTaskPerfPtr->gfxTaskTime[cnt].rspEnd
	    - nuDebTaskPerfPtr->gfxTaskTime[cnt].rspStart;
	EndX	=  (taskTime* VFRAME)/16666;
	gfxRspTaskVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRspTaskVtx[vtxIdx+1].v.ob[0] = StartX + EndX;
	gfxRspTaskVtx[vtxIdx+2].v.ob[0] = StartX + EndX;
	gfxRspTaskVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
	StartX += EndX;
    }
    /* Display graphic, RDP and task bars */
    gSPVertex(glistPtr++, gfxRdpTaskVtx, 32, 0);
    StartX = X0;
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	taskTime = nuDebTaskPerfPtr->gfxTaskTime[cnt].rdpEnd
	    - nuDebTaskPerfPtr->gfxTaskTime[cnt].rspStart;
	EndX	=  (taskTime* VFRAME)/16666;
	gfxRdpTaskVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRdpTaskVtx[vtxIdx+1].v.ob[0] = EndX;
	gfxRdpTaskVtx[vtxIdx+2].v.ob[0] = EndX;
	gfxRdpTaskVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
	StartX += EndX;
    }
    /* RDP-CMD Conter	*/
    gSPVertex(glistPtr++, gfxRdpCmdVtx, 32, 0);
    StartX = X0;
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	taskTime = nuDebTaskPerfPtr->gfxTaskTime[cnt].dpCnt[NU_DEB_DP_CMD_CTR];
	taskTime = (taskTime * 1643) / 100000;
	EndX	=  (taskTime*VFRAME)/16666;
	gfxRdpCmdVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRdpCmdVtx[vtxIdx+1].v.ob[0] = EndX;
	gfxRdpCmdVtx[vtxIdx+2].v.ob[0] = EndX;
	gfxRdpCmdVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
	StartX += EndX;
    }
    
    /* RDP-PIPE Conter	*/
    gSPVertex(glistPtr++, gfxRdpPipeVtx, 32, 0);
    StartX = X0;
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	taskTime = nuDebTaskPerfPtr->gfxTaskTime[cnt].dpCnt[NU_DEB_DP_PIPE_CTR];
	taskTime = (taskTime * 1643) / 100000;
	EndX	=  (taskTime*VFRAME)/16666;
	gfxRdpPipeVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRdpPipeVtx[vtxIdx+1].v.ob[0] = EndX;
	gfxRdpPipeVtx[vtxIdx+2].v.ob[0] = EndX;
	gfxRdpPipeVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
	StartX += EndX;
    }
    /* RDP-Tmem Conter	*/
    gSPVertex(glistPtr++, gfxRdpTmemVtx, 32, 0);
    StartX = X0;
    for(cnt = 0; cnt < nuDebTaskPerfPtr->gfxTaskCnt; cnt++){
	vtxIdx = cnt * 4;
	taskTime = nuDebTaskPerfPtr->gfxTaskTime[cnt].dpCnt[NU_DEB_DP_TMEM_CTR];
	taskTime = (taskTime * 1643) / 100000;
	EndX	=  (taskTime*VFRAME)/16666;
	gfxRdpTmemVtx[vtxIdx+0].v.ob[0] = StartX;
	gfxRdpTmemVtx[vtxIdx+1].v.ob[0] = EndX;
	gfxRdpTmemVtx[vtxIdx+2].v.ob[0] = EndX;
	gfxRdpTmemVtx[vtxIdx+3].v.ob[0] = StartX;
	gSP2Triangles(glistPtr++, vtxIdx+0, vtxIdx+2, vtxIdx+1, 0,
		      vtxIdx+0, vtxIdx+3, vtxIdx+2, 0);
	StartX += EndX;
    }
    
    gSPDisplayList(glistPtr++, markInitDl);

    for(cnt = 0; cnt < NU_DEB_MARKER_NUM; cnt ++){
	if(nuDebTaskPerfPtr->markerTime[cnt] > 0){
	    s64		StartTime;
	    s64		retraceTime;
	    
	    retraceTime = nuDebTaskPerfPtr->retraceTime;
	    StartTime = nuDebTaskPerfPtr->markerTime[cnt] - retraceTime;
	    StartX = ((StartTime * VFRAME)/16666) * frameScale[frameNum]
		  + 20 ;
	    StartX *= scale;
	    EndX = StartX + MARK_SIZE_X * scale;
	    gSPTextureRectangle(glistPtr++,
				StartX << 2, (y  - MARK_SIZE_Y * scale) << 2,
				EndX << 2, y  << 2,
				G_TX_RENDERTILE,
				(cnt * MARK_SIZE_X) <<5, 0,
				1<<(10 - (scale - 1)), 1<<(10 - (scale - 1)));
	}
    }

    gDPFullSync(glistPtr++);
    gSPEndDisplayList(glistPtr++);
    
#ifdef NU_DEBUG    
    if((u32)(glistPtr - glistBuf[glistCnt]) > 256){
	osSyncPrintf("nuDebTaskPerfBar0: gfx list buffer over.\n");
    }
#endif /* NU_DEBUG */
    
    /* Start graphic task */
    nuGfxTaskStart(glistBuf[glistCnt],
		  (s32)(glistPtr - glistBuf[glistCnt]) * sizeof(Gfx),
		   NU_GFX_UCODE_F3DEX , flag);
    glistCnt++;
    glistCnt &= 0x01;
}
#endif /* NU_DEBUG */
