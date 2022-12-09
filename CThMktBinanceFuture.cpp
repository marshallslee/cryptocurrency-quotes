#include "CThMktBinanceFutures.h"

using namespace std;

CThMktBinanceFutures::CThMktBinanceFutures()
{
    mBinanceFuturesWS_Url = "wss://fstream.binance.com/stream?streams=btcusdt@depth20@100ms";

    QObject::connect(&mBinanceFuturesWS, &QWebSocket::connected, this, &CThMktBinanceFutures::onConnected);
    QObject::connect(&mBinanceFuturesWS, &QWebSocket::disconnected, this, &CThMktBinanceFutures::onDisconnected);
    QObject::connect(&mBinanceFuturesWS, &QWebSocket::textMessageReceived, this, &CThMktBinanceFutures::onTextMessageReceived);
    QObject::connect(&mBinanceFuturesWS, &QWebSocket::pong, this, &CThMktBinanceFutures::onPongReceived);

    srand(QTime::currentTime().msec()
            + QTime::currentTime().second()
            + QTime::currentTime().minute()
            + QTime::currentTime().hour() + 777);

    mpTimer = std::make_unique<QTimer>();
    QObject::connect(mpTimer.get(), SIGNAL(timeout()), this, SLOT(slotTimer500mSec()));
    mpTimer->start(500); // 500 for 500ms
}

CThMktBinanceFutures::~CThMktBinanceFutures()
{

}

BinanceFuturesStatus_en CThMktBinanceFutures::GetStatusBinanceFutures(void)
{
    return mBinanceFuturesStatus;
}

bool CThMktBinanceFutures::SetStatusBinanceFutures(BinanceFuturesStatus_en iStatus)
{
    mBinanceFuturesStatus = iStatus;

    switch (mBinanceFuturesStatus)
    {
    case BinanceFuturesStatus_en::Init:
        emit sigLog1("BinanceFuturesStatus_en::Init");
        break;

    case BinanceFuturesStatus_en::WaitForMktPairs:
        emit sigLog1("BinanceFuturesStatus_en::WaitForMktPairs");
        break;

    case BinanceFuturesStatus_en::Ready:
        emit sigLog1("BinanceFuturesStatus_en::Ready:");
        break;

    default:
        break;
    }

    return true;
}

void CThMktBinanceFutures::slotTimer500mSec(void) {
    if (mbStartCnt) {
        ++mCountTimer;
        if (mCountTimer % 2 == 0) {
            mCountTimer = 0;
        }
    }
}

void CThMktBinanceFutures::run() {
    QObject::connect(this, SIGNAL(sigConnectWS()), this, SLOT(connectWS()), Qt::QueuedConnection);

    while (true) {
        msleep(1);
        switch (GetStatusBinanceFutures()) {
        case BinanceFuturesStatus_en::Init:
            SetStatusBinanceFutures(BinanceFuturesStatus_en::WaitForMktPairs);
            break;

        case BinanceFuturesStatus_en::WaitForMktPairs:
            SetStatusBinanceFutures(BinanceFuturesStatus_en::Ready);
            emit sigConnectWS();
            mbStartCnt = true;
            break;

        case BinanceFuturesStatus_en::Ready:
            break;

        default:
            break;
        }
    }
}

void CThMktBinanceFutures::connectWS(void)
{
    mBinanceFuturesWS.open(mBinanceFuturesWS_Url);
}

void CThMktBinanceFutures::onConnected(void) {
    emit sigLog1("WS Binance Futures Connected");
}

void CThMktBinanceFutures::onDisconnected(void)
{
    mBinanceFuturesWS.open(mBinanceFuturesWS_Url);
}

void CThMktBinanceFutures::onTextMessageReceived(QString imessage) {
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto stream = json_doc.object()["stream"].toString();

    if (stream == "btcusdt@depth20@100ms") {
        emit sigBinanceFuturesTextLabel(imessage);
    }
}

void CThMktBinanceFutures::onPongReceived(quint64, const QByteArray&) {

}
