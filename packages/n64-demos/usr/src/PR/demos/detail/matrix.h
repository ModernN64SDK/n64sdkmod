
#ifndef __matrix__
#define __matrix__

#include <ultra64.h>
#include "vector.h"

typedef float FMatrix[4][4];

extern void multMtxPos(vec3 res, Mtx *mat, vec3 pos);
extern void multMtxVec(vec3 res, Mtx *mat, vec3 vec);
extern void multMatPos(vec3 res, FMatrix mat, vec3 pos);
extern void multMatVec(vec3 res, FMatrix mat, vec3 vec);
extern void multMatPosNoNorm(float res[4], FMatrix fmat, float pos[3]);
extern void multMtx(Mtx *dst, Mtx *s1, Mtx *s2);
extern void multMat(FMatrix dst, FMatrix s1, FMatrix s2);

#ifdef DEBUG
extern void printMat(char *name, FMatrix mat);
extern void printMtx(char *name, Mtx *mat);
extern void printFMtx(char *name, Mtx *mat);
#endif

extern int floatMatInvert(float inv[4][4], float src[4][4]);

#define makeIdentMtx(m)	guMtxIdent(m)

#define makeIdentMat(m) guMtxIdent(m)

/*
 * Macro:	copyMtx
 * Description:	copys matrix s into matrix dst
 */
#define copyMtx(dst,s)					\
if (1) {						\
    (dst)->m[0][0] = (s)->m[0][0];			\
    (dst)->m[0][1] = (s)->m[0][1];			\
    (dst)->m[0][2] = (s)->m[0][2];			\
    (dst)->m[0][3] = (s)->m[0][3];			\
							\
    (dst)->m[1][0] = (s)->m[1][0];			\
    (dst)->m[1][1] = (s)->m[1][1];			\
    (dst)->m[1][2] = (s)->m[1][2];			\
    (dst)->m[1][3] = (s)->m[1][3];			\
							\
    (dst)->m[2][0] = (s)->m[2][0];			\
    (dst)->m[2][1] = (s)->m[2][1];			\
    (dst)->m[2][2] = (s)->m[2][2];			\
    (dst)->m[2][3] = (s)->m[2][3];			\
							\
    (dst)->m[3][0] = (s)->m[3][0];			\
    (dst)->m[3][1] = (s)->m[3][1];			\
    (dst)->m[3][2] = (s)->m[3][2];			\
    (dst)->m[3][3] = (s)->m[3][3];			\
} else

/*
 * Macro:	copyMat
 * Description:	copys matrix s into matrix dst
 */
#define copyMat(dst,s)				\
if (1) {					\
    (dst)[0][0] = (s)[0][0];			\
    (dst)[0][1] = (s)[0][1];			\
    (dst)[0][2] = (s)[0][2];			\
    (dst)[0][3] = (s)[0][3];			\
						\
    (dst)[1][0] = (s)[1][0];			\
    (dst)[1][1] = (s)[1][1];			\
    (dst)[1][2] = (s)[1][2];			\
    (dst)[1][3] = (s)[1][3];			\
						\
    (dst)[2][0] = (s)[2][0];			\
    (dst)[2][1] = (s)[2][1];			\
    (dst)[2][2] = (s)[2][2];			\
    (dst)[2][3] = (s)[2][3];			\
						\
    (dst)[3][0] = (s)[3][0];			\
    (dst)[3][1] = (s)[3][1];			\
    (dst)[3][2] = (s)[3][2];			\
    (dst)[3][3] = (s)[3][3];			\
} else


#endif
