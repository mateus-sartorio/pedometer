#pragma once

#include <stdint.h>
#include <math.h>

typedef struct Vector3_Struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Vector3;

float VECTOR3_VectorModulus(Vector3 vector3);