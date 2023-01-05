#ifndef CDLGMAIN_H
#define CDLGMAIN_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QListWidgetItem>
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
    void sigCurrentPairChange(QString);

private slots:
    void slotBtnStart(void);
    void slotPairChanged(QListWidgetItem*);

public slots:
    void slotLog1(QString iStr);
    void slotUpbitOrderbook(QString price);
    void slotUpbitTicker(QString);
    void slotBinanceOrderbook(QString price);
    void slotBinanceFuturesOrderbook(QString price);
    void slotCreatePairsUpbit(Pairs_um*);
    void slotCreatePairsBinance(Pairs_um*);
    void slotCreatePairsBinanceFutures(Pairs_um*);

private:
    // 호가창에 표시할 아이템 수
    const int numQuotes = 15;

    // 매도 컬럼 인덱스
    const int colAsk = 0;

    // 호가 컬럼 인덱스
    const int colPrice = 1;

    // 매수 컬럼 인덱스
    const int colBid = 2;

    // 각 호가창 테이블별 row count
    int tblRowCount  = 0;

    QString mCurrentUpbitPair = "KRW-BTC";

public:
    std::unique_ptr<CThMktUpbit> mpThMktUpbit;
    std::unique_ptr<CThMktBinance> mpThMktBinance;
    std::unique_ptr<CThMktBinanceFutures> mpThMktBinanceFutures;

    QTableWidgetItem *askPriceItemUpbit;
    QTableWidgetItem *askSizeItemUpbit;
    QTableWidgetItem *bidPriceItemUpbit;
    QTableWidgetItem *bidSizeItemUpbit;

    QTableWidgetItem *askPriceItemBinance;
    QTableWidgetItem *askSizeItemBinance;
    QTableWidgetItem *bidPriceItemBinance;
    QTableWidgetItem *bidSizeItemBinance;

    QTableWidgetItem *askPriceItemBinanceFutures;
    QTableWidgetItem *askSizeItemBinanceFutures;
    QTableWidgetItem *bidPriceItemBinanceFutures;
    QTableWidgetItem *bidSizeItemBinanceFutures;

    Pairs_um* upbitPairs;
    Pairs_um* binancePairs;
    Pairs_um* binanceFuturesPairs;
};
#endif // CDLGMAIN_H
