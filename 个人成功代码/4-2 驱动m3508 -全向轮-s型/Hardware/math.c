#include "Math.h"

/**
 * @brief  10进制数字符串转10进制
 * 
 * @params string 10进制数字符串
 */
int my_atoi(char* string)
{
	uint8_t i = 0, p = 0;
	int num = 0;
	int flag = 1;
	if(string[0] == '-')
	{
		flag = -1;
		p++;
	}
	while(string[p] == '0')
	{
		p++;
	}
	while(string[p] != '\0')
	{
		num *=10;
		num += (string[p++]-48);
		i++;
	}
	num *= flag;
	return num;
}

/**
 * @brief  int16_t类型数值绝对值
 * 
 * @params num int16_t类型数值
 */
int16_t int16_abs(int16_t num)
{
	if(num > 0)return num;
	else return -num;
}

/**
 * @brief  float类型数值绝对值
 * 
 * @params num float类型数值
 */
float float_abs(float num)
{
	if(num > 0)return num;
	else return -num;
}

/**
 * @brief  int16_t类型数值限制函数
 * 
 * @params MAX 最大值
 * @params MIN 最小值
 * @params num int16_t类型数值
 */
int16_t int16_limit(int16_t MAX, int16_t MIN, int16_t num)
{
	if(num > MAX)return MAX;
	else if(num < MIN)return MIN;
	else return num;
}

/**
 * @brief  float类型数值限制函数
 * 
 * @params MAX 最大值
 * @params MIN 最小值
 * @params num float类型数值
 */
float float_limit(float num, float Min, float Max)
{
	if (num < Min)
    {
        return Min;
    }
    else if (num > Max)
    {
        return Max;
    }
	return num;
}

/**
 * @brief  快速计算以e为底的幂
 * 
 * @params num float类型数值
 */
float fast_expf(float num)
{
    // 将 e^x 转换成 2^(x / ln2)
    const float LOG2E = 1.4426950408889634f; // 1 / ln(2)
    float y = num * LOG2E;

    // 整数部分和小数部分
    int i = (int)y;
    float f = y - i;

    // 近似 2^f （用多项式）
    float poly = 1.0f + f * (0.69314718f + f * (0.24022651f + f * 0.05550411f));

    // 通过位运算构造 2^i
    uint32_t exp_bits = (i + 127) << 23;
    float pow2i = *((float*)&exp_bits);

    return pow2i * poly;
}

int float_to_uint(float x, float x_min, float x_max, int bits)
{
	float span = x_max - x_min;
	float offset = x_min;
	
	return (int)((x - offset)*((float)((1<<bits)-1))/ span);
}

float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	// converts unsigned int to float, given range and number of bits
	float span = x_max- x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

///**
// * @brief 将浮点数映射到整型
// *
// * @param x 浮点数
// * @param Float_1 浮点数1
// * @param Float_2 浮点数2
// * @param Int_1 整型1
// * @param Int_2 整型2
// * 
// * @return int32_t 整型
// */
//int32_t float_to_int(float x, float Float_1, float Float_2, int32_t Int_1, int32_t Int_2)
//{
//    float tmp = (x - Float_1) / (Float_2 - Float_1);
//    int32_t out = tmp * (float) (Int_2 - Int_1) + Int_1;
//    return (out);
//}
