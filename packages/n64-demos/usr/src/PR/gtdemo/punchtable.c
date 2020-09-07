
/*
 * This silly little program does the "in-betweening" and builds
 * angle tables for a punch cycle.
 *
 */

#include <stdio.h>

float	la[64], lw[64], ra[64], rw[64];

#define	PUNCH_CYCLE_LENGTH	10

#define L_ARM_INITIAL	0.0
#define R_ARM_INITIAL	-45.0
#define L_WRIST_INITIAL	-120.0
#define R_WRIST_INITIAL	-90.0

#define L_ARM_FINAL	-90.0
#define R_ARM_FINAL	-90.0
#define L_WRIST_FINAL	-10.0
#define R_WRIST_FINAL	-5.0

/*
 *
 */
int
main(int argc, char *argv[])
{
    int		i, j, max_frames, key1, key2, key3;
    float	lai, lwi, rai, rwi;


    while ((argc > 1) && (argv[1][0] == '-')) {
	switch(argv[1][1]) {
	    
	  default:
	    fprintf(stderr,"option [%s] not recognized.\n", argv[1]);
	    break;

	}
	argc--;
	argv++;
    }

    max_frames = PUNCH_CYCLE_LENGTH;

    key1 = max_frames/2;

    la[0] = L_ARM_INITIAL;
    lw[0] = L_WRIST_INITIAL;
    ra[0] = R_ARM_INITIAL;
    rw[0] = R_WRIST_INITIAL;

    lai = (L_ARM_FINAL - L_ARM_INITIAL)/key1;
    lwi = (L_WRIST_FINAL - L_WRIST_INITIAL)/key1;
    rai = (R_ARM_FINAL - R_ARM_INITIAL)/key1;
    rwi = (R_WRIST_FINAL - R_WRIST_INITIAL)/key1;

    for (i=1; i<max_frames; i++) {

	la[i] = la[i-1] + lai;
	lw[i] = lw[i-1] + lwi;
	ra[i] = ra[i-1] + rai;
	rw[i] = rw[i-1] + rwi;

	if (i == key1) {
	    lai = -lai;
	    lwi = -lwi;
	    rai = -rai;
	    rwi = -rwi;
	}
    }

    fprintf(stdout,"\n/* ---- GENERATED FILE, DO NOT EDIT ---- */\n\n");
    fprintf(stdout,"/*\n * punchtable.h\n *\n");
    fprintf(stdout," * This file holds the punch cycle angle tables.\n *\n */\n");
    fprintf(stdout,"\n#define PUNCH_CYCLE_LEN\t%d\n\n",max_frames+1);

    fprintf(stdout,"float left_arm[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",la[i]);
    }
    fprintf(stdout,"\t%f\n",L_ARM_INITIAL);
    fprintf(stdout,"};\n\n");

    fprintf(stdout,"float left_wrist[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",lw[i]);
    }
    fprintf(stdout,"\t%f\n",L_WRIST_INITIAL);
    fprintf(stdout,"};\n\n");

    fprintf(stdout,"float right_arm[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",ra[i]);
    }
    fprintf(stdout,"\t%f\n",R_ARM_INITIAL);
    fprintf(stdout,"};\n\n");

    fprintf(stdout,"float right_wrist[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",rw[i]);
    }
    fprintf(stdout,"\t%f\n",R_WRIST_INITIAL);
    fprintf(stdout,"};\n\n");

}
