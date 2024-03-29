#include "CThMktBinanceFutures.h"
#include "Util.h"

using namespace std;

CThMktBinanceFutures::CThMktBinanceFutures()
{
    mBinanceFuturesWS_Url = "wss://fstream.binance.com/stream?streams=" + mStream;

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
    if (mpTimer->isActive())
    {
        mpTimer->stop();
    }

    mbContinue = false;
    wait(1000);
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

void CThMktBinanceFutures::slotTimer500mSec(void)
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

void CThMktBinanceFutures::run()
{
    QObject::connect(this, SIGNAL(sigGetAllBinanceFuturesPairs()), this, SLOT(getAllBinanceFuturesPairs()), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigConnectWS()), this, SLOT(connectWS()), Qt::QueuedConnection);

    while (mbContinue)
    {
        msleep(1);
        switch (GetStatusBinanceFutures())
        {
        case BinanceFuturesStatus_en::Init:
            SetStatusBinanceFutures(BinanceFuturesStatus_en::WaitForMktPairs);
            emit sigGetAllBinanceFuturesPairs();
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

void CThMktBinanceFutures::reconnectWS(void)
{
    emit sigLog1(tr("[바이낸스 선물] %1 스트림으로 재연결").arg(mBinanceFuturesWS_Url.toString()));
    // 여기다가 reconnect 하는 로직을 구현
    mBinanceFuturesWS.close();
    mBinanceFuturesWS.open(mBinanceFuturesWS_Url);
}

void CThMktBinanceFutures::onConnected(void)
{
    emit sigLog1("WS Binance Futures Connected");
}

void CThMktBinanceFutures::onDisconnected(void)
{
    mBinanceFuturesWS.open(mBinanceFuturesWS_Url);
}

void CThMktBinanceFutures::onTextMessageReceived(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto stream = json_doc.object()["stream"].toString();

    if(stream.contains("@depth20@100ms"))
    {
        emit sigBinanceFuturesOrderbook(imessage);
    }
    else if(stream.contains("@aggTrade"))
    {
        emit sigBinanceFuturesTicker(imessage);
    }
}

void CThMktBinanceFutures::onPongReceived(quint64, const QByteArray&)
{

}

void CThMktBinanceFutures::getAllBinanceFuturesPairs()
{
    // 페어 리스트를 불러온 상태가 아니라면
    if(mbGotPairList == false) {
        QNetworkRequest request;
        QNetworkAccessManager* iManager = new QNetworkAccessManager(this);

        request.setUrl(QUrl(mUrlV1 + tr("fapi/v1/exchangeInfo")));
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
                            QJsonObject symbolData = sArray1[i].toObject();
                            QString contractType = symbolData["contractType"].toString();
                            if(contractType != "PERPETUAL")
                            {
                                continue;
                            }

                            QJsonArray arrFilterData = symbolData["filters"].toArray();

                            for (auto data : arrFilterData)
                            {
                                QString filterType = data.toObject()["filterType"].toString();
                                if(filterType == "PRICE_FILTER")
                                {
                                    QString strTickSize = data.toObject()["tickSize"].toString();
                                    iPair1.strTickSize = strTickSize;

                                    int tickSize = getDigitLength(std::move(strTickSize));
                                    iPair1.tickSize = tickSize;
                                }
                                else if (filterType == "LOT_SIZE")
                                {
                                    QString strStepSize = data.toObject()["stepSize"].toString();
                                    iPair1.strStepSize = strStepSize;

                                    int stepSize = getDigitLength(std::move(strStepSize));
                                    iPair1.stepSize = stepSize;
                                }
                            }

                            iPair1.orgName = symbolData["symbol"].toString().toLower();
                            iPair1.quote_symbol = symbolData["quoteAsset"].toString();
                            if (iPair1.quote_symbol != "USDT")
                            {
                                continue;
                            }
                            iPair1.base_symbol = symbolData["baseAsset"].toString();
                            iPair1.name = iPair1.base_symbol + tr("/") + iPair1.quote_symbol;
                            mBinanceFuturesPairs_um[iPair1.orgName] = iPair1;
                            ++mSubsPairs;
                        }
                        emit sigLog1(tr("Rcvd BinanceFuturesPair Counts = %1").arg(mCountPairs));
                        emit sigCreatePairsBinanceFutures(&mBinanceFuturesPairs_um);
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

void CThMktBinanceFutures::setStream(QString stream)
{
    mStream = stream;
    mBinanceFuturesWS_Url = "wss://fstream.binance.com/stream?streams=" + mStream;
}

void CThMktBinanceFutures::setCurrentPair(QString currentPair)
{
    mCurrentBinanceFuturesPair = currentPair;
}

