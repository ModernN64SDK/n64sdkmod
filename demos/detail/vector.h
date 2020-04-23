
#ifndef __vector__
#define __vector__

#include <ultra64.h>

typedef float vec2[2];
typedef float vec3[3];

extern void crossVec3(vec3 result, vec3 v1, vec3 v2);

#define COPY_VEC3(dst,src)			\
if (1) {					\
    (dst)[0] = (src)[0];			\
    (dst)[1] = (src)[1];			\
    (dst)[2] = (src)[2];			\
} else

#define NORMALIZE_VEC3(v)						\
if (1) {								\
    float f = sqrtf((v)[0]*(v)[0] + (v)[1]*(v)[1] + (v)[2]*(v)[2]);	\
    (v)[0] /= f;							\
    (v)[1] /= f;							\
    (v)[2] /= f;							\
} else

#define SUB_VEC3(result,v1,v2)			\
if (1) {					\
    (result)[0] = (v1)[0] - (v2)[0];		\
    (result)[1] = (v1)[1] - (v2)[1];		\
    (result)[2] = (v1)[2] - (v2)[2];		\
} else

#define ADD_VEC3(result,v1,v2)			\
if (1) {					\
    (result)[0] = (v1)[0] + (v2)[0];		\
    (result)[1] = (v1)[1] + (v2)[1];		\
    (result)[2] = (v1)[2] + (v2)[2];		\
} else
#endif
