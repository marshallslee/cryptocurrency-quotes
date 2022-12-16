#ifndef CTHMKTUPBIT_H
#define CTHMKTUPBIT_H

#include "Variables.h"

using namespace std;

enum class UpbitStatus_en
{
    Init = 0,
    WaitForMktPairs,
    Ready,
};

class CThMktUpbit : public QThread
{
    Q_OBJECT
public:
    CThMktUpbit();
    ~CThMktUpbit();

protected:
    void run();

signals:
    void sigLog1(QString);
    void sigUpbitTextLabel(QString);
    // Mkt API
    void sigGETPairAll();
    // to WS
    void sigConnectWS();
    void sigCreatePairsUpbit(Pairs_um*);

public slots:
    void slotTimer500mSec(void);

    // Mkt API
    void getPairAll(void);

    // to WS
    void connectWS(void);

private:
    UpbitStatus_en mUpbitStatus = UpbitStatus_en::Init;
    UpbitStatus_en GetStatusUpbit(void);
    bool SetStatusUpbit(UpbitStatus_en iStatus);

private:
    QString mUrlV1 = "https://api.upbit.com/v1/";
    bool mbGotPairList = false;
    bool mbVerifyUntrnsOrder;

private: // Data Set
    std::unordered_map<QString, TradingPair_st> mUpbitPairs_um;
    int32_t mCountPairs = 0;

private: // Timer
    std::unique_ptr<QTimer> mpTimer;
    int32_t mCountTimer = 0;

public: // WebSocket
    QUrl mWS_Url;
    QWebSocket mWS;
    int64_t mCntObu = 0, mCntTrade = 0, mCntTicker = 0;
    bool mbStartCnt = false;

    void onConnected(void);
    void onDisconnected(void);
    void onTextMessageReceived(QString imessage);
    void onPongReceived(quint64, const QByteArray&);

    QString mUUID;
    QString generateUUID(void);
    QString mCurrentPair = "KRW-BTC";
};

#endif // CTHMKTUPBIT_H
