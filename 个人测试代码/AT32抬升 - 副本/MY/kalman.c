#include<sys.h>
#include<kalman.h>
#include<math.h>

void kalman_init(struct kalman_1*kalman,float A,float B,float H,float Q,float R,float X,float P)
{
    kalman->A=A;
    kalman->B=B;
    kalman->H=H;
    kalman->Q=Q;
    kalman->R=R;
    kalman->X=X;
    kalman->P=P;
}

float kalman_oper(struct kalman_1*kalman,float Z,float U)
{
    kalman->X=kalman->A*kalman->X+kalman->B*U;
    kalman->P=kalman->A*kalman->P*kalman->A+kalman->Q;
    kalman->K=kalman->P*kalman->H/(kalman->H*kalman->P*kalman->H+kalman->R);
    kalman->X=kalman->X+kalman->K*(Z-kalman->H*kalman->X);
    kalman->P=(1-kalman->K*kalman->H)*kalman->P;
    return kalman->X;
}
