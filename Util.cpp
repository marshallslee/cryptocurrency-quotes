#include <algorithm>
#include <QString>
#include "Util.h"

int getDigitLength(QString& strDecimal)
{
    std::reverse(strDecimal.begin(), strDecimal.end());
    int digitCountsToSub = 0;

    for(int i=0; i<strDecimal.length(); ++i)
    {
        if(strDecimal[i] != '0')
        {
            break;
        }
        ++digitCountsToSub;
    }

    // "0.0..."에서 앞의 "0."도 추가로 빼줘야 소수점 자리수가 최종적으로 계산된다.
    return strDecimal.length() - digitCountsToSub - 2;
}

int getDigitLength(QString&& strDecimal)
{
    if(!strDecimal.contains('.'))
    {
        return 0;
    }

    std::reverse(strDecimal.begin(), strDecimal.end());
    int digitCountsToSub = 0;

    for(int i=0; i<strDecimal.length(); ++i)
    {
        if(strDecimal[i] != '0')
        {
            break;
        }
        ++digitCountsToSub;
    }

    // "0.0..."에서 앞의 "0."도 추가로 빼줘야 소수점 자리수가 최종적으로 계산된다.
    return strDecimal.length() - digitCountsToSub - 2;
}

int getDecimalLengthFromPrice(double& price)
{
    if(price >= 0 && price < 0.1)
    {
        return 4;
    }
    else if (price >= 0.1 && price < 1)
    {
        return 3;
    }
    else if (price >= 1 && price < 10)
    {
        return 2;
    }
    else if (price >= 10 && price < 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
