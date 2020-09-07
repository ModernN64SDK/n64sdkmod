
/*
 * This silly little program does the "in-betweening" and builds
 * angle tables for a walk cycle.
 *
 */

#include <stdio.h>

float	lf[64], sf[64], lr[64], sr[64];

#define	WALK_CYCLE_LENGTH	20
#define CYCLE_KEY1_DIV		4
#define CYCLE_KEY2_DIV		2

#define LEG_INITIAL	0.0
#define LEG_FINAL	0.0
#define SHIN_INITIAL	0.0
#define SHIN_FINAL	0.0

#define FORWARD_LEG_LIFT	(-60.0)
#define FORWARD_LEG_MID		(-45.0)
#define FORWARD_SHIN_BEND	(60.0)

#define REAR_LEG_PUSH		(60.0)
#define REAR_SHIN_BEND		(60.0)

/*
 *
 */
int
main(int argc, char *argv[])
{
    int		i, j, max_frames, key1, key2, key3;
    float	lfi, sfi, lri, sri;


    while ((argc > 1) && (argv[1][0] == '-')) {
	switch(argv[1][1]) {
	    
	  default:
	    fprintf(stderr,"option [%s] not recognized.\n", argv[1]);
	    break;

	}
	argc--;
	argv++;
    }

    max_frames = WALK_CYCLE_LENGTH;
    key1 = (max_frames/CYCLE_KEY1_DIV);
    key2 = (max_frames/CYCLE_KEY2_DIV);
    key3 = key1+key2;

    lfi = sfi = lri = sri = 0.0;
    lfi = FORWARD_LEG_LIFT / key1;
    sfi = FORWARD_SHIN_BEND / key1;
    lri = REAR_LEG_PUSH / key2;

    for (i=1; i<max_frames; i++) {

	lf[i] = lf[i-1] + lfi;
	sf[i] = sf[i-1] + sfi;
	lr[i] = lr[i-1] + lri;
	sr[i] = sr[i-1] + sri;

	if (i == key1) {
	    lfi = (FORWARD_LEG_MID - FORWARD_LEG_LIFT)/(key2-key1);
	    sfi = (SHIN_FINAL - FORWARD_SHIN_BEND)/(key2-key1);
	}
	if (i == key2) {
	    lfi = (LEG_FINAL - FORWARD_LEG_MID)/(max_frames-key2);
	    sfi = 0.0;

	    lri = -lri;
	    sri = (REAR_SHIN_BEND - SHIN_INITIAL)/(key3-key2);
	}
	if (i == key3) {
	    sri = (SHIN_FINAL - REAR_SHIN_BEND)/(max_frames-key3);
	}
    }

    fprintf(stdout,"\n/* ---- GENERATED FILE, DO NOT EDIT ---- */\n\n");
    fprintf(stdout,"/*\n * walktable.h\n *\n");
    fprintf(stdout," * This file holds the walk cycle angle tables.\n *\n */\n");
    fprintf(stdout,"\n#define WALK_CYCLE_LEN\t%d\n\n",max_frames+1);

    fprintf(stdout,"float leg_front[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",lf[i]);
    }
    fprintf(stdout,"\t%f\n",0.0);
    fprintf(stdout,"};\n\n");

    fprintf(stdout,"float shin_front[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",sf[i]);
    }
    fprintf(stdout,"\t%f\n",0.0);
    fprintf(stdout,"};\n\n");

    fprintf(stdout,"float leg_rear[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",lr[i]);
    }
    fprintf(stdout,"\t%f\n",0.0);
    fprintf(stdout,"};\n\n");

    fprintf(stdout,"float shin_rear[] =\n{\n");
    for (i=0; i<max_frames; i++) {
	fprintf(stdout,"\t%f,\n",sr[i]);
    }
    fprintf(stdout,"\t%f\n",0.0);
    fprintf(stdout,"};\n\n");

}
