#ifndef CDLGMAIN_H
#define CDLGMAIN_H

#include <QMainWindow>
#include <QTableWidgetItem>
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

private:
    // 호가창에 표시할 아이템 수
    const int numQuotes = 15;

    // 매도 컬럼
    const int colAsk = 0;
    // 호가 컬럼
    const int colPrice = 1;

    // 매수 컬럼
    const int colBid = 2;
        int mTwRowCount  = 0;

public:
    std::unique_ptr<CThMktUpbit> mpThMktUpbit;
    std::unique_ptr<CThMktBinance> mpThMktBinance;
    std::unique_ptr<CThMktBinanceFutures> mpThMktBinanceFutures;

    QTableWidgetItem *askPriceItemUpbit = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *askSizeItemUpbit = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *bidPriceItemUpbit = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *bidSizeItemUpbit = new QTableWidgetItem[numQuotes];

    QTableWidgetItem *askPriceItemBinance = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *askSizeItemBinance = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *bidPriceItemBinance = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *bidSizeItemBinance = new QTableWidgetItem[numQuotes];

    QTableWidgetItem *askPriceItemBinanceFutures = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *askSizeItemBinanceFutures = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *bidPriceItemBinanceFutures = new QTableWidgetItem[numQuotes];
    QTableWidgetItem *bidSizeItemBinanceFutures = new QTableWidgetItem[numQuotes];
};
#endif // CDLGMAIN_H
