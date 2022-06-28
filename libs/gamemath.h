#ifndef _QTHWK_GAMEMATH_H_
#define _QTHWK_GAMEMATH_H_

#include<cmath>

namespace __gameMath
{
    const double eps = 1e-9;
    const double INF = 0x3f3f3f3f;
	
	bool isInteger(const double v)
	{
        return std::fabs(v-int(v+eps)) < eps;
	}
    double div(double x,double y)
    {
        return fabs(y)<eps?(INF):(x/y);
    }
}
using __gameMath::eps;
using __gameMath::INF;
using __gameMath::isInteger;
using __gameMath::div;

#endif // _QTHWK_GAMEMATH_H_
