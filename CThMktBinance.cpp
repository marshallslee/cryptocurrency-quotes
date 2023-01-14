#include "CThMktBinance.h"

using namespace std;

CThMktBinance::CThMktBinance()
{
    mBinanceWS_Url = "wss://stream.binance.com:9443/stream?streams=" + mStream;

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

void CThMktBinance::slotTimer500mSec(void)
{
    if (mbStartCnt)
    {
        ++mCountTimer;
        if (mCountTimer % 2 == 0)
        {
            mCountTimer = 0;
        }
    }
}

void CThMktBinance::run()
{
    QObject::connect(this, SIGNAL(sigGetAllBinancePairs()), this, SLOT(getAllBinancePairs()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigConnectWS()), this, SLOT(connectWS()), Qt::QueuedConnection);

    while (true)
    {
        msleep(1);
        switch (GetStatusBinance())
        {
        case BinanceStatus_en::Init:
            SetStatusBinance(BinanceStatus_en::WaitForMktPairs);
            emit sigGetAllBinancePairs();
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

void CThMktBinance::reconnectWS(void)
{
    emit sigLog1(tr("Connecting to %1").arg(mBinanceWS_Url.toString()));
    // 여기다가 reconnect 하는 로직을 구현
    mBinanceWS.close();
    mBinanceWS.open(mBinanceWS_Url);
}

void CThMktBinance::onConnected(void)
{
    emit sigLog1("WS Binance Connected");
}

void CThMktBinance::onDisconnected(void)
{
    mBinanceWS.open(mBinanceWS_Url);
}

void CThMktBinance::onTextMessageReceived(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto stream = json_doc.object()["stream"].toString();

    if(stream.contains("@depth20@100ms"))
    {
        emit sigBinanceOrderbook(imessage);
    }
    else if(stream.contains("@aggTrade"))
    {
        emit sigBinanceTicker(imessage);
    }
}

void CThMktBinance::onPongReceived(quint64, const QByteArray&)
{

}

void CThMktBinance::getAllBinancePairs()
{
    // 페어 리스트를 불러온 상태가 아니라면
    if(mbGotPairList == false)
    {
        QNetworkRequest request;
        QNetworkAccessManager* iManager = new QNetworkAccessManager(this);

        request.setUrl(QUrl(mUrlV1 + tr("api/v3/exchangeInfo")));
        request.setRawHeader("Content-Type", "application/json;charset=UTF-8");
        QObject::connect(iManager, &QNetworkAccessManager::finished, this, [=, this](QNetworkReply *reply)
        {
            if (reply->error() == QNetworkReply::NoError)
            {
                QString iStr1 = reply->readAll();
                if (iStr1 != "" && iStr1 != "{}")
                {
                    auto json_doc = QJsonDocument::fromJson(iStr1.toUtf8());
                    QJsonArray sArray1 = json_doc.object()["symbols"].toArray();
                    mCountPairs = sArray1.size();
                    if (mCountPairs > 0)
                    {
                        TradingPair_st iPair1;
                        for (int32_t i = 0; i < mCountPairs; ++i)
                        {
                            iPair1.orgName = sArray1[i].toObject()["symbol"].toString().toLower();
                            iPair1.quote_symbol = sArray1[i].toObject()["quoteAsset"].toString();
                            if (iPair1.quote_symbol != "USDT")
                            {
                                continue;
                            }
                            iPair1.base_symbol = sArray1[i].toObject()["baseAsset"].toString();
                            iPair1.name = iPair1.base_symbol + tr("/") + iPair1.quote_symbol;
                            mBinancePairs_um[iPair1.orgName] = iPair1;
                            ++mSubsPairs;
                        }
                        emit sigLog1(tr("Rcvd BinancePair Counts = %1").arg(mCountPairs));
                        emit sigCreatePairsBinance(&mBinancePairs_um);
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

void CThMktBinance::setStream(QString stream)
{
    mStream = stream;
    mBinanceWS_Url = "wss://stream.binance.com:9443/stream?streams=" + mStream;
}
