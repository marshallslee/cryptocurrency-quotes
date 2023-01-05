#ifndef CTHMKTBINANCEFUTURES_H
#define CTHMKTBINANCEFUTURES_H
#include <QThread>
#include <QTimer>
#include <QWebSocket>
#include <QtNetwork>

#include "Variables.h"

using namespace std;

enum class BinanceFuturesStatus_en
{
    Init = 0,
    WaitForMktPairs,
    Ready,
};

class CThMktBinanceFutures : public QThread
{
    Q_OBJECT
public:
    CThMktBinanceFutures();
    ~CThMktBinanceFutures();

protected:
    void run();

signals:
    void sigLog1(QString);
    void sigBinanceFuturesOrderbook(QString);
    void sigConnectWS();
    void sigGetAllBinanceFuturesPairs();
    void sigCreatePairsBinanceFutures(Pairs_um*);

public slots:
    void slotTimer500mSec(void);
    void getAllBinanceFuturesPairs(void);

    // to WS
    void connectWS(void);

private:
    BinanceFuturesStatus_en mBinanceFuturesStatus = BinanceFuturesStatus_en::Init;
    BinanceFuturesStatus_en GetStatusBinanceFutures(void);
    bool SetStatusBinanceFutures(BinanceFuturesStatus_en iStatus);
    std::unordered_map<QString, TradingPair_st> mBinanceFuturesPairs_um;

    // 바이낸스 API 도메인
    QString mUrlV1 = "https://fapi.binance.com/";

    // 페어 리스트를 불러왔는지 여부를 담는 변수.
    bool mbGotPairList = false;

private: // Timer
    std::unique_ptr<QTimer> mpTimer;
    int32_t mCountTimer = 0;
    int32_t mCountPairs = 0;
    int32_t mSubsPairs  = 0;

public: // WebSocket
    QUrl mBinanceFuturesWS_Url;
    QWebSocket mBinanceFuturesWS;
    int64_t mCntObu = 0, mCntTrade = 0, mCntTicker = 0;
    bool mbStartCnt = false;

    void onConnected(void);
    void onDisconnected(void);
    void onTextMessageReceived(QString imessage);
    void onPongReceived(quint64, const QByteArray&);
};

#endif // CTHMKTBINANCEFUTURES_H
