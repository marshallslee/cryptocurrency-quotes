#include "CDlgMain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if 0
    CDlgMain w; // stack 에 저장(용량에 한계)
    w.show();
#else
    // C++98, Traditional way
    //CDlgMain *w = new CDlgMain(); // heap 에 저장, heap 에서 클래스의 인스턴스 메모리 할당받음

    //w->show();


    // C++17 이후
    // heap 에 저장, heap 에서 클래스의 인스턴스 메모리 할당받음 - 파괴자에서 heap 메모리 반환
    // Boost Library - 2010에도 사용
    std::unique_ptr<CDlgMain> w1 = std::make_unique<CDlgMain>();

    w1->show();
#endif
    return a.exec();
}
