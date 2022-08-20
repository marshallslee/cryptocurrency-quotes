#ifndef CDLGMAIN_H
#define CDLGMAIN_H

#include <QMainWindow>
#include "CThMktUpbit.h"
#include "CThMktBinance.h"
#include "CThMktBinanceFutures.h"

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
    void sigBinanceFuturesTextLabel(QString);

private slots:
    void slotBtnStart(void);

public slots:
    void slotLog1(QString iStr);
    void upbitBTCPrice(QString price);
    void binanceBTCPrice(QString price);
    void binanceFuturesBTCPrice(QString price);

public:
    std::unique_ptr<CThMktUpbit> mpThMktUpbit;
    std::unique_ptr<CThMktBinance> mpThMktBinance;
    std::unique_ptr<CThMktBinanceFutures> mpThMktBinanceFutures;
};
#endif // CDLGMAIN_H
