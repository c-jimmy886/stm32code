#ifndef __MATH_H
#define __MATH_H

#include "stm32f10x.h"                  // Device header

int my_atoi(char* string);
int16_t int16_abs(int16_t num);
float float_abs(float num);
int16_t int16_limit(int16_t MAX, int16_t MIN, int16_t num);
float float_limit(float num, float Min, float Max);
float fast_expf(float num);
int float_to_uint(float x, float x_min, float x_max, int bits);
float uint_to_float(int x_int, float x_min, float x_max, int bits);

#endif

