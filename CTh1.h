#ifndef CTH1_H
#define CTH1_H

#include <QThread>

enum class RcvdOrder
{
    Init = 0,
    CleanOrder = 1,
    WashOrder = 2,
    ShopOrder = 3,
};

class CTh1 : public QThread
{
    Q_OBJECT
public:
    CTh1();
    ~CTh1();

protected:
    void run();

signals:
    void sigLog1(QString);

private:
    RcvdOrder mOrder_Clean = RcvdOrder::Init;

public:
    void CallBack_SetOrderClean(int32_t iOrder);
};

#endif // CTH1_H
