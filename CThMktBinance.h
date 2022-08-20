#ifndef CTHMKTBINANCE_H
#define CTHMKTBINANCE_H
#include <QThread>
#include <QTimer>
#include <QWebSocket>
#include <QtNetwork>

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

signals:
    void sigLog1(QString);
    void sigBinanceTextLabel(QString);
    void sigConnectWS();

public slots:
    void slotTimer500mSec(void);

    // to WS
    void connectWS(void);

private:
    BinanceStatus_en mBinanceStatus = BinanceStatus_en::Init;
    BinanceStatus_en GetStatusBinance(void);
    bool SetStatusBinance(BinanceStatus_en iStatus);

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
};

#endif // CTHMKTBINANCE_H
