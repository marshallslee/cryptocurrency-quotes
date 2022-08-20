#include "CThMktBinance.h"

using namespace std;

CThMktBinance::CThMktBinance()
{
    mBinanceWS_Url = "wss://stream.binance.com:9443/stream?streams=btcusdt@depth20@100ms";

    QObject::connect(&mBinanceWS, &QWebSocket::connected, this, &CThMktBinance::onConnected);
    QObject::connect(&mBinanceWS, &QWebSocket::disconnected, this, &CThMktBinance::onDisconnected);
    QObject::connect(&mBinanceWS, &QWebSocket::textMessageReceived, this, &CThMktBinance::onTextMessageReceived);
    QObject::connect(&mBinanceWS, &QWebSocket::pong, this, &CThMktBinance::onPongReceived);

    srand(QTime::currentTime().msec()
            + QTime::currentTime().second()
            + QTime::currentTime().minute()
            + QTime::currentTime().hour() + 777);

    mpTimer = std::make_unique<QTimer>();
    QObject::connect(mpTimer.get(), SIGNAL(timeout()), this, SLOT(slotTimer500mSec()));
    mpTimer->start(500); // 500 for 500ms
}

CThMktBinance::~CThMktBinance()
{

}

BinanceStatus_en CThMktBinance::GetStatusBinance(void)
{
    return mBinanceStatus;
}

bool CThMktBinance::SetStatusBinance(BinanceStatus_en iStatus)
{
    mBinanceStatus = iStatus;

    switch (mBinanceStatus)
    {
    case BinanceStatus_en::Init:
        emit sigLog1("BinanceStatus_en::Init");
        break;

    case BinanceStatus_en::WaitForMktPairs:
        emit sigLog1("BinanceStatus_en::WaitForMktPairs");
        break;

    case BinanceStatus_en::Ready:
        emit sigLog1("BinanceStatus_en::Ready:");
        break;

    default:
        break;
    }

    return true;
}

void CThMktBinance::slotTimer500mSec(void) {
    if (mbStartCnt) {
        ++mCountTimer;
        if (mCountTimer % 2 == 0) {
            mCountTimer = 0;
        }
    }
}

void CThMktBinance::run() {
    QObject::connect(this, SIGNAL(sigConnectWS()), this, SLOT(connectWS()), Qt::QueuedConnection);

    while (true) {
        msleep(1);
        switch (GetStatusBinance()) {
        case BinanceStatus_en::Init:
            SetStatusBinance(BinanceStatus_en::WaitForMktPairs);
            break;

        case BinanceStatus_en::WaitForMktPairs:
            SetStatusBinance(BinanceStatus_en::Ready);
            emit sigConnectWS();
            mbStartCnt = true;
            break;

        case BinanceStatus_en::Ready:
            break;

        default:
            break;
        }
    }
}

void CThMktBinance::connectWS(void)
{
    mBinanceWS.open(mBinanceWS_Url);
}

void CThMktBinance::onConnected(void) {
    emit sigLog1("WS Binance Connected");
}

void CThMktBinance::onDisconnected(void)
{
    mBinanceWS.open(mBinanceWS_Url);
}

void CThMktBinance::onTextMessageReceived(QString imessage) {
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto stream = json_doc.object()["stream"].toString();

    if (stream == "btcusdt@depth20@100ms") {
        emit sigBinanceTextLabel(imessage);
    }
}

void CThMktBinance::onPongReceived(quint64, const QByteArray&) {

}
