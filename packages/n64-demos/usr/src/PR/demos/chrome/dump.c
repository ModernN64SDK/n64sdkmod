/**************************************************************************
 *                                                                        *
 *               Copyright (C) 1994, Silicon Graphics, Inc.               *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright  law.  They  may not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *                                                                        *
 *************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: dump.c,v $
        $Revision: 1.6 $
        $Date: 1998/12/24 06:42:18 $
 *---------------------------------------------------------------------*/

/*
 * !! deleted !!
 * File:	dump.c
 * Creator:     kluster@sgi.com
 * Create Date: Tue May 16 16:25:11 PDT 1995
 * 
 * Pretty much ripped off from mbi2read.c:
 * Creator:	hsa@sgi.com
 * Create Date:	Mon Jul 18 14:57:14 PDT 1994
 *
 * This library enables Ultra 64 hardware applications
 * to dump their rsp/rdp display lists or memory contents
 *
 */

#include <ultra64.h>
#include <PR/verify.h>

typedef int			bool;

#define TASKSZ32_MAX	4096

#ifndef TRUE
#   define TRUE		1
#endif

#ifndef FALSE
#   define FALSE	0
#endif

u8 *DRAM;

u32	segment_base[16];

static bool	verbose = TRUE, do_nested = TRUE, exp_matrix = FALSE;
static bool	exp_mp = FALSE, row_major=FALSE, use_verifyinfo=FALSE;
static int	dl_depth=0;

static void	read_dma(u32 *bufp);
static void	read_imm(u32 *bufp);
static void	read_rdp(u32 *bufp);

static void     dump_shade_coef(void);
static void     dump_txtr_coef(void);
static void     dump_zbuff_coef(void);


/*
 * Takes a pointer in DRAM, and a length in bytes. Parses
 * the display list there, until length bytes is consumed or
 * a G_ENDDL command is found.
 */
void parse_DL(u32 *gfxp)
{
    u32		readBuf[4];
    u8		op;
    int		docont = 1;

    DRAM = (u8 *) 0x80000000;

    while (docont) {

	op = (u8) ((*gfxp & 0xff000000) >> 24);
	bcopy((char *) gfxp, (char *) &(readBuf[0]), sizeof(Gfx));

	switch (op & 0xc0) {
	    
	  case (u8) 0x00:
	    osSyncPrintf("\tDMA : ");
	    read_dma(readBuf);  
	    break;

	  case (u8) 0x80:
	    osSyncPrintf("\tIMM : ");
	    read_imm(readBuf); 

	    if (op == (u8) G_ENDDL ) {
		docont = 0;
	    }
	    break;

	  case (u8) 0xc0:
	    osSyncPrintf("\tRDP : ");
	    read_rdp(readBuf);
	    break;

	  default:
	    osSyncPrintf("ERROR : display list is lost, op = %02x\n",op);
	    break;

	}

	osSyncPrintf("\n");
	gfxp += (sizeof(Gfx) >> 2);
    }

    osSyncPrintf("\n\n=============== DL Pop (%d)=============\n",
						dl_depth--);
}

/*
 * showmatrix
 *  print matrix out in nice format
 */
#define MAXPMAT 2
#define MAXMMAT 10
static float modelcurrent[16]={1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0};
static float projectioncurrent[16]={1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0};
static float modelstack[MAXMMAT][16];
static float projectionstack[MAXPMAT][16];
static int modelptr=-1,projectionptr=-1;
static char *mtype[2] = { "modeling","projection" };
static char *mop[2] = { "multiply","load" };
static char *mpush[2] = { "nopush","push" };
void showmatrix(float *mtx,char b0)
{
	int i,j;
	float (*mcurrent)[16],(*mstack)[][16];
	int *mptr;
	float result[16];

	osSyncPrintf("\t\tparam = %s  %s  %s\n",mtype[b0&1],mop[(b0>>1)&1],
		   mpush[(b0>>2)&1]);
	for (i=0; i<4; i++) {
	    osSyncPrintf("\t");
	for (j=0; j<4; j++) {
		if (row_major)
		osSyncPrintf("%12.5f ",mtx[i + (j*4)]);
		else
		osSyncPrintf("%12.5f ",mtx[j + (i*4)]);
	}
	    osSyncPrintf("\n");
	}
	
	/* 
	 * PUSH MATRIX ON STACK?
	 */
	if (b0 & 4) {
		mcurrent= (b0 & 1) ? &projectioncurrent : &modelcurrent;
		mstack= (b0 & 1) ? 
				(float (*)[][16]) &projectionstack : 
				(float (*)[][16]) &modelstack;
		mptr= (b0 & 1) ? &projectionptr : &modelptr;

		if ((*mptr)++ >= ((b0 & 1) ? MAXPMAT : MAXMMAT)) {
		osSyncPrintf("WARNING!! too many %s matrices on stack\n",
				mtype[b0&1]);
			(*mptr)--;
			return;
		}
		for (i=0; i<16; i++) (*mstack)[*mptr][i] = (*mcurrent)[i];
	}

	/*
	 * MULTPIPLY MATRIX WITH CURRENT?
	 */
	mcurrent= (b0 & 1) ? &projectioncurrent : &modelcurrent;
	if (!(b0 & 2)) {
		for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			result[i + (j*4)] = 
				(*mcurrent)[i+(0*4)] * (mtx)[0+(j*4)] +
				(*mcurrent)[i+(1*4)] * (mtx)[1+(j*4)] +
				(*mcurrent)[i+(2*4)] * (mtx)[2+(j*4)] +
				(*mcurrent)[i+(3*4)] * (mtx)[3+(j*4)];
		}
		}
		for (i=0; i<16;i++) (*mcurrent)[i]=result[i];
	} else {
		for (i=0; i<16;i++) (*mcurrent)[i]=mtx[i];
	}

	for (i=0;i<4;i++) {
	for (j=0;j<4;j++) {
		result[i + (j*4)] = 
			projectioncurrent[i+(0*4)] * modelcurrent[0+(j*4)] +
			projectioncurrent[i+(1*4)] * modelcurrent[1+(j*4)] +
			projectioncurrent[i+(2*4)] * modelcurrent[2+(j*4)] +
			projectioncurrent[i+(3*4)] * modelcurrent[3+(j*4)];
	}
	}

	if (exp_mp) {
	osSyncPrintf("\t\tPROJECTION current:\n");
		for (i=0; i<4; i++) {
	osSyncPrintf("\t");
		for (j=0; j<4; j++) {
			if (row_major)
			osSyncPrintf("%12.5f ",projectioncurrent[i + (j*4)]);
			else
			osSyncPrintf("%12.5f ",projectioncurrent[j + (i*4)]);
		}
	osSyncPrintf("\n");
		}
	
	osSyncPrintf("\t\tMODEL current:\n");
		for (i=0; i<4; i++) {
	osSyncPrintf("\t");
		for (j=0; j<4; j++) {
			if (row_major)
			osSyncPrintf("%12.5f ",modelcurrent[i + (j*4)]);
			else
			osSyncPrintf("%12.5f ",modelcurrent[j + (i*4)]);
		}
	osSyncPrintf("\n");
		}
	}

	osSyncPrintf("\t\tCONCATENATED:\n");
	for (i=0; i<4; i++) {
	    osSyncPrintf("\t");
	for (j=0; j<4; j++) {
		if (row_major)
		osSyncPrintf("%12.5f ",result[i + (j*4)]);
		else
		osSyncPrintf("%12.5f ",result[j + (i*4)]);
	}
	    osSyncPrintf("\n");
	}


}

void popmatrix(int b0)
{
	int i,j;
	float (*mcurrent)[16],(*mstack)[][16];
	int *mptr;
	float result[16];

	mcurrent= (b0 & 1) ? &projectioncurrent : &modelcurrent;
	mstack= (b0 & 1) ? 
			(float (*)[][16]) &projectionstack : 
			(float (*)[][16]) &modelstack;
	mptr= (b0 & 1) ? &projectionptr : &modelptr;

	osSyncPrintf("\t\t Popping %s stack\n",mtype[b0&1]);
	if ((*mptr) < 0) {
	osSyncPrintf("WARNING!! Popped too many matrices off the %s stack\n",
			mtype[b0&1]);
		return;
	}
	for (i=0; i<16; i++) (*mcurrent)[i] = (*mstack)[*mptr][i];
	(*mptr)--;

	osSyncPrintf("\t\tPopped:\n");
	for (i=0; i<4; i++) {
	    osSyncPrintf("\t");
	for (j=0; j<4; j++) {
		if (row_major)
		osSyncPrintf("%12.5f ",(*mcurrent)[i + (j*4)]);
		else
		osSyncPrintf("%12.5f ",(*mcurrent)[j + (i*4)]);
	}
	    osSyncPrintf("\n");
	}

	for (i=0;i<4;i++) {
	for (j=0;j<4;j++) {
		result[i + (j*4)] = 
			projectioncurrent[i+(0*4)] * modelcurrent[0+(j*4)] +
			projectioncurrent[i+(1*4)] * modelcurrent[1+(j*4)] +
			projectioncurrent[i+(2*4)] * modelcurrent[2+(j*4)] +
			projectioncurrent[i+(3*4)] * modelcurrent[3+(j*4)];
	}
	}

	if (exp_mp) {
	osSyncPrintf("\t\tPROJECTION current:\n");
		for (i=0; i<4; i++) {
	osSyncPrintf("\t");
		for (j=0; j<4; j++) {
			if (row_major)
			osSyncPrintf("%12.5f ",projectioncurrent[i + (j*4)]);
			else
			osSyncPrintf("%12.5f ",projectioncurrent[j + (i*4)]);
		}
	osSyncPrintf("\n");
		}
	
	osSyncPrintf("\t\tMODEL current:\n");
		for (i=0; i<4; i++) {
	osSyncPrintf("\t");
		for (j=0; j<4; j++) {
			if (row_major)
			osSyncPrintf("%12.5f ",modelcurrent[i + (j*4)]);
			else
			osSyncPrintf("%12.5f ",modelcurrent[j + (i*4)]);
		}
	osSyncPrintf("\n");
		}
	}

	osSyncPrintf("\t\tCONCATENATED:\n");
	for (i=0; i<4; i++) {
	    osSyncPrintf("\t");
	for (j=0; j<4; j++) {
		if (row_major)
		osSyncPrintf("%12.5f ",result[i + (j*4)]);
		else
		osSyncPrintf("%12.5f ",result[j + (i*4)]);
	}
	    osSyncPrintf("\n");
	}

}

float numprint(int num, int s, int i, int f)
{
	s = (s>=0) ? (num & (1<<s)) : 0;
	s = s ? -1 : 1;
	num = num & ((1<<(i+f))-1);
	return (float) s * ((float) num / ((float) (1<<(f))));
}

/*
 * Parse and dump a DMA-type display list command.
 */
static void
read_dma(u32 *bufp)
{
    u32		addr, *data, *gfxp;
    u8		seg_id;
    u16		len;
    char	op, b0, b1;
    float	mtx[16];
    int		i;
    Vtx_t	*vp;
    int		index,movemem[4];
    float	f;

    op = ((*bufp & 0xff000000) >> 24);

    /* DMA op addressing is all the same: */
    seg_id = (u8) ((bufp[1] & 0x0f000000) >> 24);
    addr = segment_base[seg_id] + (bufp[1] & 0x00ffffff);
    len = (u16) (bufp[0] & 0x0000ffff);

    switch (op) {

      case (char) G_SPNOOP:
	osSyncPrintf("\tSPNOOP ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_MTX:
	osSyncPrintf("\tMTX ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	b0 = (char) ((bufp[0] & 0x00ff0000) >> 16);
	osSyncPrintf("\t\tparam = %02x len = %d ptr = %08x\n", b0, len, addr);
	if (verbose) {
	    data = (u32 *) &(DRAM[addr]);
	    i=0;
	    while (len > 0) {
		osSyncPrintf("\t\t\t%08x\n",*data);
		mtx[i]=((float)*data);
		if (*data & 0x80000000) 
			mtx[i] = -((float) ((-(*data))&0x7FFFFFFF));
                mtx[i++] /= 65536.0;
		data++;
		len -= 4;
	    }
	    if (exp_matrix) showmatrix(mtx,b0);
	}
	break;

      case (char) G_VIEWPORT:
	b0 = (char) ((bufp[0] & 0x00f00000) >> 20);
	b1 = (char) ((bufp[0] & 0x000f0000) >> 16);;
	index = (char) ((bufp[0] & 0x00ff0000) >> 16);
	if (verbose) {
	    data = (u32 *) &(DRAM[addr]);
	    for(i=0; i<4; i++) {
		movemem[i]=*data;
		data++;
	    }
	}
	switch(index) {
	  case G_MV_VIEWPORT:
		osSyncPrintf("\tVIEWPORT ");
		osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
		osSyncPrintf("\t\tlen = %d ptr = %08x\n",len, addr);
		if (len != 16) osSyncPrintf(
		     "ERROR : length %d invalid for VIEWPORT command.\n",len);
		if (verbose) {
		f=numprint((movemem[0]>>16)&0xffff,15,13,2);
		osSyncPrintf("\t\t\tX scale = %.2f",f);
		f=numprint((movemem[2]>>16)&0xffff,15,13,2);
		osSyncPrintf("\tX translate = %.2f\n",f);
		f=numprint((movemem[0])&0xffff,15,13,2);
		osSyncPrintf("\t\t\tY scale = %.2f",f);
		f=numprint((movemem[2])&0xffff,15,13,2);
		osSyncPrintf("\tY translate = %.2f\n",f);
		f=numprint((movemem[1]>>16)&0xffff,15,13,2);
		osSyncPrintf("\t\t\tZ scale = %.2f",f);
		f=numprint((movemem[3]>>16)&0xffff,15,13,2);
		osSyncPrintf("\tZ translate = %.2f\n",f);
		}
		break;
	  case G_MV_LOOKATY:
		osSyncPrintf("\tLOOKAT Y  ");
		osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
		osSyncPrintf("\t\tlen = %d ptr = %08x\n",
				len, addr);
		if (len != 16) osSyncPrintf(
		     "ERROR : length %d invalid for LOOKATY command.\n",len);
		if (movemem[0] != 0x00800000 || movemem[1] != 0x00800000) 
		   osSyncPrintf(
		   "ERROR : first 2 LOOKAT_X words must be 0x00800000 [0x%08X 0x%08X]\n",
			movemem[0],movemem[1]);
		if (verbose) {
		f=numprint((movemem[2]>>24)&0xff,7,0,7);
		osSyncPrintf("\t\t\tx = %.3f",f);
		f=numprint((movemem[2]>>16)&0xff,7,0,7);
		osSyncPrintf("\ty = %.3f",f);
		f=numprint((movemem[2]>>8)&0xff,7,0,7);
		osSyncPrintf("\tz = %.3f\n",f);
		}
		break;
	  case G_MV_LOOKATX:
		osSyncPrintf("\tLOOKAT X  ");
		osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
		osSyncPrintf("\t\tlen = %d ptr = %08x\n",
				len, addr);
		if (len != 16) osSyncPrintf(
		     "ERROR : length %d invalid for LOOKATX command.\n",len);
		if (movemem[0] != 0 || movemem[1] != 0) osSyncPrintf(
		    "ERROR : first 2 LOOKAT_X words must be 0x00000000 [0x%08X 0x%08X]\n",
			movemem[0],movemem[1]);
		if (verbose) {
		f=numprint((movemem[2]>>24)&0xff,7,0,7);
		osSyncPrintf("\t\t\tx = %.3f",f);
		f=numprint((movemem[2]>>16)&0xff,7,0,7);
		osSyncPrintf("\ty = %.3f",f);
		f=numprint((movemem[2]>>8)&0xff,7,0,7);
		osSyncPrintf("\tz = %.3f\n",f);
		}
		break;
	  case G_MV_NUMLIGHT:
		osSyncPrintf("\tNUM_LIGHTS");
		osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
		osSyncPrintf("\t\tlen = %d ptr = %08x\n",
				len, addr);
		if (len != 8) osSyncPrintf(
		     "ERROR : length %d invalid for NUM_LIGHTS command.\n",len);
		if (verbose) {
		f=numprint((movemem[0]>>24)&0xff,-1,8,0);
		if ((movemem[0]&0xffff)/32-1 == 1) 
		   osSyncPrintf("\t\t\tLight 1 will be on.  ");
		else
		   osSyncPrintf("\t\t\tLights 1-%d will be on.  ",
			(movemem[0]&0xffff)/32-1);
		osSyncPrintf("Light %d is the ambient light.\n",
			(movemem[0]&0xffff)/32);
		}
		break;
	  case G_MV_L0:
	  case G_MV_L1:
	  case G_MV_L2:
	  case G_MV_L3:
	  case G_MV_L4:
	  case G_MV_L5:
	  case G_MV_L6:
	  case G_MV_L7:
		osSyncPrintf("\tLIGHT    ");
		osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
		osSyncPrintf("\t\tlen = %d ptr = %08x light #%d\n",
				len, addr,(index-G_MV_L0)/2+1);
		if (len != 16) osSyncPrintf(
		     "ERROR : length %d invalid for LIGHT command.\n",len);
		if ((movemem[0]&0xFFFFFF00) != (movemem[1]&0xFFFFFF00)) 
		    osSyncPrintf(
		        "ERROR : colors do not match: 0x%08X != 0x%08X\n",
			movemem[0],movemem[1]);
		if (verbose) {
		f=numprint((movemem[0]>>24)&0xff,-1,8,0);
		osSyncPrintf("\t\t\tred = %.0f  ",f);
		f=numprint((movemem[0]>>16)&0xff,-1,8,0);
		osSyncPrintf("\tgreen = %.0f",f);
		f=numprint((movemem[0]>>8)&0xff,-1,8,0);
		osSyncPrintf("\tblue = %.0f\n",f);

		f=numprint((movemem[2]>>24)&0xff,7,0,7);
		osSyncPrintf("\t\t\tx = %.3f",f);
		f=numprint((movemem[2]>>16)&0xff,7,0,7);
		osSyncPrintf("\ty = %.3f",f);
		f=numprint((movemem[2]>>8)&0xff,7,0,7);
		osSyncPrintf("\tz = %.3f\n",f);
		}
		break;
	  default:
		osSyncPrintf(
			"ERROR : invalid MOVEMEM command: 0x%02X\n",index);
		break;
	}
	break;

      case (char) G_VTX:
	osSyncPrintf("\tVTX ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	b0 = (char) ((bufp[0] & 0x00f00000) >> 20);
	b1 = (char) ((bufp[0] & 0x000f0000) >> 16);;
	osSyncPrintf("\t\tn = %d v0 = %d len = %d ptr = %08x\n", 
		b0, b1, len, addr);
	if (verbose) {
	    data = (u32 *) &(DRAM[addr]);
	    while (len > 0) {

		vp = (Vtx_t *) data;
		osSyncPrintf("\t\t\t%08x ===(pos:  %d %d %d)\n",
			data[0],vp->ob[0],vp->ob[1],vp->ob[2]);
		osSyncPrintf("\t\t\t%08x\n",data[1]);
		osSyncPrintf("\t\t\t%08x    (txtr: %04hx %04hx)\n",
			data[2],vp->tc[0],vp->tc[1]);
		osSyncPrintf("\t\t\t%08x    (colr: %02x %02x %02x %02x)\n",
			data[3],vp->cn[0],vp->cn[1],vp->cn[2],vp->cn[3]);

		data += 4;
		len -= 16;
	    }
	}
	break;

      case (char) G_DL:
	osSyncPrintf("\tDL ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	b0 = (char) ((bufp[0] & 0x00ff0000) >> 16);
	osSyncPrintf("\t\tparam = %02x len = %d ptr = %08x\n", b0, len, addr);
	gfxp = (u32 *) &(DRAM[addr]);
	osSyncPrintf("\n=============== DL Push (%d)=============\n\n",
						++dl_depth);
	if (do_nested)
	    parse_DL(gfxp);
	else
	    osSyncPrintf("\n\n=============== DL Pop (%d)=============\n",
						dl_depth--);
	break;

      default:
	osSyncPrintf("\tUNKNOWN DMA %02x ",op);
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;
    }
}

/*
 * Parse and dump a IMM-type display list command.
 */
static void
read_imm(u32 *bufp)
{
    char	op, b0, b1, b2, b3;
    int		i0, i1;

    op = ((*bufp & 0xff000000) >> 24);

    switch (op) {

      case (char) G_TRI1:
	osSyncPrintf("\tTRI1 ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	b0 = (char) ((bufp[1] & 0xff000000) >> 24);
	b1 = (char) ((bufp[1] & 0x00ff0000) >> 16);
	b2 = (char) ((bufp[1] & 0x0000ff00) >>  8);
	b3 = (char) ((bufp[1] & 0x000000ff));
	osSyncPrintf("\t\tflag = %02x v0 = %d v1 = %d v2 = %d\n", 
		b0, b1, b2, b3);
	break;

      case (char) G_POPMTX:
	osSyncPrintf("\tPOPMTX ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	b0 = (char) (bufp[1] & 0x000000ff);
	osSyncPrintf("\t\tparam = %d \n", b0);
	if (exp_matrix) popmatrix(b0);
	break;

      case (char) G_SEGMENT:
	osSyncPrintf("\tSEGMENT ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	b0 = (char) ((bufp[1] & 0x0f000000) >> 24);
	segment_base[b0] = bufp[1] & 0x00ffffff;
	osSyncPrintf("\t\tid = %d base = %08x\n", b0, segment_base[b0]);
	break;

      case (char) G_TEXTURE:
	osSyncPrintf("\tTEXTURE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int) (bufp[0] & 0x0000ffff);
	i1 = (int) ((bufp[1] & 0xffff0000) >> 16);
	b0 = (char) ((bufp[1] & 0x0000ff00) >> 8);
	b1 = (char) (bufp[1] & 0x000000ff);
	osSyncPrintf("\t\tnum = %d on = %d, s-scale = %04x t-scale = %04x\n",
		b0, b1, i0, i1);
	break;

      case (char) G_SETOTHERMODE_H:
	osSyncPrintf("\tSETOTHERMODE_H ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	osSyncPrintf("\t\tw0 = %08x w1 = %08x\n", 
		(bufp[0] & 0x00ffffff), bufp[1]);
	break;

      case (char) G_SETOTHERMODE_L:
	osSyncPrintf("\tSETOTHERMODE_L ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	osSyncPrintf("\t\tw0 = %08x w1 = %08x\n", 
		(bufp[0] & 0x00ffffff), bufp[1]);
	break;

      case (char) G_RDPHALF_1:
	osSyncPrintf("\tRDPHALF_1 ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	osSyncPrintf("\t\tS=%.2f  T=%.2f\n", 
		(0x80000000&bufp[1])?((float)((bufp[1]>>16)&0x7fff)/-32.0):
					((float)((bufp[1]>>16)&0x7fff)/32.0),
		(0x8000&bufp[1])?((float)((bufp[1])&0x7fff)/-32.0):
					((float)((bufp[1])&0x7fff)/32.0));
	break;

      case (char) G_RDPHALF_2:
	osSyncPrintf("\tRDPHALF_2 ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	osSyncPrintf("\t\tDsDx=%.4f  DtDy=%.4f\n", 
		(0x80000000&bufp[1])?((float)((bufp[1]>>16)&0x7fff)/-1024.0):
					((float)((bufp[1]>>16)&0x7fff)/1024.0),
		(0x8000&bufp[1])?((float)((bufp[1])&0x7fff)/-1024.0):
					((float)((bufp[1])&0x7fff)/1024.0));
	break;

/*      case (char) 0xb3:	/* hack for now... */
      case (char) G_ENDDL:
	osSyncPrintf("\tENDDL ");
	osSyncPrintf("\t\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETGEOMETRYMODE:
	osSyncPrintf("\tSETGEOMETRYMODE ");
	osSyncPrintf("\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	osSyncPrintf("\t\tmode = %04x \n", (bufp[1] & 0x0000ffff));
	break;

      case (char) G_CLEARGEOMETRYMODE:
	osSyncPrintf("\tCLEARGEOMETRYMODE ");
	osSyncPrintf("\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	osSyncPrintf("\t\tmode = %04x \n", (bufp[1] & 0x0000ffff));
	break;

      case (char) G_PERSPNORMALIZE:
	osSyncPrintf("\tPERSPNORMALIZE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      default:
	osSyncPrintf("\tUNKNOWN IMM %02x ",op);
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;
    }
}

/*
 * Parse and dump a RDP-type display list command.
 */
static void
read_rdp(u32 *bufp)
{
    u8		seg_id;
    u32		addr;
    int		i0, i1, i2, i3;
    char	op, b0;

    op = ((*bufp & 0xff000000) >> 24);

    switch (op) {

      case (char) G_NOOP:
	osSyncPrintf("\tNOOP ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETCIMG:
	osSyncPrintf("\tSETCIMG ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int) ((bufp[0] & 0x00fff000) >> 12);
	i1 = (int) ((bufp[0] & 0x00000fff));
	b0 = (char)  ((bufp[0] & 0xfc000000) >> 26);
	seg_id = (u8) ((bufp[1] & 0x0f000000) >> 24);
	addr = segment_base[seg_id] + (bufp[1] & 0x00ffffff);
	osSyncPrintf("\t\tw = %d h = %d t = %d ptr = %08x\n", 
		i0, i1, b0, addr);
	break;

      case (char) G_SETMIMG:
	osSyncPrintf("\tSETMIMG ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int) ((bufp[0] & 0x00fff000) >> 12);
	i1 = (int) ((bufp[0] & 0x00000fff));
	b0 = (char)  ((bufp[0] & 0xfc000000) >> 26);
	seg_id = (u8) ((bufp[1] & 0x0f000000) >> 24);
	addr = segment_base[seg_id] + (bufp[1] & 0x00ffffff);
	osSyncPrintf("\t\tw = %d h = %d t = %d ptr = %08x\n", 
		i0, i1, b0, addr);
	break;

      case (char) G_SETTIMG:
	osSyncPrintf("\tSETTIMG ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int) ((bufp[0] & 0x00fff000) >> 12);
	i1 = (int) ((bufp[0] & 0x00000fff));
	b0 = (char)  ((bufp[0] & 0xfc000000) >> 26);
	seg_id = (u8) ((bufp[1] & 0x0f000000) >> 24);
	addr = segment_base[seg_id] + (bufp[1] & 0x00ffffff);
	osSyncPrintf("\t\tw = %d h = %d t = %d ptr = %08x\n", 
		i0, i1, b0, addr);
	break;

      case (char) G_SETCOMBINE:
	osSyncPrintf("\tSETCOMBINE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETENVCOLOR:
	osSyncPrintf("\tSETENVCOLOR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00ffffff));
	i1 = (int)  ((bufp[1] & 0xffffffff));
	osSyncPrintf("\t\tt = %08x c = %08x\n", i0, i1);
	break;

      case (char) G_SETPRIMCOLOR:
	osSyncPrintf("\tSETPRIMCOLOR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00ffffff));
	i1 = (int)  ((bufp[1] & 0xffffffff));
	osSyncPrintf("\t\tt = %08x c = %08x\n", i0, i1);
	break;

      case (char) G_SETBLENDCOLOR:
	osSyncPrintf("\tSETBLENDCOLOR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00ffffff));
	i1 = (int)  ((bufp[1] & 0xffffffff));
	osSyncPrintf("\t\tt = %08x c = %08x\n", i0, i1);
	break;

      case (char) G_SETFOGCOLOR:
	osSyncPrintf("\tSETFOGCOLOR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00ffffff));
	i1 = (int)  ((bufp[1] & 0xffffffff));
	osSyncPrintf("\t\tt = %08x c = %08x\n", i0, i1);
	break;

      case (char) G_SETFILLCOLOR:
	osSyncPrintf("\tSETFILLCOLOR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00ffffff));
	i1 = (int)  ((bufp[1] & 0xffffffff));
	osSyncPrintf("\t\tt = %08x c = %08x\n", i0, i1);
	break;

      case (char) G_FILLRECT:
	osSyncPrintf("\tFILLRECT ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00fff000) >> 12);
	i1 = (int)  ((bufp[0] & 0x00000fff));
	i2 = (int)  ((bufp[1] & 0x00fff000) >> 12);
	i3 = (char) ((bufp[1] & 0x00000fff));
	osSyncPrintf("\t\tx0 = %d y0 = %d x1 = %d y1 = %d\n",i0,i1,i2,i3);
	break;

      case (char) G_SETTILE:
	osSyncPrintf("\tSETTILE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_LOADTILE:
	osSyncPrintf("\tLOADTILE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_LOADBLOCK:
	osSyncPrintf("\tLOADBLOCK ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETTILESIZE:
	osSyncPrintf("\tSETTILESIZE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_LOADTLUT:
	osSyncPrintf("\tLOADTLUT ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_RDPSETOTHERMODE:
	osSyncPrintf("\tRDPSETOTHERMODE ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETPRIMDEPTH:
	osSyncPrintf("\tSETPRIMDEPTH ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETSCISSOR:
	osSyncPrintf("\tSETSCISSOR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETCONVERT:
	osSyncPrintf("\tSETCONVERT ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETKEYR:
	osSyncPrintf("\tSETKEYR ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_SETKEYGB:
	osSyncPrintf("\tSETKEYGB ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_RDPFULLSYNC:
	osSyncPrintf("\tRDPFULLSYNC ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_RDPTILESYNC:
	osSyncPrintf("\tRDPTILESYNC ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_RDPPIPESYNC:
	osSyncPrintf("\tRDPPIPESYNC ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_RDPLOADSYNC:
	osSyncPrintf("\tRDPLOADSYNC ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;

      case (char) G_TEXRECTFLIP:
	osSyncPrintf("\tTEXRECTFLIP ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00fff000) >> 12);
	i1 = (int)  ((bufp[0] & 0x00000fff));
	i2 = (int)  ((bufp[1] & 0x00fff000) >> 12);
	i3 = (char) ((bufp[1] & 0x00000fff));
	osSyncPrintf("\t\tx0 = %d y0 = %d x1 = %d y1 = %d\n",i0,i1,i2,i3);
	break;

      case (char) G_TEXRECT:
	osSyncPrintf("\tTEXRECT ");
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	i0 = (int)  ((bufp[0] & 0x00fff000) >> 12);
	i1 = (int)  ((bufp[0] & 0x00000fff));
	i2 = (int)  ((bufp[1] & 0x00fff000) >> 12);
	i3 = (char) ((bufp[1] & 0x00000fff));
	osSyncPrintf("\t\tx0 = %d y0 = %d x1 = %d y1 = %d\n",i0,i1,i2,i3);
	break;

      default:
	osSyncPrintf("\tUNKNOWN %02x ",op);
	osSyncPrintf("\t\t\t\t%08x %08x\n",bufp[0],bufp[1]);
	break;
    }
}

void write064bits(u64 input)
{
    /* ick, puke, gag */

    osSyncPrintf("%c%c%c%c%c%c%c%c", 
    (int) (0xff & ((input & ((u64) 0xff << 56))) >> 56),
    (int) (0xff & ((input & ((u64) 0xff << 48))) >> 48),
    (int) (0xff & ((input & ((u64) 0xff << 40))) >> 40),
    (int) (0xff & ((input & ((u64) 0xff << 32))) >> 32),
    (int) (0xff & ((input & ((u64) 0xff << 24))) >> 24),
    (int) (0xff & ((input & ((u64) 0xff << 16))) >> 16),
    (int) (0xff & ((input & ((u64) 0xff <<  8))) >>  8),
    (int) (0xff & ((input & ((u64) 0xff <<  0))) >>  0));
}


void dump_DL(u32 *gfxp, int len)
{
    int i;

    osSyncPrintf("Dumping 0x%08x %d \n", gfxp, len);
    
    for (i=0; i<len/4; i+=2)
	{
	    osSyncPrintf("%.2d 0x%08x 0x%08x\n", i/2,   gfxp[i], gfxp[i+1]); 
	}
}

void dump_mem(u32 *memp, int len, OSTask *tlistp)
{
    int i;
    
    osSyncPrintf("Task header \n");

    osSyncPrintf("0x%08x\n", tlistp->t.type);
    osSyncPrintf("0x%08x\n", tlistp->t.flags);

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.ucode_boot));
    osSyncPrintf("0x%08x\n", tlistp->t.ucode_boot_size);

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.ucode));
    osSyncPrintf("0x%08x\n", tlistp->t.ucode_size);

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.ucode_data));
    osSyncPrintf("0x%08x\n", tlistp->t.ucode_data_size);

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.dram_stack));
    osSyncPrintf("0x%08x\n", tlistp->t.dram_stack_size);

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.output_buff));
    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.output_buff_size));

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.data_ptr));
    osSyncPrintf("0x%08x\n", tlistp->t.data_size);

    osSyncPrintf("0x%08x\n", osVirtualToPhysical(tlistp->t.yield_data_ptr));
    osSyncPrintf("0x%08x\n", tlistp->t.yield_data_size);
    osSyncPrintf("Task finished \n");
    
    for (i=0; i<len/4; i+=2)
	{
	    osSyncPrintf("0x%08x%08x\n", memp[i], memp[i+1]); 
	}
}

/* Nathan's mem dumping task. I'm sure I'm going to go in and reformat
this code when he's not looking. doh! */

#define MAX_DL 1000
#define MAX_DMA 1000

void
gdDumpTask(OSTask *tlistp)
{
	char *c;
	int j,i,addr,same;
	char c1[16],c2[16];
	int cur_list,last_list,list_addr[MAX_DL];
	int cur_dma,last_dma,dma_addr[MAX_DMA],dma_len[MAX_DMA];
	int notdone;
	u32 w1,w2;
	u8 op;
	u32 segment_base[16];

	/*
	 * Dump the task header 
	 */
	osSyncPrintf("\nDUMP_START\n");
	c=(char *)tlistp;
	for(i=0;i<sizeof(OSTask);i+=4) {
		u32 w;
		w= *((u32 *) c);
		w &= 0x7fffffff;
		osSyncPrintf("%08x\n",w);
		c+=4;
	}

	osSyncPrintf("\n\n");

	/*
	 * Dump the task header to dram
	 */
	osSyncPrintf("$%08x\n",(int)tlistp);
	osSyncPrintf("# task header #   \n");
	c=(char *)tlistp;
	for(i=0;i<sizeof(OSTask);i++) {
		osSyncPrintf("%02x\n",*c);
		c++;
	}
	/*
	 * Dump the boot code to dram
	 */
	osSyncPrintf("$%08x\n",(int)tlistp->t.ucode_boot);
	osSyncPrintf("# boot code #   \n");
	c=(char *)(tlistp->t.ucode_boot);
	for(i=0;i<tlistp->t.ucode_boot_size;i++) {
		osSyncPrintf("%02x\n",*c);
		c++;
	}
	/*
	 * Dump the ucode to dram
	 */
	osSyncPrintf("$%08x\n",(int)tlistp->t.ucode);
	osSyncPrintf("# ucode #   \n");
	c=(char *)(tlistp->t.ucode);
	for(i=0;i<8000;i++) {
		osSyncPrintf("%02x\n",*c);
		c++;
	}
	/*
	 * Dump the ucode data to dram
	 */
	osSyncPrintf("$%08x\n",(int)tlistp->t.ucode_data);
	osSyncPrintf("# ucode data #   \n");
	c=(char *)(tlistp->t.ucode_data);
	for(i=0;i<tlistp->t.ucode_data_size;i++) {
		osSyncPrintf("%02x\n",*c);
		c++;
	}

	/*
	 * Dump the display list & its data to dram
	 */
	cur_list=0;
	last_list=0;
	list_addr[0]=(int)tlistp->t.data_ptr;
	while(cur_list<=last_list) {
		osSyncPrintf("$%08x\n",(int)list_addr[cur_list]);
		osSyncPrintf("# dislpay list %d #   \n",cur_list);
		c=(char *) (list_addr[cur_list]|0x80000000);
		notdone=1;
		cur_dma=0;
		last_dma=-1;
		while(notdone) {
			w1=*((u32 *) c);
			w2=*((u32 *) (c+4));
			op=((w1 & 0xff000000) >> 24);
			osSyncPrintf("%08x %08x\n",w1,w2);
			switch(op&0xc0) {
				case 0x00:	/* DMA */
					if (op==(u8)G_DL) {
						list_addr[++last_list]=
							segment_base[((w2 & 0x0f000000) >> 24)] +
							(w2&0x00ffffff);
					} else {
						dma_addr[++last_dma]=segment_base[((w2 & 0x0f000000) >> 24)] +
                                                        (w2&0x00ffffff);
						dma_len[last_dma]=w1 & 0x0000ffff;
					}
					break;
				case 0x80:	/* IMM */
					if (op == (u8)G_ENDDL) notdone=0;
					if (op == (u8)G_SEGMENT) {
						segment_base[((w2 & 0x0f000000) >> 24)] =
							w2 & 0x00ffffff;
					}
				case 0xc0:	/* RDP */
					if (op == (u8)G_SETTIMG) {
						dma_addr[++last_dma]=segment_base[((w2 & 0x0f000000) >> 24)] +
                                                        (w2&0x00ffffff);
                                                dma_len[last_dma]=0x1000;	/* 4k=max texture */
					}
					break;
				default:	/* ERROR */
					osSyncPrintf("\n\n ERROR IN DL: cmd=%08x %08x\n",w1,w2);
					break;
			}
			c+=8;
		}
		while(cur_dma<=last_dma) {
			osSyncPrintf("$%08x\n",(int)dma_addr[cur_dma]);
			osSyncPrintf("# dislpay list data %d @ %08x, len %x #   \n",cur_dma,dma_addr[cur_dma],dma_len[cur_dma]);
			c=(char *)((dma_addr[cur_dma]|0x80000000));
			for(i=0;i<dma_len[cur_dma];i++) {
				osSyncPrintf("%02x\n",*c);
				c++;
			}
			cur_dma++;
		}
		cur_list++;
	}

	/*
	 * dram all dumped
	 */
	osSyncPrintf("\n\n");
}

/* Hackery of Steve's rdp2read program */

/*
 * File:	rdp2read.c
 * Creator:	hsa@sgi.com
 * Create Date:	Wed Jul 27 13:25:47 PDT 1994
 *
 * This program reads in a RDP command stream and prints out a 'readable' 
 * version of it.  Also does some checks for common errors in the 
 * display list.
 *
 */

#ifndef ABS
#define ABS(x)			((x) > 0 ? (x) : (-(x)))
#endif

#ifndef MAX
#define MAX(x, y)		((x) > (y) ? (x) : (y))
#endif

#define MAX_HISTORY		40

#define STL_PREC		(1 << 21)
#define W_PREC			((unsigned)(1 << 31))
#define Z_PREC			(1 << 16)
#define RGBA_PREC		(1 << 16)
#define MAX_COORD_X		((float) (1 << 11) - 1)
#define MIN_COORD_X		(- (float) (1 << 11))
#define MAX_COORD_Y		((float) (1 << 11) - 1)
#define MIN_COORD_Y		(- (float) (1 << 11))

#define D_COMBINE		"combine"
#define D_TEX0			"tex0"
#define D_TEX1			"tex1"
#define D_PRIM			"prim"
#define D_SHADE			"shade"
#define D_ENV			"env"
#define D_CENTER		"center"
#define D_SCALE			"scale"
#define D_COMB_ALPHA		"combined_alpha"
#define D_TEX0_ALPHA		"tex0_alpha"
#define D_TEX1_ALPHA		"tex1_alpha"
#define D_PRIM_ALPHA		"prim_alpha"
#define D_SHADE_ALPHA		"shade_alpha"
#define D_ENV_ALPHA		"env_alpha"
#define D_LOD_FRAC		"lod_frac"
#define D_PRIM_LOD_FRAC		"prim_lod_frac"
#define D_NOISE			"noise"
#define D_CONV_K4		"k4"
#define D_CONV_K5		"k5"
#define D_ONE_PT_OH		"1.0"
#define D_ZERO			"0.0"

 
typedef char 	  		i8;
typedef short int  		i16;
typedef long int 		i32;

typedef struct {
	struct {
	  unsigned int cmd: 8;		/* command */
	  unsigned int tile: 3;		/* tile number */
	  unsigned int npa: 1;		/* non-primitive attribute command */
	  unsigned int prim: 1;		/* primitive command */
	  unsigned int lbt: 1;		/* load block/tile command */
	  unsigned int sts: 1;		/* set tile/tile size command */
	} pc[MAX_HISTORY];  		/* previous commands, circular buffer */
	int curr_cmd;       		/* index of current command */
	int n_cmd;          		/* number of commands processed so far */
} SyncWarnings;


/*
 *   G l o b a l s
 */

static bool		do_verbose = TRUE;
static u32		NumPrimitives = 0;
static SyncWarnings 	SyncW;
static int		nWarnings   = 0;
static int		nErrors     = 0;
static int		nClocks     = 0;
static int		nPrimClocks = 0;
static int		nAttClocks  = 0;
static int		nLoadClocks = 0;
static int		nSyncClocks = 0;

static u8               *RDPListPtr;
static int              GotFullSync = 0;

/*
 *   F u n c t i o n s
 */

/* Silly hack to avoid having to rewrite the structure of this code.
   I basically try to mimic what an fread call is doing except have 
   the input coming from a memory location instead of from an input
   u8 */

static void myfread(void *Output, u8 Size, u8 Num)
{
    memcpy(Output, RDPListPtr, Size * Num);
    
    RDPListPtr += Size * Num;
}


/*
 *  Classify command as non-primitive attribute, which are
 *  not buffered and so must be explicitly synced.
 */
static int
  is_npa_cmd( int cmd )
{
  switch(cmd)
  {
    case G_NOOP:
    case G_SETCIMG:
    case G_SETMIMG:
    case G_SETTIMG:
    case G_SETCOMBINE:
    case G_SETENVCOLOR:
    case G_SETBLENDCOLOR:
    case G_SETFOGCOLOR:
    case G_SETFILLCOLOR:
    case G_RDPSETOTHERMODE:
    /* case G_SETSCISSOR: */
    case G_SETCONVERT:
    case G_SETKEYR:
    case G_SETKEYGB:
	return(1);
    default:
	return(0);
  }
}


/*
 *  Classify as triangle command 
 */
static int
  is_tri_cmd( int cmd )
{
  switch(cmd)
  {
    case G_TRI_FILL:
    case G_TRI_SHADE:
    case G_TRI_TXTR:
    case G_TRI_SHADE_TXTR:
    case G_TRI_FILL_ZBUFF:
    case G_TRI_SHADE_ZBUFF:
    case G_TRI_TXTR_ZBUFF:
    case G_TRI_SHADE_TXTR_ZBUFF:
      return(1);
    default:
      return(0);
  }
}

/*
 *  classify as texture rectangle command
 */
static int
  is_tex_rect_cmd( int cmd )
{
  switch(cmd)
  {
    case G_TEXRECTFLIP:
    case G_TEXRECT:
      return(1);
    default:
      return(0);
  }
}

/*
 *  Classify as rectangle command
 */
static int
  is_rect_cmd( int cmd )
{
  if(is_tex_rect_cmd(cmd) || cmd == G_FILLRECT)
    return(1);
  else
    return(0);
}

/*
 *  Classify command as load block or load tile 
 */
static int
  is_lbt_cmd( int cmd )
{
  switch(cmd)
  {
    case G_LOADTILE:
    case G_LOADBLOCK:
    case G_LOADTLUT:
      return(1);
    default:
      return(0);
  }
}

/*
 * classify comand as primitive
 */
static int
  is_prim_cmd( int cmd )
{
  if(is_rect_cmd(cmd) || is_tri_cmd(cmd) || is_lbt_cmd(cmd))
    return(1);
  else
    return(0);
}

/*
 *  Classify command as set_tile or set_tile_size
 */
static int
  is_sts_cmd( int cmd )
{
  switch(cmd)
  {
    case G_SETTILE:
    case G_SETTILESIZE:
      return(1);
    default:
      return(0);
  }
}


/*
 *  Search through last MAX_HISTORY commands, looking for primitive
 *  command and noting if pipe or full sync command was found first.
 */
static int
  no_sync_since_prim( void )
{
  int indx, i, sync_found = 0, prim_found = 0;

  if(SyncW.n_cmd < 1)
    return(0);

  indx = SyncW.curr_cmd - 1;
  if(indx < 0)
    indx = MAX_HISTORY - 1;

  for(i = 0; i < SyncW.n_cmd; i++)
  {
    /* osSyncPrintf("SyncW.pc[%d].cmd = %d\n", indx, SyncW.pc[indx].cmd); */
    if(SyncW.pc[indx].cmd == G_RDPFULLSYNC || 
       SyncW.pc[indx].cmd == G_RDPPIPESYNC)
      ++sync_found;
    else if(SyncW.pc[indx].prim)
    {
      ++prim_found;
      break;
    }

    --indx;
    if(indx < 0)
      indx = MAX_HISTORY-1;
  }
  /* osSyncPrintf("sync_found = %d\n", sync_found); */
  return(!sync_found && prim_found);
}

/*
 *  Search through last MAX_HISTORY commands, looking for primitive
 *  command and noting if load sync command was found first.
 */
static int
  no_load_sync_since_prim( void )
{
  int indx, i, sync_found = 0, prim_found = 0;

  if(SyncW.n_cmd < 1)
    return(0);

  indx = SyncW.curr_cmd - 1;
  if(indx < 0)
    indx = MAX_HISTORY - 1;

  for(i = 0; i < SyncW.n_cmd; i++)
  {
    /* osSyncPrintf("SyncW.pc[%d].cmd = %d\n", indx, SyncW.pc[indx].cmd); */
    if(SyncW.pc[indx].cmd == G_RDPLOADSYNC || 
       SyncW.pc[indx].cmd == G_RDPFULLSYNC ||
       SyncW.pc[indx].cmd == G_RDPPIPESYNC)
      ++sync_found;
    else if(SyncW.pc[indx].prim)
    {
      ++prim_found;
      break;
    }

    --indx;
    if(indx < 0)
      indx = MAX_HISTORY-1;
  }
  /* osSyncPrintf("sync_found = %d\n", sync_found); */
  return(!sync_found && prim_found);
}

/*
 *  Search through last MAX_HISTORY commands, looking for primitive
 *  command and noting if tile sync command was found before primitive
 *  referencing given tile number.
 */
static int
  no_tile_sync_since_prim( int tile )
{
  int indx, i, sync_found = 0, prim_found = 0;

  if(SyncW.n_cmd < 1)
    return(0);

  indx = SyncW.curr_cmd - 1;
  if(indx < 0)
    indx = MAX_HISTORY - 1;

  for(i = 0; i < SyncW.n_cmd; i++)
  {
    /* osSyncPrintf("SyncW.pc[%d].cmd = %d\n", indx, SyncW.pc[indx].cmd); */
    if(SyncW.pc[indx].cmd == G_RDPTILESYNC ||
       SyncW.pc[indx].cmd == G_RDPFULLSYNC ||
       SyncW.pc[indx].cmd == G_RDPPIPESYNC ||
       SyncW.pc[indx].cmd == G_RDPLOADSYNC)
      ++sync_found;
    else if(SyncW.pc[indx].prim && (SyncW.pc[indx].tile == tile))
    {
      ++prim_found;
      break;
    }

    --indx;
    if(indx < 0)
      indx = MAX_HISTORY-1;
  }
  /* osSyncPrintf("sync_found = %d\n", sync_found); */
  return(!sync_found && prim_found);
}


/*
 *  check_for_syncs:
 *
 *  Check current command against recent history of commands
 */
static void
  check_for_syncs( void )
{
  int cur_cmd;
  int tile;

  cur_cmd = SyncW.pc[SyncW.curr_cmd].cmd;
  tile    = SyncW.pc[SyncW.curr_cmd].tile;

  /* if current command is non-prim attribute,
	and no pipe/full sync since last primitive,
	print warning.
  */
  if(is_npa_cmd(cur_cmd) && no_sync_since_prim())
  {
    osSyncPrintf("******* WARNING: no pipe or full sync between last primitive and current\n");
    osSyncPrintf("*******          attribute change.\n\n");
    ++nWarnings;
  }

  /* if current command is load block/tile, and no load
	sync between last primitive and current command,
	print warning.
  */
  if(is_lbt_cmd(cur_cmd) && no_load_sync_since_prim())
  {
    osSyncPrintf("******* WARNING: no load sync between last primitive and current\n");
    osSyncPrintf("*******          load.\n\n");
    ++nWarnings;
  }

  /* if current command is set tile/tile_size, and previous
	primitive used that tile, and no tile sync before
	current command, print warning.
  */
  if(is_sts_cmd(cur_cmd) && no_tile_sync_since_prim(tile))
  {
    osSyncPrintf("******* WARNING: no tile sync between last primitive that used this tile(%d)\n", tile);
    osSyncPrintf("*******          and current tile setting.\n\n");
    ++nWarnings;
  }
}

float _yh, _ym, _yl, _delxm;

static void
dump_edge_coef(u32 gfx0, u32 gfx1)
{
    i8		dir, level, tile;
    i16		YL, YM, YH;
    float       yl, ym, yh;
    i32		XL, XH, XM;
    float       xl, xh, xm;
    i32		DxLDy, DxHDy, DxMDy;
    float	dxhdy, dxldy, dxmdy;
    u32		buffer[8];
    float 	x[3], y[3], area, dy1, dy2, add_area, sub_area;
    int		i, appx_clks;
    float	dx1, dx2;

    dir =   (gfx0 & 0x00800000) >> 23;
    level = (gfx0 & 0x00380000) >> 19;
    tile =  (gfx0 & 0x00070000) >> 16;

    YL = (gfx0 & 0x0000ffff);
    YM = ((gfx1 & 0xffff0000) >> 16);
    YH = (gfx1 & 0x0000ffff);

    myfread(buffer, sizeof(u32), 6);	/* we already have gfx0, gfx1... */

    XL = *((i32 *) &(buffer[0]));
    DxLDy = *((i32 *) &(buffer[1]));
    XH = *((i32 *) &(buffer[2]));
    DxHDy = *((i32 *) &(buffer[3]));
    XM = *((i32 *) &(buffer[4]));
    DxMDy = *((i32 *) &(buffer[5]));

    _yl = yl = (float)YL/4.0;
    _ym = ym = (float)YM/4.0; 
    _yh = yh = (float)YH/4.0;

    xl = (float)XL / 65536.0;
    xm = (float)XM / 65536.0;
    xh = (float)XH / 65536.0;

    dxhdy = (float) DxHDy / 65536.0;
    dxldy = (float) DxLDy / 65536.0;
    dxmdy = (float) DxMDy / 65536.0;

    _delxm = xl - (xh + (ym-yh)*dxhdy);

    osSyncPrintf("\n\t    Edge coefficients:\n");
    osSyncPrintf("\t\tdir = %d level = %d tile = %d\n",
	    dir, level, tile);
    osSyncPrintf("\t\tYL =     %04hx\tYM =        %04hx\tYH =        %04hx\n",
	    YL, YM, YH);
    osSyncPrintf("\t\t     %8.3f\t       %8.3f\t        %8.3f\n", yl, ym, yh);
    osSyncPrintf("\t\tXL = %08x\tDxLDy = %08x\n", XL, DxLDy);
    osSyncPrintf("\t\t     %8.3f\t       %8.3f\n", xl, dxldy);
    osSyncPrintf("\t\tXH = %08x\tDxHDy = %08x\n", XH, DxHDy);
    osSyncPrintf("\t\t     %8.3f\t       %8.3f\n", xh, dxhdy);
    osSyncPrintf("\t\tXM = %08x\tDxMDy = %08x\n", XM, DxMDy);
    osSyncPrintf("\t\t     %8.3f\t       %8.3f\n", xm, dxmdy);


    y[0] = yh;
    y[1] = ym;
    y[2] = yl;

    x[0] = xh + (yh - (int)yh) * dxhdy;
    x[1] = xl;
    x[2] = xh + dxhdy * (yl - yh);

    for(area = i = 0; i < 3; i++)
      area += x[i] * y[(i+1) % 3] - y[i] * x[(i+1) % 3];
    area /= 2.0;

    area = ABS(area);

    dx1 = ABS(dxhdy - dxmdy);
    dx2 = ABS(dxhdy - dxldy);
    dy1 = (9.0 / dx1);
    dy2 = (9.0 / dx2);

    /* if no span > 9 pixels, clamp to min span size for all spans */
    if(dy1 > (ym - yh) || dy2 > (yl - ym))
    {
      sub_area = area;
      add_area = 9 * (yl - yh);
    }
    else /* find area of sub triangle whose spans are all less than 9 pixels */
    {
      sub_area = (4.5 * dy1 + 4.5 * dy2); 
      add_area = ((dy1 + dy2) * 9.0); 
    }

    appx_clks = 22 + area + add_area - sub_area ;
    nPrimClocks += appx_clks;
    
    osSyncPrintf("\t\t     dy1.........................  %f\n", dy1);
    osSyncPrintf("\t\t     dy2.........................  %f\n", dy2); 
    osSyncPrintf("\t\t     x0 (%f) y0 (%f)\n", x[0], y[0]);
    osSyncPrintf("\t\t     x1 (%f) y1 (%f)\n", x[1], y[1]);
    osSyncPrintf("\t\t     x2 (%f) y2 (%f)\n", x[2], y[2]);

    osSyncPrintf("\t\t     number of spans.............  %d\n", (int)(yl - yh));
    osSyncPrintf("\t\t     area........................  %f\n", area);
    osSyncPrintf("\t\t     add area....................  %f\n", add_area );
    osSyncPrintf("\t\t     sub area....................  %f\n", sub_area );
    osSyncPrintf("\t\t     appx. num clocks............  %d\n", appx_clks );
    osSyncPrintf("\t\t     %% overhead..................  %f\n", 
	(float)(appx_clks - area) / (float) appx_clks * 100.0 );


    /* 
     *   check for illegal X/Y magnitude
     */ 
    if(xl > MAX_COORD_X || xl < MIN_COORD_X)
    {
	osSyncPrintf("\n\t\t ****** ERROR, xl(%f) out of range (%f,%f)\n",
	  xl, MIN_COORD_X, MAX_COORD_X);
	nErrors++;
    }

    if(xh > MAX_COORD_X || xh < MIN_COORD_X)
    {
	osSyncPrintf("\n\t\t ****** ERROR, xh(%f) out of range (%f,%f)\n",
	  xh, MIN_COORD_X, MAX_COORD_X);
	nErrors++;
    }

    if(xm > MAX_COORD_X || xm < MIN_COORD_X)
    {
	osSyncPrintf("\n\t\t ****** ERROR, xm(%f) out of range (%f,%f)\n",
	  xm, MIN_COORD_X, MAX_COORD_X);
	nErrors++;
    }

    if(yl > MAX_COORD_Y || yl < MIN_COORD_Y)
    {
	osSyncPrintf("\n\t\t ****** ERROR, yl(%f) out of range (%f,%f)\n",
	  yl, MIN_COORD_Y, MAX_COORD_Y);
	nErrors++;
    }

    if(yh > MAX_COORD_Y || yh < MIN_COORD_Y)
    {
	osSyncPrintf("\n\t\t ****** ERROR, yh(%f) out of range (%f,%f)\n",
	  yh, MIN_COORD_Y, MAX_COORD_Y);
	nErrors++;
    }

    if(ym > MAX_COORD_Y || ym < MIN_COORD_Y)
    {
	osSyncPrintf("\n\t\t ****** ERROR, ym(%f) out of range (%f,%f)\n",
	  ym, MIN_COORD_Y, MAX_COORD_Y);
	nErrors++;
    }

    
    /* 
     *   check for illegal slope/yh combinations
     */ 

    if(dir == 1) /* left major */
    {
      if((dxmdy < dxhdy) && (YH != YM) ) {
	osSyncPrintf("\n\t\t ****** WARNING, dxmdy < dxhdy for left major triangle\n");
        ++nWarnings;
      }
      if((dxldy > dxmdy) && (YH != YM) ) {
	osSyncPrintf("\n\t\t ****** WARNING, dxldy > dxmdy for left major triangle\n");
        ++nWarnings;
      }
    }
    else /* right major */
    {
      if((dxhdy < dxmdy) && (YH != YM)) {
	osSyncPrintf("\n\t\t ****** WARNING, dxhdy < dxmdy for right major triangle\n");
        ++nWarnings;
      }
      if((dxmdy > dxldy) && (YH != YM) ) {
	osSyncPrintf("\n\t\t ****** WARNING, dxmdy > dxldy for right major triangle\n");
        ++nWarnings;
      }
    }

    if(YM < YH) {
      osSyncPrintf("\n\t\t ****** WARNING, YM < YH\n");
      ++nWarnings;
    }
    if(YL < YH) {
      osSyncPrintf("\n\t\t ****** WARNING, YL < YH\n");
      ++nWarnings;
    }
    if(YL < YM) {
      osSyncPrintf("\n\t\t ****** WARNING, YL < YM\n");
      ++nWarnings;
    }
}

static void
dump_shade_coef()
{
    u16		buffer[32];
    i32		red, green, blue, alpha;
    i32		DrDx, DgDx, DbDx, DaDx;
    i32		DrDe, DgDe, DbDe, DaDe;
    i32		DrDy, DgDy, DbDy, DaDy;

    myfread(buffer, sizeof(u16), 32);

    red   = buffer[0] << 16;	red   |= buffer[8];
    green = buffer[1] << 16;	green |= buffer[9];
    blue  = buffer[2] << 16;	blue  |= buffer[10];
    alpha = buffer[3] << 16;	alpha |= buffer[11];

    DrDx = buffer[4] << 16;	DrDx |= buffer[12];
    DgDx = buffer[5] << 16;	DgDx |= buffer[13];
    DbDx = buffer[6] << 16;	DbDx |= buffer[14];
    DaDx = buffer[7] << 16;	DaDx |= buffer[15];

    DrDe = buffer[16] << 16;	DrDe |= buffer[24];
    DgDe = buffer[17] << 16;	DgDe |= buffer[25];
    DbDe = buffer[18] << 16;	DbDe |= buffer[26];
    DaDe = buffer[19] << 16;	DaDe |= buffer[27];

    DrDy = buffer[20] << 16;	DrDy |= buffer[28];
    DgDy = buffer[21] << 16;	DgDy |= buffer[29];
    DbDy = buffer[22] << 16;	DbDy |= buffer[30];
    DaDy = buffer[23] << 16;	DaDy |= buffer[31];

    osSyncPrintf("\n\t    Shade coefficients:\n");
    osSyncPrintf("\t\t  red %08x green %08x  blue %08x alpha %08x\n",
	    red, green, blue, alpha);
    osSyncPrintf("\t\t     R  %8.6f\t        G  %8.6f\n", 
	(float) red / RGBA_PREC, (float) green / RGBA_PREC);
    osSyncPrintf("\t\t     B  %8.6f\t        A  %8.6f\n", 
	(float) blue / RGBA_PREC, (float) alpha / RGBA_PREC);
    osSyncPrintf("\n");

    osSyncPrintf("\t\t DrDx %08x  DgDx %08x  DbDx %08x  DaDx %08x\n",
	    DrDx, DgDx, DbDx, DaDx);
    osSyncPrintf("\t\t  dRdX  %8.6f\t     dGdX  %8.6f\n", 
	(float) DrDx / RGBA_PREC, (float) DgDx / RGBA_PREC);
    osSyncPrintf("\t\t  dBdX  %8.6f\t     dAdX  %8.6f\n", 
	(float) DbDx / RGBA_PREC, (float) DaDx / RGBA_PREC);
    osSyncPrintf("\n");

    osSyncPrintf("\t\t DrDe %08x  DgDe %08x  DbDe %08x  DaDe %08x\n",
	    DrDe, DgDe, DbDe, DaDe);
    osSyncPrintf("\t\t  dRdE  %8.6f\t     dGdE  %8.6f\n", 
	(float) DrDe / RGBA_PREC, (float) DgDe / RGBA_PREC);
    osSyncPrintf("\t\t  dBdE  %8.6f\t     dAdE  %8.6f\n", 
	(float) DbDe / RGBA_PREC, (float) DaDe / RGBA_PREC);
    osSyncPrintf("\n");

    osSyncPrintf("\t\t DrDy %08x  DgDy %08x  DbDy %08x  DaDy %08x\n",
	    DrDy, DgDy, DbDy, DaDy);
    osSyncPrintf("\t\t  dRdY  %8.6f\t     dGdY  %8.6f\n", 
	(float) DrDy / RGBA_PREC, (float) DgDy / RGBA_PREC);
    osSyncPrintf("\t\t  dBdY  %8.6f\t     dAdY  %8.6f\n", 
	(float) DbDy / RGBA_PREC, (float) DaDy / RGBA_PREC);
    osSyncPrintf("\n");

    osSyncPrintf("\t\tR0 %5.2f\tG0 %5.2f\tB0 %5.2f\tA0 %5.2f\n",
	(float) red   / RGBA_PREC, 
	(float) green / RGBA_PREC, 
	(float) blue  / RGBA_PREC, 
	(float) alpha / RGBA_PREC);
    osSyncPrintf("\t\tR1 %5.2f\tG1 %5.2f\tB1 %5.2f\tA1 %5.2f\n",
	(float) red  / RGBA_PREC + (float)((int)_ym-(int)_yh)*DrDe / RGBA_PREC +
			 (float)_delxm*DrDx / RGBA_PREC, 
	(float) green/ RGBA_PREC + (float)((int)_ym-(int)_yh)*DgDe / RGBA_PREC +
			 (float)_delxm*DgDx / RGBA_PREC, 
	(float) blue / RGBA_PREC + (float)((int)_ym-(int)_yh)*DbDe / RGBA_PREC +
			 (float)_delxm*DbDx / RGBA_PREC, 
	(float) alpha/ RGBA_PREC + (float)((int)_ym-(int)_yh)*DaDe / RGBA_PREC +
			 (float)_delxm*DaDx / RGBA_PREC);
    osSyncPrintf("\t\tR2 %5.2f\tG2 %5.2f\tB2 %5.2f\tA2 %5.2f\n",
	(float) red  / RGBA_PREC + (float)((int)_yl-(int)_yh)*DrDe / RGBA_PREC,
	(float) green/ RGBA_PREC + (float)((int)_yl-(int)_yh)*DgDe / RGBA_PREC,
	(float) blue / RGBA_PREC + (float)((int)_yl-(int)_yh)*DbDe / RGBA_PREC,
	(float) alpha/ RGBA_PREC + (float)((int)_yl-(int)_yh)*DaDe / RGBA_PREC);
    osSyncPrintf("\n");
}

static void
dump_txtr_coef()
{
    u16		buffer[32];
    u32		gfx0, gfx1;
    int 	s, t, w, l;
    int		dxs, dxt, dxw, dxl;
    int		des, det, dew, del;
    int		dys, dyt, dyw, dyl;

    
    myfread(buffer, sizeof(u16), 32);

    s = buffer[0] << 16;	s |= buffer[8];
    t = buffer[1] << 16;	t |= buffer[9];
    w = buffer[2] << 16;	w |= buffer[10];
    l = buffer[3] << 16;	l |= buffer[11];

    dxs = buffer[4] << 16;	dxs |= buffer[12];
    dxt = buffer[5] << 16;	dxt |= buffer[13];
    dxw = buffer[6] << 16;	dxw |= buffer[14];
    dxl = buffer[7] << 16;	dxl |= buffer[15];

    des = buffer[16] << 16;	des |= buffer[24];
    det = buffer[17] << 16;	det |= buffer[25];
    dew = buffer[18] << 16;	dew |= buffer[26];
    del = buffer[19] << 16;	del |= buffer[27];

    dys = buffer[20] << 16;	dys |= buffer[28];
    dyt = buffer[21] << 16;	dyt |= buffer[29];
    dyw = buffer[22] << 16;	dyw |= buffer[30];
    dyl = buffer[23] << 16;	dyl |= buffer[31];


    osSyncPrintf("\n\t    Texture coefficients:\n");
    osSyncPrintf("\t\t  S %08x  T %08x  W %08x  L %08x\n", s, t, w, l);
    osSyncPrintf("\t\t     S  %8.6f\t        T  %8.6f\n", (float) s / STL_PREC, (float) t / STL_PREC);
    osSyncPrintf("\t\t     W  %8.6f\t        L  %8.6f\n", (float) w / W_PREC, (float) l / STL_PREC);
    osSyncPrintf("\n");
    osSyncPrintf("\t\t  dSdX %08x  dTdX %08x  dWdX %08x  dLdX %08x\n", dxs, dxt, dxw, dxl);
    osSyncPrintf("\t\t  dSdX  %8.6f\t     dTdX  %8.6f\n", (float) dxs / STL_PREC, (float) dxt / STL_PREC);
    osSyncPrintf("\t\t  dWdX  %8.6f\t     dLdX  %8.6f\n", (float) dxw / W_PREC, (float) dxl / STL_PREC);
    osSyncPrintf("\n");
    osSyncPrintf("\t\t  dSdE %08x  dTdE %08x  dWdE %08x  dLdE %08x\n", des, det, dew, del);
    osSyncPrintf("\t\t  dSdE  %8.6f\t     dTdE  %8.6f\n", (float) des / STL_PREC, (float) det / STL_PREC);
    osSyncPrintf("\t\t  dWdE  %8.6f\t     dLdE  %8.6f\n", (float) dew / W_PREC, (float) del / STL_PREC);
    osSyncPrintf("\n");
    osSyncPrintf("\t\t  dSdY %08x  dTdY %08x  dWdY %08x  dLdY %08x\n", dys, dyt, dyw, dyl);
    osSyncPrintf("\t\t  dSdY  %8.6f\t     dTdY  %8.6f\n", (float) dys / STL_PREC, (float) dyt / STL_PREC);
    osSyncPrintf("\t\t  dWdY  %8.6f\t     dLdY  %8.6f\n", (float) dyw / W_PREC, (float) dyl / STL_PREC);
    osSyncPrintf("\n");

    osSyncPrintf("\t\tS0 %5.2f\tT0 %5.2f\tW0 %9.6f\tL0 %9.6f\n",
	(float) s     / STL_PREC, 
	(float) t     / STL_PREC, 
	(float) w     / W_PREC, 
	(float) l     / STL_PREC);
    osSyncPrintf("\t\tS1 %5.2f\tT1 %5.2f\tW1 %9.6f\tL1 %9.6f\n",
	(float) s     / STL_PREC + (float)((int)_ym-(int)_yh)*des / STL_PREC +
			 (float)_delxm*dxs / STL_PREC, 
	(float) t     / STL_PREC + (float)((int)_ym-(int)_yh)*det / STL_PREC +
			 (float)_delxm*dxt / STL_PREC, 
	(float) w     / W_PREC   + (float)((int)_ym-(int)_yh)*dew / W_PREC   +
			 (float)_delxm*dxw / W_PREC, 
	(float) l     / STL_PREC + (float)((int)_ym-(int)_yh)*del / STL_PREC +
			 (float)_delxm*dxl / STL_PREC);
    osSyncPrintf("\t\tS2 %5.2f\tT2 %5.2f\tW2 %9.6f\tL2 %9.6f\n",
	(float) s     / STL_PREC + (float)((int)_yl-(int)_yh)*des / STL_PREC,
	(float) t     / STL_PREC + (float)((int)_yl-(int)_yh)*det / STL_PREC,
	(float) w     / W_PREC   + (float)((int)_yl-(int)_yh)*dew / W_PREC,
	(float) l     / STL_PREC + (float)((int)_yl-(int)_yh)*del / STL_PREC);
    osSyncPrintf("\n");
}



static void
dump_zbuff_coef()
{
    u32		buffer[4];
    int		z, dxz, dez, dyz;

    myfread(buffer, sizeof(u32), 4);
    
    z   = buffer[0];
    dxz = buffer[1];
    dez = buffer[2];
    dyz = buffer[3];

    osSyncPrintf("\n\t    Z-buffer coefficients:\n");
    osSyncPrintf("\t\t  Z %08x  dZdX %08x  dZdE %08x  dZdY %08x\n", z, dxz, dez, dyz);
    osSyncPrintf("\t\t     Z  %8.6f\t     dZdX  %8.6f\n", (float)   z / 65536.0, (float) dxz / 65536.0);
    osSyncPrintf("\t\t  dZdE  %8.6f\t     dZdY  %8.6f\n", (float) dez / 65536.0, (float) dyz / 65536.0);
    osSyncPrintf("\n");

    osSyncPrintf("\t\tZ0 %8.6f\n",
	(float) z     / 65536.0);
    osSyncPrintf("\t\tZ1 %8.6f\n",
	(float) z     / 65536.0 + (float)((int)_ym-(int)_yh)*dez / 65536.0 +
			 (float)_delxm*dxz / 65536.0);
    osSyncPrintf("\t\tZ2 %8.6f\n",
	(float) z     / 65536.0 + (float)((int)_yl-(int)_yh)*dez / 65536.0);
    osSyncPrintf("\n");
}


/*
 *  suba_decode: convert select to string
 */
static char *
  suba_decode(int sel)
{
   switch(sel)
   {
     case 0:  return(D_COMBINE);
     case 1:  return(D_TEX0);
     case 2:  return(D_TEX1);
     case 3:  return(D_PRIM);
     case 4:  return(D_SHADE);
     case 5:  return(D_ENV);
     case 6:  return(D_ONE_PT_OH);
     case 7:  return(D_NOISE);
     default: return(D_ZERO);
   }
}

/*
 *  subb_decode: convert select to string
 */
static char *
  subb_decode(int sel)
{
   switch(sel)
   {
     case 0:  return(D_COMBINE);
     case 1:  return(D_TEX0);
     case 2:  return(D_TEX1);
     case 3:  return(D_PRIM);
     case 4:  return(D_SHADE);
     case 5:  return(D_ENV);
     case 6:  return(D_CENTER);
     case 7:  return(D_CONV_K4);
     default: return(D_ZERO);
   }
}

/*
 *  mul_decode: convert select to string
 */
static char *
  mul_decode(int sel)
{
   switch(sel)
   {
     case 0:  return(D_COMBINE);
     case 1:  return(D_TEX0);
     case 2:  return(D_TEX1);
     case 3:  return(D_PRIM);
     case 4:  return(D_SHADE);
     case 5:  return(D_ENV);
     case 6:  return(D_SCALE);
     case 7:  return(D_COMB_ALPHA);
     case 8:  return(D_TEX0_ALPHA);
     case 9:  return(D_TEX1_ALPHA);
     case 10: return(D_PRIM_ALPHA);
     case 11: return(D_SHADE_ALPHA);
     case 12: return(D_ENV_ALPHA);
     case 13: return(D_LOD_FRAC);
     case 14: return(D_PRIM_LOD_FRAC);
     case 15: return(D_CONV_K5);
     default: return(D_ZERO);
   }
}

/*
 *  add_decode: convert select to string
 */
static char *
  add_decode(int sel)
{
   switch(sel & 7)
   {
     case 0:  return(D_COMBINE);
     case 1:  return(D_TEX0);
     case 2:  return(D_TEX1);
     case 3:  return(D_PRIM);
     case 4:  return(D_SHADE);
     case 5:  return(D_ENV);
     case 6:  return(D_ONE_PT_OH);
     case 7:  return(D_ZERO);
   }
}

/*
 *  a_subab_decode: convert select to string
 */
static char *
  a_subab_decode(int sel)
{
   switch(sel & 7)
   {
     case 0:  return(D_COMB_ALPHA);
     case 1:  return(D_TEX0_ALPHA);
     case 2:  return(D_TEX1_ALPHA);
     case 3:  return(D_PRIM_ALPHA);
     case 4:  return(D_SHADE_ALPHA);
     case 5:  return(D_ENV_ALPHA);
     case 6:  return(D_ONE_PT_OH);
     case 7:  return(D_ZERO);
   }
}

/*
 *  a_mul_decode: convert select to string
 */
static char *
  a_mul_decode(int sel)
{
   switch(sel & 7)
   {
     case 0:  return(D_LOD_FRAC);
     case 1:  return(D_TEX0_ALPHA);
     case 2:  return(D_TEX1_ALPHA);
     case 3:  return(D_PRIM_ALPHA);
     case 4:  return(D_SHADE_ALPHA);
     case 5:  return(D_ENV_ALPHA);
     case 6:  return(D_PRIM_LOD_FRAC);
     case 7:  return(D_ZERO);
   }
}

/*
 *  a_add_decode: convert select to string
 */
static char *
  a_add_decode(int sel)
{
   switch(sel & 7)
   {
     case 0:  return(D_COMB_ALPHA);
     case 1:  return(D_TEX0_ALPHA);
     case 2:  return(D_TEX1_ALPHA);
     case 3:  return(D_PRIM_ALPHA);
     case 4:  return(D_SHADE_ALPHA);
     case 5:  return(D_ENV_ALPHA);
     case 6:  return(D_ONE_PT_OH);
     case 7:  return(D_ZERO);
   }
}


static void
  dump_setcombine(u32 gfx0, u32 gfx1)
{
  union {
    struct {
      unsigned int pad0:    2;
      unsigned int cmd:     6;
      unsigned int suba_r0: 4;
      unsigned int mult_r0: 5;
      unsigned int suba_a0: 3;
      unsigned int mult_a0: 3;
      unsigned int suba_r1: 4;
      unsigned int mult_r1: 5;
    }   bf;
    u32 word;
  } w0;
	
  union {
    struct {
      unsigned int subb_r0: 4;
      unsigned int subb_r1: 4;
      unsigned int suba_a1: 3;
      unsigned int mult_a1: 3;
      unsigned int adds_r0: 3;
      unsigned int subb_a0: 3;
      unsigned int adds_a0: 3;
      unsigned int adds_r1: 3;
      unsigned int subb_a1: 3;
      unsigned int adds_a1: 3;
    }   bf;
    u32 word;
  } w1;

  unsigned int suba, subb, mul, add;
  char *dsuba, *dsubb, *dmul, *dadd;

  w0.word = gfx0;
  w1.word = gfx1;

  suba = w0.bf.suba_r0;
  subb = w1.bf.subb_r0;
  mul  = w0.bf.mult_r0;
  add  = w1.bf.adds_r0;

  osSyncPrintf("\n\t    Red/Green/Blue Controls, Cycle 0:\n");
  dsuba = suba_decode(suba);
  dsubb = subb_decode(subb);
  dmul  = mul_decode(mul);
  dadd  = add_decode(add);
  osSyncPrintf("\t\t    (%s - %s) * %s + %s\n", dsuba, dsubb, dmul, dadd);

  suba = w0.bf.suba_r1;
  subb = w1.bf.subb_r1;
  mul  = w0.bf.mult_r1;
  add  = w1.bf.adds_r1;

  osSyncPrintf("\n\t    Red/Green/Blue Controls, Cycle 1:\n");
  dsuba = suba_decode(suba);
  dsubb = subb_decode(subb);
  dmul  = mul_decode(mul);
  dadd  = add_decode(add);
  osSyncPrintf("\t\t    (%s - %s) * %s + %s\n", dsuba, dsubb, dmul, dadd);

  suba = w0.bf.suba_a0;
  subb = w1.bf.subb_a0;
  mul  = w0.bf.mult_a0;
  add  = w1.bf.adds_a0;

  osSyncPrintf("\n\t    Alpha Controls, Cycle 0:\n");
  dsuba = a_subab_decode(suba);
  dsubb = a_subab_decode(subb);
  dmul  = a_mul_decode(mul);
  dadd  = a_add_decode(add);
  osSyncPrintf("\t\t    (%s - %s) * %s + %s\n", dsuba, dsubb, dmul, dadd);

  suba = w1.bf.suba_a1;
  subb = w1.bf.subb_a1;
  mul  = w1.bf.mult_a1;
  add  = w1.bf.adds_a1;

  osSyncPrintf("\n\t    Alpha Controls, Cycle 1:\n");
  dsuba = a_subab_decode(suba);
  dsubb = a_subab_decode(subb);
  dmul  = a_mul_decode(mul);
  dadd  = a_add_decode(add);
  osSyncPrintf("\t\t    (%s - %s) * %s + %s\n", dsuba, dsubb, dmul, dadd);
}

/*
 *  Dump other modes
 */
static void
  dump_setother(u32 gfx0, u32 gfx1)
{
  union {
    struct {
      unsigned int pad0: 2;
      unsigned int cmd: 6;
      unsigned int atomic: 1;
      unsigned int dither_en: 1;
      unsigned int cycle_type: 2;
      unsigned int persp_tex_en: 1;
      unsigned int detail_tex_en: 1;
      unsigned int sharpen_tex_en: 1;
      unsigned int tex_lod_en: 1;
      unsigned int en_tlut: 1;
      unsigned int tlut_type: 1;
      unsigned int sample_type: 1;
      unsigned int mid_texel: 1;
      unsigned int bi_lerp_0: 1;
      unsigned int bi_lerp_1: 1;
      unsigned int convert_one: 1;
      unsigned int key_en: 1;
      unsigned int blend_mask: 8;
    } bf;
    u32 word;
  } w0;

  union {
    struct {
      unsigned int b_m1a_0: 2;
      unsigned int b_m1a_1: 2;
      unsigned int b_m1b_0: 2;
      unsigned int b_m1b_1: 2;
      unsigned int b_m2a_0: 2;
      unsigned int b_m2a_1: 2;
      unsigned int b_m2b_0: 2;
      unsigned int b_m2b_1: 2;
      unsigned int texture_edge_mode: 1;
      unsigned int force_blend: 1;
      unsigned int alpha_cvg_select: 1;
      unsigned int cvg_times_alpha: 1;
      unsigned int z_mode: 2;
      unsigned int cvg_dest: 2;
      unsigned int color_on_cvg: 1;
      unsigned int image_read_en: 1;
      unsigned int z_update_en: 1;
      unsigned int z_compare_en: 1;
      unsigned int antialias_en: 1;
      unsigned int z_source_sel: 1;
      unsigned int dither_alpha_en: 1;
      unsigned int alpha_compare_en: 1;
    } bf;
    u32 word;
  } w1;

  w0.word = gfx0;
  w1.word = gfx1;

  osSyncPrintf("\t\t    atomic primitive .....  %d\n", w0.bf.atomic);
  osSyncPrintf("\t\t    dither_enable.........  %d\n", w0.bf.dither_en);
  osSyncPrintf("\t\t    cycle_type............  %d ", w0.bf.cycle_type);

  if(w0.bf.cycle_type == 0) osSyncPrintf("(one cycle)\n");
  else if(w0.bf.cycle_type == 1) osSyncPrintf("(two cycle)\n");
  else if(w0.bf.cycle_type == 2) osSyncPrintf("(copy)\n");
  else osSyncPrintf("(fill)\n");

  osSyncPrintf("\t\t    persp_tex_en..........  %d\n", w0.bf.persp_tex_en);
  osSyncPrintf("\t\t    detail_tex_en.........  %d\n", w0.bf.detail_tex_en);
  osSyncPrintf("\t\t    sharpen_tex_en........  %d\n", w0.bf.sharpen_tex_en);
  osSyncPrintf("\t\t    tex_lod_en............  %d\n", w0.bf.tex_lod_en);
  osSyncPrintf("\t\t    en_tlut...............  %d\n", w0.bf.en_tlut);
  osSyncPrintf("\t\t    tlut_type.............  %d\n", w0.bf.tlut_type);
  osSyncPrintf("\t\t    sample_type...........  %d\n", w0.bf.sample_type);
  osSyncPrintf("\t\t    mid_texel.............  %d\n", w0.bf.mid_texel);
  osSyncPrintf("\t\t    bi_lerp_0.............  %d\n", w0.bf.bi_lerp_0);
  osSyncPrintf("\t\t    bi_lerp_1.............  %d\n", w0.bf.bi_lerp_1);
  osSyncPrintf("\t\t    convert_one...........  %d\n", w0.bf.convert_one);
  osSyncPrintf("\t\t    key_en................  %d\n", w0.bf.key_en);
  osSyncPrintf("\t\t    blend_mask............  0x%0.2x\n", w0.bf.blend_mask);

  osSyncPrintf("\n\t    Blend Mux Controls, Cycle 0: ");
  osSyncPrintf("(%d, %d, %d, %d)\n", w1.bf.b_m1a_0, w1.bf.b_m1b_0, w1.bf.b_m2a_0,
    w1.bf.b_m2b_0);
  osSyncPrintf("\t\t    blend mult 1a.........  ");

  switch(w1.bf.b_m1a_0)
  {
    case 0: osSyncPrintf("pixel/blend color\n");
	    break;
    case 1: osSyncPrintf("memory color\n");
	    break;
    case 2: osSyncPrintf("blend reg color\n");
	    break;
    case 3: osSyncPrintf("fog color\n");
	    break;
  }

  osSyncPrintf("\t\t    blend mult 1b.........  ");

  switch(w1.bf.b_m1b_0)
  {
    case 0: osSyncPrintf("pixel alpha\n");
	    break;
    case 1: osSyncPrintf("fog alpha\n");
	    break;
    case 2: osSyncPrintf("shade alpha\n");
            break;
    case 3: osSyncPrintf("0.0\n");
            break;
  }

  osSyncPrintf("\t\t    blend mult 2a.........  ");

  switch(w1.bf.b_m2a_0)
  {
    case 0: osSyncPrintf("pixel/blend color\n");
            break;
    case 1: osSyncPrintf("memory color\n");
            break;
    case 2: osSyncPrintf("blend reg color\n");
            break;
    case 3: osSyncPrintf("fog color\n");
            break;
  }

  osSyncPrintf("\t\t    blend mult 2b.........  ");

  switch(w1.bf.b_m2b_0)
  {
    case 0: osSyncPrintf("1.0 - <1b input>\n");
            break;
    case 1: osSyncPrintf("memory alpha\n");
            break;
    case 2: osSyncPrintf("1.0\n");
            break;
    case 3: osSyncPrintf("0.0\n");
            break;
  }

  osSyncPrintf("\n\t    Blend Mux Controls, Cycle 1: ");
  osSyncPrintf("(%d, %d, %d, %d)\n", w1.bf.b_m1a_1, w1.bf.b_m1b_1, w1.bf.b_m2a_1,
    w1.bf.b_m2b_1);
  osSyncPrintf("\t\t    blend mult 1a.........  ");

  switch(w1.bf.b_m1a_1)
  {
    case 0: osSyncPrintf("pixel/blend color\n");
	    break;
    case 1: osSyncPrintf("memory color\n");
	    break;
    case 2: osSyncPrintf("blend reg color\n");
	    break;
    case 3: osSyncPrintf("fog color\n");
	    break;
  }

  osSyncPrintf("\t\t    blend mult 1b.........  ");

  switch(w1.bf.b_m1b_1)
  {
    case 0: osSyncPrintf("pixel alpha\n");
	    break;
    case 1: osSyncPrintf("fog alpha\n");
	    break;
    case 2: osSyncPrintf("shade alpha\n");
            break;
    case 3: osSyncPrintf("0.0\n");
            break;
  }

  osSyncPrintf("\t\t    blend mult 2a.........  ");

  switch(w1.bf.b_m2a_1)
  {
    case 0: osSyncPrintf("pixel/blend color\n");
            break;
    case 1: osSyncPrintf("memory color\n");
            break;
    case 2: osSyncPrintf("blend reg color\n");
            break;
    case 3: osSyncPrintf("fog color\n");
            break;
  }

  osSyncPrintf("\t\t    blend mult 2b.........  ");

  switch(w1.bf.b_m2b_1)
  {
    case 0: osSyncPrintf("1.0 - <1b input>\n");
            break;
    case 1: osSyncPrintf("memory alpha\n");
            break;
    case 2: osSyncPrintf("1.0\n");
            break;
    case 3: osSyncPrintf("0.0\n");
            break;
  }

  osSyncPrintf("\n");

  osSyncPrintf("\t\t    texture_edge_mode.....  %d\n", w1.bf.texture_edge_mode);
  osSyncPrintf("\t\t    force_blend...........  %d\n", w1.bf.force_blend);
  osSyncPrintf("\t\t    alpha_cvg_select......  %d\n", w1.bf.alpha_cvg_select);
  osSyncPrintf("\t\t    cvg_times_alpha.......  %d\n", w1.bf.cvg_times_alpha);

  osSyncPrintf("\t\t    z_mode................  ");
  switch(w1.bf.z_mode)
  {
    case 0: osSyncPrintf("opaque\n");
            break;
    case 1: osSyncPrintf("interpenetrating\n");
            break;
    case 2: osSyncPrintf("transparent\n");
            break;
    case 3: osSyncPrintf("decal\n");
            break;
  }


  osSyncPrintf("\t\t    cvg_dest..............  ");
  switch(w1.bf.cvg_dest)
  {
    case 0: osSyncPrintf("clamp(normal)\n");
            break;
    case 1: osSyncPrintf("wrap\n");
            break;
    case 2: osSyncPrintf("zap(force to full cvg)\n");
            break;
    case 3: osSyncPrintf("save(don't overwrite mem cvg)\n");
            break;
  }

  osSyncPrintf("\t\t    color_on_cvg..........  %d\n", w1.bf.color_on_cvg);
  osSyncPrintf("\t\t    image_read_en.........  %d\n", w1.bf.image_read_en);
  osSyncPrintf("\t\t    z_update_en...........  %d\n", w1.bf.z_update_en);
  osSyncPrintf("\t\t    z_compare_en..........  %d\n", w1.bf.z_compare_en);
  osSyncPrintf("\t\t    antialias_en..........  %d\n", w1.bf.antialias_en);
  osSyncPrintf("\t\t    z_source_sel..........  %s\n",
	    w1.bf.z_source_sel ? "prim z" : "pixel z");
  osSyncPrintf("\t\t    dither_alpha_en.......  %d\n", w1.bf.dither_alpha_en);
  osSyncPrintf("\t\t    alpha_compare_en......  %d\n", w1.bf.alpha_compare_en);
}



static void
  dump_texrect(u32 gfx0, u32 gfx1)
{
  u32		buffer[4];
  Gtexrect	*texrp;
  float		xl, yl, xh, yh;
  float 	area;

  buffer[0] = gfx0;
  buffer[1] = gfx1;
  myfread(&gfx0, sizeof(u32), 1);
  myfread(&gfx1, sizeof(u32), 1);
  buffer[2] = gfx0;
  buffer[3] = gfx1;
  osSyncPrintf("\t\t\t\t\t%08x %08x\n",buffer[2],buffer[3]);
  texrp = (Gtexrect *) &(buffer[0]);
  xl = (float)texrp->xl / 4.0;
  xh = (float)texrp->xh / 4.0;
  yl = (float)texrp->yl / 4.0;
  yh = (float)texrp->yh / 4.0;

  osSyncPrintf("\t\tile = %d\n", texrp->tile);
  osSyncPrintf("\t\txl = %f yl = %f xh = %f yh = %f\n",
		xl, yl, xh, yh);
  osSyncPrintf("\t\ts = %f t = %f dsdx = %f dtdy = %f\n",
		(float)texrp->s / 32.0, (float)texrp->t / 32.0, 
		(float)texrp->dsdx / 1024.0, (float)texrp->dtdy / 1024.0);
 
  area = (xl - xh) * (yl - yh); 
  osSyncPrintf("\t\t     area........................  %f\n", area);

  nPrimClocks += area;
}

/*
 * Parse and dump a RDP-type display list command.
 */
static void
do_rdp(u32 gfx0, u32 gfx1)
{
    u32		addr, buffer[20];
    int		i0, i1, i2, i3;
    char	op, b0, tstring[32];
    float	area;
    Gsetimg	*imgp;
    Gsetcombine	*combp;
    Gsetcolor	*scolp;
    Gfillrect	*fillrp;
    Gsettile	*stilep;
    Gloadtile	*ltilep;
    Gloadtlut	*ltlutp;

    /* every command has at least 64-bits... */
    buffer[0] = gfx0;
    buffer[1] = gfx1;

    op = (char) ((gfx0 & 0xff000000) >> 24);
   

    /*
     *  Classify command for next sync check
     */
    SyncW.pc[SyncW.curr_cmd].cmd  = op;
    SyncW.pc[SyncW.curr_cmd].npa  = is_npa_cmd(op);
    SyncW.pc[SyncW.curr_cmd].prim = is_prim_cmd(op);
    SyncW.pc[SyncW.curr_cmd].sts  = is_sts_cmd(op);
    SyncW.pc[SyncW.curr_cmd].lbt  = is_lbt_cmd(op);

    /* save tile number */
    if(is_tex_rect_cmd(op) || is_lbt_cmd(op) || is_sts_cmd(op))
      SyncW.pc[SyncW.curr_cmd].tile = ((gfx1 & 0x07000000) >> 24);
    else if(is_tri_cmd(op))
      SyncW.pc[SyncW.curr_cmd].tile = ((gfx0 & 0x00070000) >> 16);
    else
      SyncW.pc[SyncW.curr_cmd].tile = 0;

    check_for_syncs();

    /* increment/clamp indexes */
    ++SyncW.curr_cmd;
    if(SyncW.curr_cmd > (MAX_HISTORY-1))
      SyncW.curr_cmd = 0;

    ++SyncW.n_cmd;
    if(SyncW.n_cmd > MAX_HISTORY)
      SyncW.n_cmd = MAX_HISTORY;


    /*
     *  Formatted Dump of each command
     */
    switch (op) {

      case (char) G_SETCIMG:
	osSyncPrintf("\tSETCIMG ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	imgp = (Gsetimg *) &(buffer[0]);
	osSyncPrintf("\t\tfmt = %d siz = %d wd = %d ptr = %08x\n", 
		imgp->fmt, imgp->siz, imgp->wd, gfx1);
	++nAttClocks;
	break;

      case (char) G_SETMIMG:
	osSyncPrintf("\tSETMIMG ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	imgp = (Gsetimg *) &(buffer[0]);
	osSyncPrintf("\t\tfmt = %d siz = %d wd = %d ptr = %08x\n", 
		imgp->fmt, imgp->siz, imgp->wd, gfx1);
	++nAttClocks;
	break;

      case (char) G_SETTIMG:
	osSyncPrintf("\tSETTIMG ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	imgp = (Gsetimg *) &(buffer[0]);
	osSyncPrintf("\t\tfmt = %d siz = %d wd = %d ptr = %08x\n", 
		imgp->fmt, imgp->siz, imgp->wd, gfx1);
	++nAttClocks;
	break;

      case (char) G_SETCOMBINE:
	osSyncPrintf("\tSETCOMBINE ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
        dump_setcombine(gfx0, gfx1); 
	++nAttClocks;
	break;

      case (char) G_SETENVCOLOR:
	osSyncPrintf("\tSETENVCOLOR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	scolp = (Gsetcolor *) &(buffer[0]);
	osSyncPrintf("\t\tc = %08x\n", scolp->color);
	++nAttClocks;
	break;

      case (char) G_SETPRIMCOLOR:
	osSyncPrintf("\tSETPRIMCOLOR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	scolp = (Gsetcolor *) &(buffer[0]);
	osSyncPrintf("\t\tprim_min_lev = %d prim_lev = %d c = %08x\n",
		scolp->prim_min_level, scolp->prim_level, scolp->color);
	++nAttClocks;
	break;

      case (char) G_SETBLENDCOLOR:
	osSyncPrintf("\tSETBLENDCOLOR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	scolp = (Gsetcolor *) &(buffer[0]);
	osSyncPrintf("\t\tc = %08x\n", scolp->color);
	++nAttClocks;
	break;

      case (char) G_SETFOGCOLOR:
	osSyncPrintf("\tSETFOGCOLOR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	scolp = (Gsetcolor *) &(buffer[0]);
	osSyncPrintf("\t\tc = %08x\n", scolp->color);
	++nAttClocks;
	break;

      case (char) G_SETFILLCOLOR:
	osSyncPrintf("\tSETFILLCOLOR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	scolp = (Gsetcolor *) &(buffer[0]);
	osSyncPrintf("\t\tc = %08x\n", scolp->color);
	++nAttClocks;
	break;

      case (char) G_FILLRECT:
	osSyncPrintf("\tFILLRECT ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	fillrp = (Gfillrect *) &(buffer[0]);
	osSyncPrintf("\t\tulx = %d uly = %d lrx = %d lry = %d\n",
		fillrp->x1, fillrp->y1, fillrp->x0, fillrp->y0);
       
        area = MAX((fillrp->x0 - fillrp->x1) * (fillrp->y0 - fillrp->y1),
                        9 * (fillrp->y0 - fillrp->y1)); 
	nPrimClocks += (int) area;
	break;

      case (char) G_SETTILE:
	osSyncPrintf("\tSETTILE ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	stilep = (Gsettile *) &(buffer[0]);
	osSyncPrintf("\t\tfmt = %d siz = %d line = %d tmem = %08x\n",
		stilep->fmt, stilep->siz, stilep->line, stilep->tmem);
	osSyncPrintf("\t\ttile = %d pal = %d ct = %d mt = %d\n",
		stilep->tile, stilep->palette, stilep->ct, stilep->mt);
	osSyncPrintf("\t\tmaskt = %d shiftt = %d cs = %d ms = %d masks = %d shifts = %d\n",
		stilep->maskt, stilep->shiftt, stilep->cs, stilep->ms,
		stilep->masks, stilep->shifts);
	++nAttClocks;
	break;

      case (char) G_LOADTILE:
	osSyncPrintf("\tLOADTILE ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1,NumPrimitives++);
	ltilep = (Gloadtile *) &(buffer[0]);
	osSyncPrintf("\t\tsl = %f tl = %f tile = %d sh = %f th = %f\n",
		(float)ltilep->sl / 4.0, (float)ltilep->tl / 4.0, ltilep->tile, 
		(float)ltilep->sh / 4.0, (float)ltilep->th / 4.0);
	break;

      case (char) G_LOADBLOCK:
	osSyncPrintf("\tLOADBLOCK ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1,NumPrimitives++);
	/* same as load tile... */
	ltilep = (Gloadblock *) &(buffer[0]);
	osSyncPrintf("\t\tsl = %f tl = %f tile = %d sh = %f dxt = %f\n",
		(float)ltilep->sl, (float)ltilep->tl, ltilep->tile, 
		(float)ltilep->sh, (float)ltilep->th / (float) (1 << 11));
	break;

      case (char) G_SETTILESIZE:
	osSyncPrintf("\tSETTILESIZE ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* same as load tile... */
	ltilep = (Gsettilesize *) &(buffer[0]);
	osSyncPrintf("\t\tsl = %f tl = %f tile = %d sh = %f th = %f\n",
		(float)ltilep->sl / 4.0, (float)ltilep->tl / 4.0, ltilep->tile, 
		(float)ltilep->sh / 4.0, (float)ltilep->th / 4.0);
	++nAttClocks;
	break;

      case (char) G_LOADTLUT:
	osSyncPrintf("\tLOADTLUT ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1,NumPrimitives++);
	ltlutp = (Gloadtlut *) &(buffer[0]);
	osSyncPrintf("\t\tsl = %f tl = %f tile = %d sh = %f th = %f\n",
		(float)ltlutp->sl / 4.0, (float)ltlutp->tl / 4.0, ltlutp->tile, 
		(float)ltlutp->sh / 4.0, (float)ltlutp->th / 4.0);
	break;

      case (char) G_RDPSETOTHERMODE:
	osSyncPrintf("\tRDPSETOTHERMODE ");
	osSyncPrintf("\t\t%08x %08x\n",gfx0,gfx1);
	dump_setother(gfx0, gfx1);
	++nAttClocks;
	break;

      case (char) G_SETPRIMDEPTH:
	osSyncPrintf("\tSETPRIMDEPTH ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	i0 = gfx1 >> 16;
	i1 = (gfx1 << 16) >> 16;
	osSyncPrintf("\t\tprimZ = %d primDelZ = %d\n", i0, i1);
	++nAttClocks;
	break;

      case (char) G_SETSCISSOR:
	osSyncPrintf("\tSETSCISSOR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	i0 = (gfx0 & 0x00fff000) >> 12;
	i1 = (gfx0 & 0x00000fff);
	i2 = (gfx1 & 0x00fff000) >> 12;
	i3 = (gfx1 & 0x00000fff);
	osSyncPrintf("\t\txH = %.3f yH = %.3f xL = %.3f yL = %.3f\n",
		(float)i0 / 4.0, (float)i1 / 4.0, (float)i2 / 4.0, (float)i3 / 4.0);
	osSyncPrintf("\t\tulx = %.3f uly = %.3f lrx = %.3f lry = %.3f\n",
		(float)i0 / 4.0, (float)i1 / 4.0, (float)i2 / 4.0, (float)i3 / 4.0);
	if (gfx1 & 0x02000000) {
	    osSyncPrintf( "\t\tscissor field ON, ");
	    if (gfx1 & 0x01000000)
		osSyncPrintf( "keep odd lines\n");
	    else
		osSyncPrintf( "keep even lines\n");
	} else {
	    osSyncPrintf( "\t\tscissor field OFF\n");
	}
	++nAttClocks;
	break;

      case (char) G_SETCONVERT:
	osSyncPrintf("\tSETCONVERT ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	++nAttClocks;
	break;

      case (char) G_SETKEYR:
	osSyncPrintf("\tSETKEYR ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	++nAttClocks;
	break;

      case (char) G_SETKEYGB:
	osSyncPrintf("\tSETKEYGB ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	++nAttClocks;
	break;

      case (char) G_RDPFULLSYNC:
	osSyncPrintf("\tRDPFULLSYNC ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	GotFullSync = 1;
	nSyncClocks += 43;
	break;

      case (char) G_RDPTILESYNC:
	osSyncPrintf("\tRDPTILESYNC ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	nSyncClocks += 28;
	break;

      case (char) G_RDPPIPESYNC:
	osSyncPrintf("\tRDPPIPESYNC ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	nSyncClocks += 43;
	break;

      case (char) G_RDPLOADSYNC:
	osSyncPrintf("\tRDPLOADSYNC ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	/* do something... */
	nSyncClocks += 18;
	break;

      case (char) G_NOOP:
	osSyncPrintf("\tNOOP ");
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	nAttClocks++;
	break;

      case (char) G_TEXRECTFLIP:
	osSyncPrintf("\tTEXRECTFLIP ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1,NumPrimitives++);
	dump_texrect(gfx0, gfx1);
	break;

      case (char) G_TEXRECT:
	osSyncPrintf("\tTEXRECT ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_texrect(gfx0, gfx1);
	break;

      case (char) G_TRI_FILL:
	osSyncPrintf("\tG_TRI_FILL ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	break;

      case (char) G_TRI_SHADE:
	osSyncPrintf("\tG_TRI_SHADE ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_shade_coef();
	break;

      case (char) G_TRI_TXTR:
	osSyncPrintf("\tG_TRI_TXTR ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_txtr_coef();
	break;

      case (char) G_TRI_SHADE_TXTR:
	osSyncPrintf("\tG_TRI_SHADE_TXTR ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_shade_coef();
	dump_txtr_coef();
	break;

      case (char) G_TRI_FILL_ZBUFF:
	osSyncPrintf("\tG_TRI_FILL_ZBUFF ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1,NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_zbuff_coef();
	break;

      case (char) G_TRI_SHADE_ZBUFF:
	osSyncPrintf("\tG_TRI_SHADE_ZBUFF ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1,NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_shade_coef();
	dump_zbuff_coef();
	break;

      case (char) G_TRI_TXTR_ZBUFF:
	osSyncPrintf("\tG_TRI_TXTR_ZBUFF ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_txtr_coef();
	dump_zbuff_coef();
	break;

      case (char) G_TRI_SHADE_TXTR_ZBUFF:
	osSyncPrintf("\tG_TRI_SHADE_TXTR_ZBUFF ");
	osSyncPrintf("\t\t\t%08x %08x    prim #%04d\n",gfx0,gfx1, NumPrimitives++);
	dump_edge_coef(gfx0, gfx1);
	dump_shade_coef();
	dump_txtr_coef();
	dump_zbuff_coef();
	break;

      default:
	osSyncPrintf("\tUNKNOWN %02x ",op);
	osSyncPrintf("\t\t\t%08x %08x\n",gfx0,gfx1);
	osSyncPrintf("\n\t\t ****** ERROR, unknown command\n");
	nErrors++;
	nAttClocks++;
	break;
    }

    osSyncPrintf("\n");
}

/*
 * main routine.
 */

void dump_rdp(u8 *RDPList)
{
    u32		gfx0, gfx1, *gfxp;
    int i;

    RDPListPtr = RDPList;

    osSyncPrintf("\n\t\tReadable dump of RDP list:\n\n");
    osSyncPrintf( "----------------------------------------------------------------\n");

    myfread(&gfx0, sizeof(u32), 1);
    myfread(&gfx1, sizeof(u32), 1);

    GotFullSync = 0;
    while (!GotFullSync) {

	do_rdp(gfx0, gfx1);

	myfread(&gfx0, sizeof(u32), 1);
	myfread(&gfx1, sizeof(u32), 1);
    }

    osSyncPrintf( "\n---------------------------------------------------------------\n");
    osSyncPrintf( "Number of Warnings: %d\n", nWarnings);
    osSyncPrintf( "Number of Errors: %d\n", nErrors);
    osSyncPrintf( "Number of Sync Clocks (approximate): %d\n", nSyncClocks);
    osSyncPrintf( "Number of Att  Clocks (approximate): %d\n", nAttClocks);
    osSyncPrintf( "Number of Prim Clocks (approximate): %d\n", nPrimClocks);
    osSyncPrintf( "Number of Load Clocks (approximate): Not Implemented\n", nLoadClocks);
    osSyncPrintf( "                                    ------\n");
    osSyncPrintf( "Number of Total Clocks (approximate): %d\n", nSyncClocks +
	nAttClocks + nPrimClocks + nLoadClocks);
}
