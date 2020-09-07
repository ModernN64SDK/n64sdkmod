/*
 * vector.c - supporting vector routines
 */

#include "vector.h"

void crossVec3(vec3 result, vec3 a, vec3 b)
{
    vec3 tmp;
    
    tmp[0] = a[1] * b[2] - a[2] * b[1];
    tmp[1] = a[2] * b[0] - a[0] * b[2];
    tmp[2] = a[0] * b[1] - a[1] * b[0];

    COPY_VEC3(result, tmp);
}
