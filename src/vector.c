#include "../include/vector.h"

#include <stdint.h>
#include <math.h>

float VECTOR3_VectorModulus(Vector3 vector3) {
    return sqrtf(vector3.x*vector3.x + vector3.y*vector3.y + vector3.z*vector3.z);
}