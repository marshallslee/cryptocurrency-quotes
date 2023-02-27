#ifndef CTHMKTBINANCE_H
#define CTHMKTBINANCE_H
#include <QThread>
#include <QTimer>
#include <QWebSocket>
#include <QtNetwork>
#include "Variables.h"

using namespace std;

enum class BinanceStatus_en {
    Init = 0,
    WaitForMktPairs,
    Ready,
};

class CThMktBinance : public QThread {
    Q_OBJECT
public:
    CThMktBinance();
    ~CThMktBinance();

protected:
    void run();

signals:
    void sigLog1(QString);
    void sigBinanceOrderbook(QString);
    void sigGetAllBinancePairs();
    void sigConnectWS();
    void sigCreatePairsBinance(Pairs_um*);
    void sigBinanceTicker(QString);

public slots:
    void slotTimer500mSec(void);
    void getAllBinancePairs(void);

    // to WS
    void connectWS(void);
    void reconnectWS(void);

private:
    BinanceStatus_en mBinanceStatus = BinanceStatus_en::Init;
    BinanceStatus_en GetStatusBinance(void);
    bool SetStatusBinance(BinanceStatus_en iStatus);
    std::unordered_map<QString, TradingPair_st> mBinancePairs_um;

    // 바이낸스 API 도메인
    QString mUrlV1 = "https://api.binance.com/";

    // 페어 리스트를 불러왔는지 여부를 담는 변수.
    bool mbGotPairList = false;

    int32_t mCountPairs = 0;
    int32_t mSubsPairs  = 0;

private: // Timer
    std::unique_ptr<QTimer> mpTimer;
    int32_t mCountTimer = 0;

public: // WebSocket
    QUrl mBinanceWS_Url;
    QWebSocket mBinanceWS;
    int64_t mCntObu = 0, mCntTrade = 0, mCntTicker = 0;
    bool mbStartCnt = false;
    QString mCurrentBinancePair = "btcusdt";
    QString mStream = tr("%1@depth20@100ms/%2@aggTrade").arg(mCurrentBinancePair).arg(mCurrentBinancePair);

    void onConnected(void);
    void onDisconnected(void);
    void onTextMessageReceived(QString imessage);
    void onPongReceived(quint64, const QByteArray&);
    TradingPair_st getPairData(QString);

    void setStream(QString);
    void setCurrentPair(QString);
};

#endif // CTHMKTBINANCE_H
