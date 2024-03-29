#include "CDlgMain.h"
#include "ui_CDlgMain.h"
#include "Variables.h"
#include "Util.h"

using namespace std;

// Constructor
CDlgMain::CDlgMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CDlgMain)
{
    ui->setupUi(this);

    mpThMktUpbit = std::make_unique<CThMktUpbit>();
    mpThMktBinance = std::make_unique<CThMktBinance>();
    mpThMktBinanceFutures = std::make_unique<CThMktBinanceFutures>();

    QObject::connect(this, SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigUpbitOrderbook(QString)), this, SLOT(slotUpbitOrderbook(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigBinanceOrderbook(QString)), this, SLOT(slotBinanceOrderbook(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigBinanceFuturesOrderbook(QString)), this, SLOT(slotBinanceFuturesOrderbook(QString)), Qt::QueuedConnection);

    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigUpbitOrderbook(QString)), this, SLOT(slotUpbitOrderbook(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigCreatePairsUpbit(Pairs_um*)), this, SLOT(slotCreatePairsUpbit(Pairs_um*)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigUpbitTicker(QString)), this, SLOT(slotUpbitTicker(QString)), Qt::QueuedConnection);

    QObject::connect(mpThMktBinance.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigBinanceOrderbook(QString)), this, SLOT(slotBinanceOrderbook(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigCreatePairsBinance(Pairs_um*)), this, SLOT(slotCreatePairsBinance(Pairs_um*)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigBinanceTicker(QString)), this, SLOT(slotBinanceTicker(QString)), Qt::QueuedConnection);

    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigBinanceFuturesOrderbook(QString)), this, SLOT(slotBinanceFuturesOrderbook(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigCreatePairsBinanceFutures(Pairs_um*)), this, SLOT(slotCreatePairsBinanceFutures(Pairs_um*)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigBinanceFuturesTicker(QString)), this, SLOT(slotBinanceFuturesTicker(QString)), Qt::QueuedConnection);

    mTblRowCount = ui->tUpbitPrice->rowCount();
    ui->tUpbitPrice->setColumnWidth(0, 80);
    ui->tUpbitPrice->setColumnWidth(1, 80);
    ui->tUpbitPrice->setColumnWidth(2, 80);

    ui->tBinancePrice->setColumnWidth(0, 80);
    ui->tBinancePrice->setColumnWidth(1, 80);
    ui->tBinancePrice->setColumnWidth(2, 80);

    ui->tBinanceFuturesPrice->setColumnWidth(0, 80);
    ui->tBinanceFuturesPrice->setColumnWidth(1, 80);
    ui->tBinanceFuturesPrice->setColumnWidth(2, 80);

    mpAskPriceItemUpbit = new QTableWidgetItem[mNumQuotes];
    mpAskSizeItemUpbit = new QTableWidgetItem[mNumQuotes];
    mpBidPriceItemUpbit = new QTableWidgetItem[mNumQuotes];
    mpBidSizeItemUpbit = new QTableWidgetItem[mNumQuotes];

    mpAskPriceItemBinance = new QTableWidgetItem[mNumQuotes];
    mpAskSizeItemBinance = new QTableWidgetItem[mNumQuotes];
    mpBidPriceItemBinance = new QTableWidgetItem[mNumQuotes];
    mpBidSizeItemBinance = new QTableWidgetItem[mNumQuotes];

    mpAskPriceItemBinanceFutures = new QTableWidgetItem[mNumQuotes];
    mpAskSizeItemBinanceFutures = new QTableWidgetItem[mNumQuotes];
    mpBidPriceItemBinanceFutures = new QTableWidgetItem[mNumQuotes];
    mpBidSizeItemBinanceFutures = new QTableWidgetItem[mNumQuotes];

    QFont upbitFont = ui->tUpbitPrice->font();
    upbitFont.setPointSize(10);
    ui->tUpbitPrice->setFont(upbitFont);

    QFont binanceFont = ui->tBinancePrice->font();
    binanceFont.setPointSize(10);
    ui->tBinancePrice->setFont(binanceFont);

    QFont binanceFuturesFont = ui->tBinanceFuturesPrice->font();
    binanceFuturesFont.setPointSize(10);
    ui->tBinanceFuturesPrice->setFont(binanceFuturesFont);

    QHeaderView *header;

    // 업비트 호가창 셀 높이 지정
    header = ui->tUpbitPrice->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(10);

    // 바이낸스 호가창 셀 높이 지정
    header = ui->tBinancePrice->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(10);

    // 바이낸스 퓨처 호가창 셀 높이 지정
    header = ui->tBinanceFuturesPrice->verticalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setDefaultSectionSize(10);

    for(int i=0; i<mNumQuotes; ++i)
    {
        ui->tUpbitPrice->setItem(mTblRowCount-(i + mNumQuotes + 1), mColAsk, mpAskSizeItemUpbit + i);
        ui->tUpbitPrice->setItem(mTblRowCount-(i + mNumQuotes + 1), mColPrice, mpAskPriceItemUpbit + i);
        ui->tUpbitPrice->setItem(mTblRowCount-(mNumQuotes - i), mColPrice, mpBidPriceItemUpbit + i);
        ui->tUpbitPrice->setItem(mTblRowCount-(mNumQuotes - i), mColBid, mpBidSizeItemUpbit + i);

        ui->tBinancePrice->setItem(mTblRowCount-(i + mNumQuotes + 1), mColAsk, mpAskSizeItemBinance + i);
        ui->tBinancePrice->setItem(mTblRowCount-(i + mNumQuotes + 1), mColPrice, mpAskPriceItemBinance + i);
        ui->tBinancePrice->setItem(mTblRowCount-(mNumQuotes - i), mColPrice, mpBidPriceItemBinance + i);
        ui->tBinancePrice->setItem(mTblRowCount-(mNumQuotes - i), mColBid, mpBidSizeItemBinance + i);

        ui->tBinanceFuturesPrice->setItem(mTblRowCount-(i + mNumQuotes + 1), mColAsk, mpAskSizeItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(mTblRowCount-(i + mNumQuotes + 1), mColPrice, mpAskPriceItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(mTblRowCount-(mNumQuotes - i), mColPrice, mpBidPriceItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(mTblRowCount-(mNumQuotes - i), mColBid, mpBidSizeItemBinanceFutures + i);

        // align items right
        ui->tUpbitPrice->item(mTblRowCount-(i + mNumQuotes + 1), mColAsk)->setTextAlignment(Qt::AlignRight);
        ui->tUpbitPrice->item(mTblRowCount-(i + mNumQuotes + 1), mColPrice)->setTextAlignment(Qt::AlignRight);
        ui->tUpbitPrice->item(mTblRowCount-(mNumQuotes - i), mColPrice)->setTextAlignment(Qt::AlignRight);
        ui->tUpbitPrice->item(mTblRowCount-(mNumQuotes - i), mColBid)->setTextAlignment(Qt::AlignRight);

        ui->tBinancePrice->item(mTblRowCount-(i + mNumQuotes + 1), mColAsk)->setTextAlignment(Qt::AlignRight);
        ui->tBinancePrice->item(mTblRowCount-(i + mNumQuotes + 1), mColPrice)->setTextAlignment(Qt::AlignRight);
        ui->tBinancePrice->item(mTblRowCount-(mNumQuotes - i), mColPrice)->setTextAlignment(Qt::AlignRight);
        ui->tBinancePrice->item(mTblRowCount-(mNumQuotes - i), mColBid)->setTextAlignment(Qt::AlignRight);

        ui->tBinanceFuturesPrice->item(mTblRowCount-(i + mNumQuotes + 1), mColAsk)->setTextAlignment(Qt::AlignRight);
        ui->tBinanceFuturesPrice->item(mTblRowCount-(i + mNumQuotes + 1), mColPrice)->setTextAlignment(Qt::AlignRight);
        ui->tBinanceFuturesPrice->item(mTblRowCount-(mNumQuotes - i), mColPrice)->setTextAlignment(Qt::AlignRight);
        ui->tBinanceFuturesPrice->item(mTblRowCount-(mNumQuotes - i), mColBid)->setTextAlignment(Qt::AlignRight);
    }
}

// Destructor - 클래스 인스턴스가 종료/파괴
CDlgMain::~CDlgMain()
{
    delete[] mpAskPriceItemUpbit;
    delete[] mpAskSizeItemUpbit;
    delete[] mpBidPriceItemUpbit;
    delete[] mpBidSizeItemUpbit;

    delete[] mpAskPriceItemBinance;
    delete[] mpAskSizeItemBinance;
    delete[] mpBidPriceItemBinance;
    delete[] mpBidSizeItemBinance;

    delete[] mpAskPriceItemBinanceFutures;
    delete[] mpAskSizeItemBinanceFutures;
    delete[] mpBidPriceItemBinanceFutures;
    delete[] mpBidSizeItemBinanceFutures;
    delete ui;
}

void CDlgMain::slotBtnStart(void)
{
    emit sigLog1("Start the Application!");
    mpThMktUpbit->start();
    mpThMktBinance->start();
    mpThMktBinanceFutures->start();
}

void CDlgMain::slotUpbitOrderbook(QString price)
{
    auto json_doc = QJsonDocument::fromJson(price.toUtf8());

    if(mpUpbitPairs != nullptr)
    {
        std::unordered_map<QString, TradingPair_st>::iterator symbolData = mpUpbitPairs->find(mpThMktUpbit->mCurrentUpbitPair);
        int tickSize;

        QLocale locale(QLocale::English);

        QJsonArray arr = json_doc.object()["obu"].toArray();
        for(int i=0; i<arr.size(); ++i)
        {
            QJsonObject tempItem = arr[i].toObject();
            auto ask_price = tempItem["ap"].toDouble();
            auto bid_price = tempItem["bp"].toDouble();

            // 원화 마켓인 경우
            if(mCurrentUpbitPair.startsWith("KRW"))
            {
                tickSize = getDecimalLengthFromPrice(ask_price);
            }
            // 원화 마켓이 아닌 경우
            else
            {
                tickSize = symbolData->second.tickSize;
            }

            QString strAskPrice = locale.toString(ask_price, 'f', tickSize);
            QString strBidPrice = locale.toString(bid_price, 'f', tickSize);

            auto ask_size = tempItem["as"].toDouble();
            QString strAskSize = locale.toString(ask_size, 'f', 3);

            auto bid_size = tempItem["bs"].toDouble();
            QString strBidSize = locale.toString(bid_size, 'f', 3);

            ui->tUpbitPrice->item(mTblRowCount - (i + mNumQuotes + 1), mColAsk)->setText(strAskSize);
            ui->tUpbitPrice->item(mTblRowCount - (i + mNumQuotes + 1), mColPrice)->setText(strAskPrice);
            ui->tUpbitPrice->item(mTblRowCount - (mNumQuotes - i), mColPrice)->setText(strBidPrice);
            ui->tUpbitPrice->item(mTblRowCount - (mNumQuotes - i), mColBid)->setText(strBidSize);
        }
    }
}

void CDlgMain::slotBinanceOrderbook(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto data = json_doc.object()["data"].toObject();
    auto askData = data["asks"].toArray();
    auto bidData = data["bids"].toArray();

    if(mpBinancePairs != nullptr)
    {
        std::unordered_map<QString, TradingPair_st>::iterator symbolData = mpBinancePairs->find(mpThMktBinance->mCurrentBinancePair);
        int tickSize = symbolData->second.tickSize;
        int stepSize = symbolData->second.stepSize;

        QLocale locale(QLocale::English);
        for(int i=0; i<mNumQuotes; ++i)
        {
            auto ask_price = askData[i][0].toString().toDouble();
            auto ask_size = askData[i][1].toString().toDouble();
            auto bid_price = bidData[i][0].toString().toDouble();
            auto bid_size = bidData[i][1].toString().toDouble();

            QString strAskPrice = locale.toString(ask_price, 'f', tickSize);
            QString strAskSize = locale.toString(ask_size, 'f', stepSize);
            QString strBidPrice = locale.toString(bid_price, 'f', tickSize);
            QString strBidSize = locale.toString(bid_size, 'f', stepSize);

            ui->tBinancePrice->item(mTblRowCount - (i + mNumQuotes + 1), mColAsk)->setText(strAskSize);
            ui->tBinancePrice->item(mTblRowCount - (i + mNumQuotes + 1), mColPrice)->setText(strAskPrice);
            ui->tBinancePrice->item(mTblRowCount - (mNumQuotes - i), mColPrice)->setText(strBidPrice);
            ui->tBinancePrice->item(mTblRowCount - (mNumQuotes - i), mColBid)->setText(strBidSize);
        }
    }
}

void CDlgMain::slotBinanceFuturesOrderbook(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto data = json_doc.object()["data"].toObject();
    auto askData = data["a"].toArray();
    auto bidData = data["b"].toArray();

    if(mpBinanceFuturesPairs != nullptr)
    {
        std::unordered_map<QString, TradingPair_st>::iterator symbolData = mpBinanceFuturesPairs->find(mpThMktBinanceFutures->mCurrentBinanceFuturesPair);
        int tickSize = symbolData->second.tickSize;
        int stepSize = symbolData->second.stepSize;

        QLocale locale(QLocale::English);
        for(int i=0; i<mNumQuotes; ++i)
        {
            auto ask_price = askData[i][0].toString().toDouble();
            auto ask_size = askData[i][1].toString().toDouble();
            auto bid_price = bidData[i][0].toString().toDouble();
            auto bid_size = bidData[i][1].toString().toDouble();

            QString strAskPrice = locale.toString(ask_price, 'f', tickSize);
            QString strAskSize = locale.toString(ask_size, 'f', stepSize);
            QString strBidPrice = locale.toString(bid_price, 'f', tickSize);
            QString strBidSize = locale.toString(bid_size, 'f', stepSize);

            ui->tBinanceFuturesPrice->item(mTblRowCount - (i + mNumQuotes + 1), mColAsk)->setText(strAskSize);
            ui->tBinanceFuturesPrice->item(mTblRowCount - (i + mNumQuotes + 1), mColPrice)->setText(strAskPrice);
            ui->tBinanceFuturesPrice->item(mTblRowCount - (mNumQuotes - i), mColPrice)->setText(strBidPrice);
            ui->tBinanceFuturesPrice->item(mTblRowCount - (mNumQuotes - i), mColBid)->setText(strBidSize);
        }
    }
}

void CDlgMain::slotLog1(QString iStr)
{
    ui->teLog1->moveCursor(QTextCursor::End);
    ui->teLog1->insertPlainText(iStr + "\n");

    ui->teLog1->moveCursor(QTextCursor::End);
}

void CDlgMain::slotCreatePairsUpbit(Pairs_um* pairs)
{
    mpUpbitPairs = pairs;

    ui->upbitPairList->clear();

    for (auto item : *mpUpbitPairs)
    {
        ui->upbitPairList->addItem(QString(item.second.name));
    }

    ui->upbitPairList->sortItems(Qt::AscendingOrder);
}

void CDlgMain::slotCreatePairsBinance(Pairs_um* pairs)
{
    mpBinancePairs = pairs;

    ui->binancePairList->clear();

    for (auto item : *mpBinancePairs) {
        ui->binancePairList->addItem(QString(item.second.name));
    }

    ui->binancePairList->sortItems(Qt::AscendingOrder);
}

void CDlgMain::slotCreatePairsBinanceFutures(Pairs_um* pairs)
{
    mpBinanceFuturesPairs = pairs;

    ui->binanceFuturesPairList->clear();

    for (auto item : *mpBinanceFuturesPairs)
    {
        ui->binanceFuturesPairList->addItem(QString(item.second.name));
    }

    ui->binanceFuturesPairList->sortItems(Qt::AscendingOrder);
}

void CDlgMain::slotUpbitPairChanged(QListWidgetItem *item) {
    QString pair = item->text();  // BTC/KRW와 같은 형식으로 페어명을 가져옴
    QStringList tokens = pair.split(u'/'); // ["BTC", "KRW"]

    pair = tokens.at(1) + "-" + tokens.at(0); // KRW-BTC
    emit sigLog1(tr("[업비트] 현재 수신중인 종목 코드가 %1로 변경되었습니다.").arg(pair));
    mpThMktUpbit->mCurrentUpbitPair = pair;

    QTableWidgetItem *tblAskPrice = ui->tUpbitPrice->item(mNumQuotes-1, 1);
    QString strAskPrice = tblAskPrice->data(0).toString();
    tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
    tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));


    QTableWidgetItem *tblBidPrice = ui->tUpbitPrice->item(mNumQuotes, 1);
    QString strBidPrice = tblBidPrice->data(0).toString();
    tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
    tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
}

void CDlgMain::slotUpbitTicker(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());

    if(mpUpbitPairs != nullptr)
    {

        QString strMarketCode = json_doc.object()["cd"].toString();
        auto tradePrice = json_doc.object()["tp"].toVariant().toDouble();

        std::unordered_map<QString, TradingPair_st>::iterator symbolData = mpUpbitPairs->find(strMarketCode);

        int tickSize;

        // 원화 마켓인 경우
        if(mCurrentUpbitPair.startsWith("KRW"))
        {
            tickSize = getDecimalLengthFromPrice(tradePrice);
        }
        // 원화 마켓이 아닌 경우
        else
        {
            tickSize = symbolData->second.tickSize;
        }

        QLocale locale(QLocale::English);
        QString strTradePrice = locale.toString(tradePrice, 'f', tickSize);


        QTableWidgetItem *tblAskPrice = ui->tUpbitPrice->item(mNumQuotes-1, 1);
        QString strAskPrice = tblAskPrice->data(0).toString();

        QTableWidgetItem *tblBidPrice = ui->tUpbitPrice->item(mNumQuotes, 1);
        QString strBidPrice = tblBidPrice->data(0).toString();

        emit sigLog1(tr("[업비트] 종목코드: %1, 현재가: %2, 매도호가: %3, 매수호가: %4").arg(strMarketCode).arg(strTradePrice).arg(strAskPrice).arg(strBidPrice));

        if(tblAskPrice != nullptr && tblBidPrice != nullptr)
        {
            if(strAskPrice == strTradePrice)
            {
                tblAskPrice->setBackground(QBrush(QColor(0, 0, 255)));
                tblAskPrice->setForeground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
            }

            else if(strBidPrice == strTradePrice)
            {
                tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));
                tblBidPrice->setBackground(QBrush(QColor(0, 0, 255)));
                tblBidPrice->setForeground(QBrush(QColor(255, 255, 255)));
            }

            else
            {
                tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));
                tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
            }
        }
        else
        {
            emit sigLog1("tblAskPrice or tblBidPrice is nullptr");
        }
    }
}

void CDlgMain::slotBinancePairChanged(QListWidgetItem *item) {
    QString pair = item->text();  // BTC/KRW와 같은 형식으로 페어명을 가져옴
    QStringList tokens = pair.split(u'/'); // ["BTC", "KRW"]

    pair = tokens.at(0).toLower() + tokens.at(1).toLower(); // 예시: btcusdt

    QString newStream = tr("%1@depth20@100ms/%2@aggTrade").arg(pair).arg(pair);
    mpThMktBinance->setCurrentPair(pair);
    mpThMktBinance->setStream(newStream);
    mpThMktBinance->reconnectWS();
    emit sigLog1(tr("[바이낸스] 현재 수신 스트림이 %1로 변경되었습니다.").arg(mpThMktBinance->mStream));

    QTableWidgetItem *tblAskPrice = ui->tBinancePrice->item(mNumQuotes-1, 1);
    QString strAskPrice = tblAskPrice->data(0).toString();
    tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
    tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));


    QTableWidgetItem *tblBidPrice = ui->tBinancePrice->item(mNumQuotes, 1);
    QString strBidPrice = tblBidPrice->data(0).toString();
    tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
    tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
}

void CDlgMain::slotBinanceFuturesPairChanged(QListWidgetItem *item) {
    QString pair = item->text();  // BTC/KRW와 같은 형식으로 페어명을 가져옴
    QStringList tokens = pair.split(u'/'); // ["BTC", "KRW"]

    pair = tokens.at(0).toLower() + tokens.at(1).toLower(); // 예시: btcusdt

    QString newStream = tr("%1@depth20@100ms/%2@aggTrade").arg(pair).arg(pair);
    mpThMktBinanceFutures->setCurrentPair(pair);
    mpThMktBinanceFutures->setStream(newStream);
    mpThMktBinanceFutures->reconnectWS();
    emit sigLog1(tr("[바이낸스 선물] 현재 수신 스트림이 %1로 변경되었습니다.").arg(mpThMktBinance->mStream));

    QTableWidgetItem *tblAskPrice = ui->tBinanceFuturesPrice->item(mNumQuotes-1, 1);
    QString strAskPrice = tblAskPrice->data(0).toString();
    tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
    tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));


    QTableWidgetItem *tblBidPrice = ui->tBinanceFuturesPrice->item(mNumQuotes, 1);
    QString strBidPrice = tblBidPrice->data(0).toString();
    tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
    tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
}

void CDlgMain::slotBinanceTicker(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto tradePrice  = json_doc.object()["data"].toObject()["p"].toString().toDouble();

    auto symbol = json_doc.object()["data"].toObject()["s"].toString().toLower();
    if(mpBinancePairs != nullptr)
    {
        std::unordered_map<QString, TradingPair_st>::iterator symbolData = mpBinancePairs->find(symbol);
        int tickSize = symbolData->second.tickSize;

        QLocale locale(QLocale::English);
        QString strTradePrice = locale.toString(tradePrice, 'f', tickSize);

        QTableWidgetItem *tblAskPrice = ui->tBinancePrice->item(mNumQuotes-1, 1);
        QString strAskPrice = tblAskPrice->data(0).toString();

        QTableWidgetItem *tblBidPrice = ui->tBinancePrice->item(mNumQuotes, 1);
        QString strBidPrice = tblBidPrice->data(0).toString();

    //    emit sigLog1(tr("[바이낸스] 종목: %1, 현재가: %2, 매도호가: %3, 매수호가: %4").arg(symbol).arg(strTradePrice).arg(strAskPrice).arg(strBidPrice));

        if(tblAskPrice != nullptr && tblBidPrice != nullptr)
        {
            // 매도호가가 현재가와 동일한 경우
            if(strAskPrice == strTradePrice)
            {
                // 매도호가란을 청색 바탕에 흰 글자 처리
                tblAskPrice->setBackground(QBrush(QColor(0, 0, 255)));
                tblAskPrice->setForeground(QBrush(QColor(255, 255, 255)));

                // 매수호가란을 흰색 바탕에 검은 글자 처리
                tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
            }

            // 매수호가가 현재가와 동일한 경우
            else if(strBidPrice == strTradePrice)
            {
                // 매도호가란을 흰색 바탕에 검은 글자 처리
                tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));

                // 매수호가란을 청색 바탕에 흰 글자 처리
                tblBidPrice->setBackground(QBrush(QColor(0, 0, 255)));
                tblBidPrice->setForeground(QBrush(QColor(255, 255, 255)));
            }

            // 그 외의 칸 (현재가와 같지 않은 매도/매수호가란)
            else
            {
                // 매도/매수호가란을 각각 흰색 바탕에 검은 글자 처리
                tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));
                tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
            }
        }
    }
}

void CDlgMain::slotBinanceFuturesTicker(QString imessage)
{
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto tradePrice  = json_doc.object()["data"].toObject()["p"].toString().toDouble();

    auto symbol = json_doc.object()["data"].toObject()["s"].toString().toLower();
    if(mpBinanceFuturesPairs != nullptr)
    {
        std::unordered_map<QString, TradingPair_st>::iterator symbolData = mpBinanceFuturesPairs->find(symbol);
        int tickSize = symbolData->second.tickSize;

        QLocale locale(QLocale::English);
        QString strTradePrice = locale.toString(tradePrice);

        QTableWidgetItem *tblAskPrice = ui->tBinanceFuturesPrice->item(mNumQuotes-1, 1);
        QString strAskPrice = tblAskPrice->data(0).toString();

        QTableWidgetItem *tblBidPrice = ui->tBinanceFuturesPrice->item(mNumQuotes, 1);
        QString strBidPrice = tblBidPrice->data(0).toString();

        if(tblAskPrice != nullptr && tblBidPrice != nullptr)
        {
            if(strAskPrice == strTradePrice)
            {
                tblAskPrice->setBackground(QBrush(QColor(0, 0, 255)));
                tblAskPrice->setForeground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
            }

            else if(strBidPrice == strTradePrice)
            {
                tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));
                tblBidPrice->setBackground(QBrush(QColor(0, 0, 255)));
                tblBidPrice->setForeground(QBrush(QColor(255, 255, 255)));
            }

            else
            {
                tblAskPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblAskPrice->setForeground(QBrush(QColor(0, 0, 0)));
                tblBidPrice->setBackground(QBrush(QColor(255, 255, 255)));
                tblBidPrice->setForeground(QBrush(QColor(0, 0, 0)));
            }
        }
    }
}
