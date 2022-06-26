#ifndef _QTHWK_GAMEMATH_H_
#define _QTHWK_GAMEMATH_H_

#include<cmath>

namespace __gameMath
{
	const double eps = 1e-7;
	
	bool isInteger(const double v)
	{
		return std::fabs(v-int(v+eps)) < 1e-5;
	}
	
}

#endif
