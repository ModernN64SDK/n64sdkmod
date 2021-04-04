/*
 * morph.c -- R4300 Morph Routines, adopted for morphfaces
 */

#include <ultra64.h>

/*
 * morph -- morph vertex positions
 *
 * Input:
 *        Vtx **valist   = list of input vertex arrays
 *        float *weights = weight for each array.  Should sum to 1.0.
 *        int vacnt      = number of vertex arrays (>= 2)
 *        Vtx *vout      = output vertex array (preallocated)
 *        int vcnt       = number of verticies per list
 *        int alpha      = alpha value to assign to output verticies
 *        int zdist      = z value to assign to output verticies
 *
 * Comments:
 *        1) Texture coordinates are copied from the first set of verticies
 *        2) The flag value is copied from the first set of verticies.
 *        3) The routine is not necessarily perfectly optimized for speed,
 *           although it's pretty good.
 */

/* Faster version -- assumes vacnt == 2, doesn't do normals, does fixed Z */

void morph(Vtx **valist, float *weights, int vacnt, Vtx *vout,
	   int vcnt, int alpha, int zdist) {
  float w0, wj;
  float ob0, ob1, ob2, tc0, tc1, cn0, cn1, cn2, cn3;
  int i, j, k;
  Vtx *a0, *aj;  /* Current input array */
  OSTime mystart, myend, diff; /* For performance timing */

  if (vacnt != 2) {
#ifdef DEBUG
    osSyncPrintf("Uh oh\n");
#endif
    return;
  }

  mystart = osGetTime();
  w0 = weights[0];
  a0 = valist[0];
  for (i=0; i<vcnt; i++) {
    ob0 = w0 * a0[i].v.ob[0]; /* x */
    ob1 = w0 * a0[i].v.ob[1]; /* y */
    ob2 = zdist;              /* z */
    tc0 =      a0[i].v.tc[0] << 6; /* texture u */
    tc1 =      a0[i].v.tc[1] << 6; /* texture v */
    cn0 = w0 * a0[i].v.cn[0]; /* Color R */
    cn1 = w0 * a0[i].v.cn[1]; /* Color G */
    cn2 = w0 * a0[i].v.cn[2]; /* Color B */
    cn3 = alpha;              /* Color A */

    wj = weights[1];
    aj = valist[1];
    ob0 += wj * aj[i].v.ob[0]; /* x */
    ob1 += wj * aj[i].v.ob[1]; /* y */
    cn0 += wj * aj[i].v.cn[0]; /* Color R */
    cn1 += wj * aj[i].v.cn[1]; /* Color G */
    cn2 += wj * aj[i].v.cn[2]; /* Color B */

    vout[i].v.ob[0] = ob0;
    vout[i].v.ob[1] = ob1;
    vout[i].v.ob[2] = ob2;
    vout[i].v.flag  = 0;
    vout[i].v.tc[0] = tc0;
    vout[i].v.tc[1] = tc1;
    vout[i].v.cn[0] = cn0;
    vout[i].v.cn[1] = cn1;
    vout[i].v.cn[2] = cn2;
    vout[i].v.cn[3] = cn3;
    
  } /* for i */

  myend = osGetTime();
  diff = myend-mystart;
#if 0
  osSyncPrintf("morph =%lli nsec (%lli cycles)\n", CYCLES_TO_NSEC(diff), diff);
#endif

} /* morph */



