#include "CTh1.h"

CTh1::CTh1()
{

}

CTh1::~CTh1()
{

}

void CTh1::run()
{
    emit sigLog1("Start Th1!!");

    while (true)
    {
        msleep(10); // OS scheduler

        // 개발요구사항
        // 0. 2회 연속 동일한 명령을 받는 경우 : XX 안 한다고 했지? 와 비슷한 반응 출력
        // 1. 3가지를 누적으로 모두 실행하게 하면 "반란이다!!" 를 출력
        // 2. "닥쳐라!!" 명령을 받으면 "알겠습니다."를 출력
        // 3. "반란이다!!"를 출력하지 않았는데 "닥쳐라!!" 명령을 받으면, "모르겠습니다." 출력
        // 4. "알겠습니다." 를 출력한 직후에는 모든 상황 초기화

        // ==> 개발문서 필요 : 요구사항 구현을 알기 쉽게 1 page 로 (방법은 알아서 - 다이어그램, 표, 서술 etc)
        // 로드맵 : State Transition Diagram (Finite State Machine)

        switch (mOrder_Clean)
        {
        case RcvdOrder::CleanOrder:
            mOrder_Clean = RcvdOrder::Init;
            emit sigLog1("Th1 does NOT want to CLEAN!!"); // 1회일경우
            // 2회 연속이상일경우 : CLEAN안한다고 했지?
            break;

        case RcvdOrder::WashOrder:
            mOrder_Clean = RcvdOrder::Init;
            emit sigLog1("Th1 does NOT want to WASH!!"); // 1회일경우
            // 2회 연속이상일경우 : WASH안한다고 했지?
            break;

        case RcvdOrder::ShopOrder:
            mOrder_Clean = RcvdOrder::Init;
            emit sigLog1("Th1 does NOT want to SHOP!!"); // 1회일경우
            // 2회 연속이상일경우 : SHOP안한다고 했지?
            break;

        default:
            break;
        }
    }
}

// CallBack_SetOrderClean 함수는 외부에서만 호출!!
void CTh1::CallBack_SetOrderClean(int32_t iOrder)
{
    mOrder_Clean = static_cast<RcvdOrder>(iOrder);
}
