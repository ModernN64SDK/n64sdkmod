
/*---------------------------------------------------------------------*
        Copyright (C) 1997 Nintendo. (Originated by SGI)
        
        $RCSfile: matrix.c,v $
        $Revision: 1.5 $
        $Date: 1998/09/30 13:21:03 $
 *---------------------------------------------------------------------*/
#include <ultra64.h>
#include "matrix.h"

#ifdef DEBUG
/*
 * Function:	printMtx
 * Description:	uses osSyncPrintf to print out a mtx in integer form
 */
void printMtx(char *name,Mtx *mat)
{
    int i,j;

    osSyncPrintf("\n%s:\n",name);
    for (i=0; i<4; i++)
    {
	for (j=0; j<4; j++)
	{
	    osSyncPrintf("0x%08x ",mat->m[i][j]);
	}
	osSyncPrintf("\n");
    }
}

/*
 * Function:	printMat
 * Description:	uses osSyncPrintf to print out a float matrix
 */
void printMat(char *name, FMatrix mat)
{
    int i,j;

    osSyncPrintf("\n%s:\n",name);
    for (i=0; i<4; i++) {
	for (j=0; j<4; j++)
	    osSyncPrintf("%1.2f ",mat[i][j]);
	osSyncPrintf("\n");
    }
}



/*
 * Function:	printFMtx
 * Description:	uses osSyncPrintf to print out an mtx in floating point
 */
void printFMtx(char *name, Mtx *mat)
{
    float fmat[4][4];
    int i;

    guMtxL2F(fmat, mat);
    osSyncPrintf("\n%s:\n",name);
    for (i=0;i<4;i++)
    {
	osSyncPrintf("\t%f %f %f %f\n",
		   fmat[i][0],fmat[i][1],fmat[i][2],fmat[i][3]);
    }
}

#endif


/*
 * Function:	multMtxPos
 * Description:	Multiplies a matrix and a 3D position.
 * Arguments:	vec = 3D position
 *		mat = Mtx
 * Result:	res = mat * vec
 * NOTE:	res may be the same as vec
 */
void multMtxPos(float res[3], Mtx *mat, float pos[3])
{
    float fmat[4][4];
    int   i, j;
    float tmp4[4];
    float pos4[4];

    guMtxL2F(fmat, mat);

    pos4[0] = pos[0];
    pos4[1] = pos[1];
    pos4[2] = pos[2];
    pos4[3] = 1.0;
    
    for (i=0;i<4;i++)
    {
	tmp4[i] = 0.0;
	for (j=0;j<4;j++)
	    tmp4[i] += (fmat[j][i] * pos4[j]);
    }

    res[0] = tmp4[0] / tmp4[3];
    res[1] = tmp4[1] / tmp4[3];
    res[2] = tmp4[2] / tmp4[3];
}


/*
 * Function:	multMatVec
 * Description:	Multiplies a matrix and a 3D vector.
 * Arguments:	vec = 3D vector
 *		mat = matrix
 * Result:	res = mat * vec
 * NOTE:	res may be the same as vec
 */
void multMtxVec(vec3 res, Mtx *mat, vec3 vec)
{
    float fmat[4][4];
    int   i, j;
    float tmp4[4];
    float vec4[4];

    guMtxL2F(fmat, mat);

    vec4[0] = vec[0];
    vec4[1] = vec[1];
    vec4[2] = vec[2];
    vec4[3] = 0.0;
    
    for (i=0;i<4;i++)
    {
	tmp4[i] = 0.0;
	for (j=0;j<4;j++)
	    tmp4[i] += (fmat[j][i] * vec4[j]);
    }

    res[0] = tmp4[0];
    res[1] = tmp4[1];
    res[2] = tmp4[2];
}

/*
 * Function:	multMatPos
 * Description:	Multiplies a matrix and a 3D position.
 * Arguments:	vec = 3D position
 *		mat = floating-point matrix
 * Result:	res = mat * vec
 * NOTE:	res may be the same as vec
 */
void multMatPos(float res[3], FMatrix fmat, float pos[3])
{
    int   i, j;
    float tmp4[4];
    float pos4[4];

    pos4[0] = pos[0];
    pos4[1] = pos[1];
    pos4[2] = pos[2];
    pos4[3] = 1.0;
    
    for (i=0;i<4;i++)
    {
	tmp4[i] = 0.0;
	for (j=0;j<4;j++)
	    tmp4[i] += (fmat[j][i] * pos4[j]);
    }

    res[0] = tmp4[0] / tmp4[3];
    res[1] = tmp4[1] / tmp4[3];
    res[2] = tmp4[2] / tmp4[3];
}



/*
 * Function:	multMatVec
 * Description:	Multiplies a matrix and a 3D vector.
 * Arguments:	vec = 3D vector
 *		mat = floating-point matrix
 * Result:	res = mat * vec
 * NOTE:	res may be the same as vec
 */
void multMatVec(vec3 res, FMatrix fmat, vec3 vec)
{
    int   i, j;
    float tmp4[4];
    float vec4[4];

    vec4[0] = vec[0];
    vec4[1] = vec[1];
    vec4[2] = vec[2];
    vec4[3] = 0.0;
    
    for (i=0;i<4;i++)
    {
	tmp4[i] = 0.0;
	for (j=0;j<4;j++)
	    tmp4[i] += (fmat[j][i] * vec4[j]);
    }

    res[0] = tmp4[0];
    res[1] = tmp4[1];
    res[2] = tmp4[2];
}


/*
 * Function:	multMtx
 * Description:	Multiplies two Mtx's and puts their product in a third.
 * Arguments:	dst - the destination matrix
 *		s1, s2 - the source matrices.
 * Result:	dst = s1 * s2
 * NOTE:	dst may be the same as either of the source matrices
 */
void multMtx(Mtx *dst, Mtx *s1, Mtx *s2)
{
    int i, j;
    float result[4][4], fs1[4][4], fs2[4][4];

    guMtxL2F(fs1, s1);
    guMtxL2F(fs2, s2);

    for (i=0;i<4;i++)
    {
        for (j=0;j<4;j++)
        {
            result[i][j] = (fs1[i][0] * fs2[0][j] +
                            fs1[i][1] * fs2[1][j] +
                            fs1[i][2] * fs2[2][j] +
                            fs1[i][3] * fs2[3][j]);
        }
    }

    guMtxF2L(result, dst);
}

/*
 * Function:	multMat
 * Description:	Multiplies two matrices and puts their product in a third.
 * Arguments:	dst - the destination matrix
 *		s1, s2 - the source matrices.
 * Result:	dst = s1 * s2
 * NOTE:	dst may be the same as either of the source matrices
 */
void multMat(FMatrix dst, FMatrix s1, FMatrix s2)
{
    int i, j;
    float result[4][4];

    for (i=0;i<4;i++)
    {
        for (j=0;j<4;j++)
        {
            result[i][j] = (s1[i][0] * s2[0][j] +
                            s1[i][1] * s2[1][j] +
                            s1[i][2] * s2[2][j] +
                            s1[i][3] * s2[3][j]);
        }
    }
    copyMat(dst, result);
}


#define SMALL 1e-8              /* Small enough to be considered zero */
#define SWAP(A,B,T)             T=B,B=A,A=T
#define ABS(X)			(((X)>=0)?(X):-(X))

/*****************************************************************************

Shuffles rows in inverse of 3x3.  See comment in matrix_inv3_second_col().

*****************************************************************************/
static void
matrix_inv3_swap(float inv[3][3], int row0, int row1, int row2)
{
   int   i, tempi;
   float temp;

#define SWAP_ROWS(a, b) \
   for (i = 0; i < 3; i++) SWAP(inv[a][i], inv[b][i], temp); \
   SWAP(a, b, tempi)

   if (row0 != 0){
      if (row1 == 0) {
         SWAP_ROWS(row0, row1);
      }
      else {
         SWAP_ROWS(row0, row2);
      }
   }

   if (row1 != 1) {
      SWAP_ROWS(row1, row2);
   }
}


/*****************************************************************************

Does Gaussian elimination on second column.

*****************************************************************************/
static int
matrix_inv3_second_col(float source[3][3], float inv[3][3], int row0)
{
   int   row1, row2, i1, i2, i;
   float temp;
   float a, b;

   /* Find which row to use */
   if      (row0 == 0)  i1 = 1, i2 = 2;
   else if (row0 == 1)  i1 = 0, i2 = 2;
   else                 i1 = 0, i2 = 1;

   /* Find which is larger in abs. val.:the entry in [i1][1] or [i2][1] */
   /* and use that value for pivoting.                                  */

   a = source[i1][1]; if (a < 0) a = -a;
   b = source[i2][1]; if (b < 0) b = -b;
   if (a > b)   row1 = i1;
   else         row1 = i2;
   row2 = (row1 == i1 ? i2 : i1);

   /* Scale row1 in source */
   if ((source[row1][1] < SMALL) && (source[row1][1] > -SMALL)) return(0);
   temp = 1.0 / source[row1][1];
   source[row1][1]  = 1.0;
   source[row1][2] *= temp;     /* source[row1][0] is zero already */

   /* Scale row1 in inv */
   inv[row1][row1]  = temp;     /* it used to be a 1.0 */
   inv[row1][row0] *= temp;

   /* Clear column one, source, and make corresponding changes in inv */

   for (i = 0; i < 3; i++) if (i != row1) {     /* for i = all rows but row1 */
   temp = -source[i][1];
      source[i][1]  = 0.0;
      source[i][2] += temp * source[row1][2];

      inv[i][row1]  = temp * inv[row1][row1];
      inv[i][row0] += temp * inv[row1][row0];
   }

   /* Scale row2 in source */
   if ((source[row2][2] < SMALL) && (source[row2][2] > -SMALL)) return(0);
   temp = 1.0 / source[row2][2];
   source[row2][2] = 1.0;       /* source[row2][*] is zero already */

   /* Scale row2 in inv */
   inv[row2][row2]  = temp;     /* it used to be a 1.0 */
   inv[row2][row0] *= temp;
   inv[row2][row1] *= temp;

   /* Clear column one, source, and make corresponding changes in inv */
   for (i = 0; i < 3; i++) if (i != row2) {     /* for i = all rows but row2 */
   temp = -source[i][2];
      source[i][2] = 0.0;
      inv[i][row0] += temp * inv[row2][row0];
      inv[i][row1] += temp * inv[row2][row1];
      inv[i][row2] += temp * inv[row2][row2];
   }

   /*
    * Now all is done except that the inverse needs to have its rows shuffled.
    * row0 needs to be moved to inv[0][*], row1 to inv[1][*], etc.
    *
    * We *didn't* do the swapping before the elimination so that we could more
    * easily keep track of what ops are needed to be done in the inverse.
    */
   matrix_inv3_swap(inv, row0, row1, row2);

   return(1);
}


/*****************************************************************************

Fast inversion routine for 3 x 3 matrices.   - Written by jfh.

This takes 30 multiplies/divides, as opposed to 39 for Cramer's Rule.
The algorithm consists of performing fast gaussian elimination, by never
doing any operations where the result is guaranteed to be zero, or where
one operand is guaranteed to be zero. This is done at the cost of clarity,
alas.

Returns 1 if the inverse was successful, 0 if it failed.

*****************************************************************************/
static int
matrix_invert3(float source[3][3], float inv[3][3])
{
   int   i, row0;
   float temp;
   float a, b, c;

   inv[0][0] = inv[1][1] = inv[2][2] = 1.0;
   inv[0][1] = inv[0][2] = inv[1][0] = inv[1][2] = inv[2][0] = inv[2][1] = 0.0;

   /* attempt to find the largest entry in first column to use as pivot */
   a = source[0][0]; if (a < 0) a = -a;
   b = source[1][0]; if (b < 0) b = -b;
   c = source[2][0]; if (c < 0) c = -c;

   if (a > b) {
      if (a > c) row0 = 0;
      else row0 = 2;
   }
   else {
      if (b > c) row0 = 1;
      else row0 = 2;
   }

   /* Scale row0 of source */
   if ((source[row0][0] < SMALL) && (source[row0][0] > -SMALL)) return(0);
   temp = 1.0 / source[row0][0];
   source[row0][0]  = 1.0;
   source[row0][1] *= temp;
   source[row0][2] *= temp;

   /* Scale row0 of inverse */
   inv[row0][row0] = temp;      /* other entries are zero -- no effort  */

   /* Clear column zero of source, and make corresponding changes in inverse */

   for (i = 0; i < 3; i++) if (i != row0) {     /* for i = all rows but row0 */
      temp = -source[i][0];
      source[i][0]  = 0.0;
      source[i][1] += temp * source[row0][1];
      source[i][2] += temp * source[row0][2];
      inv[i][row0]  = temp * inv[row0][row0];
   }


   /*
    * We've now done gaussian elimination so that the source and
    * inverse look like this:
    *
    *   1 * *           * 0 0
    *   0 * *           * 1 0
    *   0 * *           * 0 1
    *
    * We now proceed to do elimination on the second column.
    */
   if (!matrix_inv3_second_col(source, inv, row0)) return(0);

   return(1);
}


/*****************************************************************************

Finds a new pivot for a non-simple 4x4.  See comments in MatInvert().

*****************************************************************************/
static int
matrix_inv4_pivot(float src[4][4], float r[4], float *s, int *swap)
{
   int   i, j;
   float temp, max;

   *swap = -1;

   if (ABS(src[3][3]) < SMALL) {

      /* Look for a different pivot element: one with largest abs value */
      max = 0.0;

      for (i = 0; i < 4; i++) {
         if      (src[i][3] >  max) max =  src[*swap = i][3];
         else if (src[i][3] < -max) max = -src[*swap = i][3];
      }

      /* No pivot element available ! */
      if (*swap < 0) return(0);

      else for (j = 0; j < 4; j++) SWAP(src[*swap][j], src[3][j], temp);
   }

   r[0] = -src[0][3];
   r[1] = -src[1][3];
   r[2] = -src[2][3];

   *s = 1.0 / src[3][3];

   src[0][3] = src[1][3] = src[2][3] = 0.0;
   src[3][3]                         = 1.0;

   src[3][0] *= (*s);
   src[3][1] *= (*s);
   src[3][2] *= (*s);

   for (i = 0; i < 3; i++) {
      src[0][i] += r[0] * src[3][i];
      src[1][i] += r[1] * src[3][i];
      src[2][i] += r[2] * src[3][i];
   }

   return(1);
}


/*****************************************************************************

This returns the inverse of the given matrix.  The result matrix
may be the same as the one to invert.

Fast inversion routine for 4 x 4 matrices, written by jfh.

Returns 1 if the inverse was successful, 0 if it failed.

This routine has been specially tweaked to notice the following:
If the matrix has the form
       * * * 0
       * * * 0
       * * * 0
       * * * 1

(as do many matrices in graphics), then we compute the inverse of
the upper left 3x3 matrix and use this to find the general inverse.

  In the event that the right column is not 0-0-0-1, we do gaussian
elimination to make it so, then use the 3x3 inverse, and then do
our gaussian elimination.

*****************************************************************************/
int
floatMatInvert(float inv[4][4], float src[4][4])
{
   float mat[4][4];
   int   i, j, simple;
   float m[3][3], inv3[3][3], s, temp;
   float r[4], t[4];
   int   swap;

   /* invert code is row major */

   for (i=0; i<4; i++)
    for (j=0; j<4; j++) {
       mat[i][j] = src[i][j];
       inv[i][j] = 0;
    }
   inv[0][0] = 1;
   inv[1][1] = 1;
   inv[2][2] = 1;
   inv[3][3] = 1;

   /* If last column is not (0,0,0,1), use special code */
   simple = (mat[0][3] == 0.0 && mat[1][3] == 0.0 &&
             mat[2][3] == 0.0 && mat[3][3] == 1.0);

   if (! simple && !matrix_inv4_pivot(mat, r, &s, &swap)) return (0);

   t[0] = mat[3][0];
   t[1] = mat[3][1];
   t[2] = mat[3][2];

   /* Copy upper-left 3x3 matrix */
   for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) m[i][j] = mat[i][j];

   if (! matrix_invert3(m, inv3)) return(0);

   for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) inv[i][j] = inv3[i][j];

   for (i = 0; i < 3; i++) for (j = 0; j < 3; j++)
      inv[3][i] -= t[j] * inv3[j][i];

   if (! simple) {

      /* We still have to undo our gaussian elimination from earlier on */
      /* add r0 * first col to last col */
      /* add r1 * 2nd   col to last col */
      /* add r2 * 3rd   col to last col */

      for (i = 0; i < 4; i++) {
         inv[i][3] += r[0] * inv[i][0] +
                      r[1] * inv[i][1] +
                      r[2] * inv[i][2];
         inv[i][3] *= s;
      }

      if (swap >= 0)
         for (i = 0; i < 4; i++) SWAP(inv[i][swap], inv[i][3], temp);
   }

   return (1);
}
