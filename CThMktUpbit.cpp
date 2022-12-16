#include "CThMktUpbit.h"

using namespace std;

CThMktUpbit::CThMktUpbit()
{
    mWS_Url = "wss://api.upbit.com/websocket/v1";
    mUUID = generateUUID();

    QObject::connect(&mWS, &QWebSocket::connected, this, &CThMktUpbit::onConnected);
    QObject::connect(&mWS, &QWebSocket::disconnected, this, &CThMktUpbit::onDisconnected);
    QObject::connect(&mWS, &QWebSocket::binaryMessageReceived, this, &CThMktUpbit::onTextMessageReceived);
    QObject::connect(&mWS, &QWebSocket::pong, this, &CThMktUpbit::onPongReceived);

    srand(QTime::currentTime().msec()
            + QTime::currentTime().second()
            + QTime::currentTime().minute()
            + QTime::currentTime().hour() + 777);

    mpTimer = std::make_unique<QTimer>();
    QObject::connect(mpTimer.get(), SIGNAL(timeout()), this, SLOT(slotTimer500mSec()));
    mpTimer->start(500); // 500 for 500ms
}

CThMktUpbit::~CThMktUpbit()
{

}

UpbitStatus_en CThMktUpbit::GetStatusUpbit(void)
{
    return mUpbitStatus;
}

bool CThMktUpbit::SetStatusUpbit(UpbitStatus_en iStatus)
{
    mUpbitStatus = iStatus;

    switch (mUpbitStatus)
    {
    case UpbitStatus_en::Init:
        emit sigLog1("UpbitStatus_en::Init");
        break;

    case UpbitStatus_en::WaitForMktPairs:
        emit sigLog1("UpbitStatus_en::WaitForMktPairs");
        break;

    case UpbitStatus_en::Ready:
        emit sigLog1("UpbitStatus_en::Ready:");
        break;

    default:
        break;
    }

    return true;
}

void CThMktUpbit::run()
{
    QObject::connect(this, SIGNAL(sigGETPairAll()),
                     this, SLOT(getPairAll()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigConnectWS()),
                     this, SLOT(connectWS()), Qt::QueuedConnection);

    while (true)
    {
        msleep(1);
        switch (GetStatusUpbit())
        {
        case UpbitStatus_en::Init:
            SetStatusUpbit(UpbitStatus_en::WaitForMktPairs);
            emit sigGETPairAll();
            break;

        case UpbitStatus_en::WaitForMktPairs:
            if (mbGotPairList == true)
            {
                SetStatusUpbit(UpbitStatus_en::Ready);
                emit sigConnectWS();
                mbStartCnt = true;
            }
            break;

        case UpbitStatus_en::Ready:
            break;

        default:
            break;
        }
    }
}

void CThMktUpbit::slotTimer500mSec(void)
{
    if (mbStartCnt)
    {
        ++mCountTimer;
        if (mCountTimer % 2 == 0)
        {
            emit sigLog1(tr("Pairs = %1, Obu = %2, Trd = %3, Tckr = %4")
                         .arg(mCountPairs).arg(mCntObu).arg(mCntTrade).arg(mCntTicker));

            mCountTimer = 0;
        }
    }
}

void CThMktUpbit::getPairAll(void)
{
    if (mbGotPairList == false)
    {
        QNetworkRequest request;
        QNetworkAccessManager* iManager = new QNetworkAccessManager(this);

        request.setUrl(QUrl(mUrlV1 + tr("market/all")));
        request.setRawHeader("Content-Type", "application/json;charset=UTF-8");
        QObject::connect(iManager, &QNetworkAccessManager::finished,
                         this, [=, this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError)
            {
                QString iStr1 = reply->readAll();
                if (iStr1 != "" && iStr1 != "{}")
                {
                    auto json_doc = QJsonDocument::fromJson(iStr1.toUtf8());
                    mCountPairs = json_doc.array().size();
                    if (mCountPairs > 0)
                    {
                        TradingPair_st iPair1;
                        for (int32_t i = 0; i < mCountPairs; ++i)
                        {
                            iPair1.orgName = json_doc.array()[i].toObject()["market"].toString();
                            iPair1.quote_symbol = iPair1.orgName.left(iPair1.orgName.indexOf("-"));
                            iPair1.base_symbol = iPair1.orgName.right(iPair1.orgName.length() - iPair1.orgName.indexOf("-") - 1);
                            iPair1.name = iPair1.base_symbol + tr("/") + iPair1.quote_symbol;

                            mUpbitPairs_um[iPair1.orgName] = iPair1;
                        }
                        emit sigLog1(tr("Rcvd UpbitPair Counts = %1").arg(mCountPairs));
                        emit sigCreatePairsUpbit(&mUpbitPairs_um);
                        mbGotPairList = true;
                    }
                }
            }
            iManager->deleteLater();
            reply->deleteLater();
        });
        iManager->get(request);
    }
}

void CThMktUpbit::connectWS(void)
{
    mWS.open(mWS_Url);
}

void CThMktUpbit::onConnected(void)
{
    emit sigLog1("WS Upbit Connected");

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

    mWS.sendBinaryMessage(iStrFinal.toUtf8());
}

void CThMktUpbit::onDisconnected(void)
{
    mWS.open(mWS_Url);
}

void CThMktUpbit::onTextMessageReceived(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    if (json_doc.object()["cd"].toString() == mCurrentPair &&
            json_doc.object()["ty"].toString() == "orderbook") {
        emit sigUpbitTextLabel(imessage);
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

void CThMktUpbit::onPongReceived(quint64, const QByteArray&)
{

}

QString CThMktUpbit::generateUUID(void)
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
