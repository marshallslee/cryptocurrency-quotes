#ifndef CTHMKTBINANCE_H
#define CTHMKTBINANCE_H

#include "Variables.h"

using namespace std;

enum class BinanceStatus_en
{
    Init = 0,
    WaitForMktPairs,
    Ready,
};

class CThMktBinance : public QThread
{
    Q_OBJECT
public:
    CThMktBinance();
    ~CThMktBinance();

protected:
    void run();

private:
    bool mbGotPairList = false;

signals:
    void sigLog1(QString);
    void sigBinanceTextLabel(QString);

    void sigConnectWS();

public slots:
//    void slotTimer500mSec(void);

    // Mkt API
//    void getPairAll(void);

    // to WS
    void connectWS(void);

private:
    BinanceStatus_en mBinanceStatus = BinanceStatus_en::Init;
    BinanceStatus_en GetStatusBinance(void);
    bool SetStatusBinance(BinanceStatus_en iStatus);

private: // Data Set
    std::unordered_map<QString, TradingPair_st> mUpbitPairs_um;
    int32_t mCountPairs = 0;

private: // Timer
    std::unique_ptr<QTimer> mpTimer;
    int32_t mCountTimer = 0;

public: // WebSocket
    QUrl mBinanceWS_Url;
    QWebSocket mBinanceWS;
    int64_t mCntObu = 0, mCntTrade = 0, mCntTicker = 0;
    bool mbStartCnt = false;

    void onConnected(void);
    void onDisconnected(void);
    void onTextMessageReceived(QString imessage);
    void onPongReceived(quint64, const QByteArray&);

    QString mUUID;
    QString generateUUID(void);
};

#endif // CTHMKTBINANCE_H
