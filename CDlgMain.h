#ifndef CDLGMAIN_H
#define CDLGMAIN_H

#include <QMainWindow>
#include "CTh1.h"
#include "CThMktUpbit.h"
#include "CThMktBinance.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CDlgMain; }
QT_END_NAMESPACE

enum class OrderToThread
{
    Clean = 1,
    Wash = 2,
    Shop = 3,
};

class CDlgMain : public QMainWindow
{
    Q_OBJECT

public:
    CDlgMain(QWidget *parent = nullptr);
    ~CDlgMain();

private:
    Ui::CDlgMain *ui;

signals:
    void sigLog1(QString);
    void sigUpbitTextLabel(QString);
    void sigBinanceTextLabel(QString);

private slots:
    void slotBtnStart(void);
    void slotBtnOrderClean(void);
    void slotBtnOrderWash(void);
    void slotBtnOrderShop(void);

public slots:
    void slotLog1(QString iStr);
    void upbitBTCPrice(QString price);
    void binanceBTCPrice(QString price);

public:
    //CTh1 th1; // stack instance
    std::unique_ptr<CTh1> mpTh1;
    std::unique_ptr<CThMktUpbit> mpThMktUpbit;
    std::unique_ptr<CThMktBinance> mpThMktBinance;

};
#endif // CDLGMAIN_H
