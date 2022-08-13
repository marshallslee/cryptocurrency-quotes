#include "CThMktBinance.h"

using namespace std;

CThMktBinance::CThMktBinance()
{
    mBinanceWS_Url = "wss://stream.binance.com:9443/ws/btcusdt@depth";
    mUUID = generateUUID();

    QObject::connect(&mBinanceWS, &QWebSocket::connected, this, &CThMktBinance::onConnected);
    QObject::connect(&mBinanceWS, &QWebSocket::disconnected, this, &CThMktBinance::onDisconnected);
    QObject::connect(&mBinanceWS, &QWebSocket::binaryMessageReceived, this, &CThMktBinance::onTextMessageReceived);
    QObject::connect(&mBinanceWS, &QWebSocket::pong, this, &CThMktBinance::onPongReceived);

    srand(QTime::currentTime().msec()
            + QTime::currentTime().second()
            + QTime::currentTime().minute()
            + QTime::currentTime().hour() + 777);

//    mpTimer = std::make_unique<QTimer>();
//    QObject::connect(mpTimer.get(), SIGNAL(timeout()), this, SLOT(slotTimer500mSec()));
//    mpTimer->start(500); // 500 for 500ms
}

CThMktBinance::~CThMktBinance()
{

}

BinanceStatus_en CThMktBinance::GetStatusBinance(void)
{
    return mBinanceStatus;
}

//void CThMktBinance::slotTimer500mSec(void)
//{
//    if (mbStartCnt)
//    {
//        ++mCountTimer;
//        if (mCountTimer % 2 == 0)
//        {
//            emit sigLog1(tr("Pairs = %1, Obu = %2, Trd = %3, Tckr = %4")
//                         .arg(mCountPairs).arg(mCntObu).arg(mCntTrade).arg(mCntTicker));

//            mCountTimer = 0;
//        }
//    }
//}

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

void CThMktBinance::run() {
    QObject::connect(this, SIGNAL(sigConnectWS()),
                     this, SLOT(connectWS()), Qt::QueuedConnection);

    while (true) {
        msleep(1);
        switch (GetStatusBinance()) {
        case BinanceStatus_en::Init:
            SetStatusBinance(BinanceStatus_en::WaitForMktPairs);
            break;

        case BinanceStatus_en::WaitForMktPairs:
            if (mbGotPairList == true) {
                SetStatusBinance(BinanceStatus_en::Ready);
                emit sigConnectWS();
                mbStartCnt = true;
            }
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
    QJsonArray iArrayTotal, iArrayCodes;
    QJsonObject iObjTicket, iObjObu, iObjTrade, iObjTicker, iObjFormat;

    iObjTicket.insert("ticket", mUUID);

    int16_t idx1 = 0;
    for (auto&& item : mUpbitPairs_um)
    {
        iArrayCodes.insert(idx1++, item.first);
    }
    iObjObu.insert("type", "orderbook");
    iObjObu.insert("codes", iArrayCodes);
    iObjObu.insert("isOnlyRealtime", "true");

    iObjTrade.insert("type", "trade");
    iObjTrade.insert("codes", iArrayCodes);
    iObjTrade.insert("isOnlyRealtime", "true");

    iObjTicker.insert("type", "ticker");
    iObjTicker.insert("codes", iArrayCodes);
    iObjTicker.insert("isOnlyRealtime", "true");

    iObjFormat.insert("format", "SIMPLE");

    iArrayTotal.insert(0, iObjTicket);
    iArrayTotal.insert(1, iObjObu);
    iArrayTotal.insert(2, iObjTrade);
    iArrayTotal.insert(3, iObjTicker);
    iArrayTotal.insert(4, iObjFormat);

    QJsonDocument doc;
    doc.setArray(iArrayTotal);
    QString iStrFinal(doc.toJson());

    mBinanceWS.sendBinaryMessage(iStrFinal.toUtf8());
}

void CThMktBinance::onDisconnected(void)
{
    mBinanceWS.open(mBinanceWS_Url);
}

void CThMktBinance::onTextMessageReceived(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    if (json_doc.object()["cd"].toString() == "KRW-BTC" &&
            json_doc.object()["ty"].toString() == "orderbook") {
        emit sigBinanceTextLabel(imessage);
    }
    if (imessage.indexOf("orderbook") > 0)
    {
        ++mCntObu;
    }
    else if (imessage.indexOf("trade") > 0)
    {
        ++mCntTrade;
#if 0
        auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
        if (json_doc.object()["cd"].toString() == "KRW-BTC")
        {
            emit sigLog1(currentPrice);
            emit sigLog1(tr("BTC/KRW CurPrice = %1, TrdVol = %2")
                         .arg(json_doc.object()["tp"].toVariant().toString())
                         .arg(json_doc.object()["tv"].toVariant().toString()));
        }
#endif
    }
    else if (imessage.indexOf("ticker") > 0)
    {
        ++mCntTicker;
    }
}

void CThMktBinance::onPongReceived(quint64, const QByteArray&)
{

}

QString CThMktBinance::generateUUID(void)
{
    srand(QTime::currentTime().msec()
            + QTime::currentTime().second()
            + QTime::currentTime().minute()
            + QTime::currentTime().hour());

    QString iStr = tr("%1%2-%3-%4-%5-%6%7%8")
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'))
        .arg((ushort)(rand() + 1), 4, 16, (QLatin1Char)('0'));

    return iStr;
}
