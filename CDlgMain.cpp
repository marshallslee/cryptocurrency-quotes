#include "CDlgMain.h"
#include "ui_CDlgMain.h"

using namespace std;

// Constructor
CDlgMain::CDlgMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CDlgMain) {
    ui->setupUi(this);

    mpThMktUpbit = std::make_unique<CThMktUpbit>();
    mpThMktBinance = std::make_unique<CThMktBinance>();
    mpThMktBinanceFutures = std::make_unique<CThMktBinanceFutures>();

    QObject::connect(this, SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigUpbitTextLabel(QString)), this, SLOT(upbitBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigBinanceTextLabel(QString)), this, SLOT(binanceBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(sigBinanceFuturesTextLabel(QString)), this, SLOT(binanceFuturesBTCPrice(QString)), Qt::QueuedConnection);

    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigUpbitTextLabel(QString)), this, SLOT(upbitBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigCreatePairsUpbit(Pairs_um*)), this, SLOT(slotCreatePairsUpbit(Pairs_um*)), Qt::QueuedConnection);

    QObject::connect(mpThMktBinance.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigBinanceTextLabel(QString)), this, SLOT(binanceBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigCreatePairsBinance(Pairs_um*)), this, SLOT(slotCreatePairsBinance(Pairs_um*)), Qt::QueuedConnection);

    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigBinanceFuturesTextLabel(QString)), this, SLOT(binanceFuturesBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigCreatePairsBinanceFutures(Pairs_um*)), this, SLOT(slotCreatePairsBinanceFutures(Pairs_um*)), Qt::QueuedConnection);

    tblRowCount = ui->tUpbitPrice->rowCount();
    ui->tUpbitPrice->setColumnWidth(0, 80);
    ui->tUpbitPrice->setColumnWidth(1, 80);
    ui->tUpbitPrice->setColumnWidth(2, 80);

    ui->tBinancePrice->setColumnWidth(0, 80);
    ui->tBinancePrice->setColumnWidth(1, 80);
    ui->tBinancePrice->setColumnWidth(2, 80);

    ui->tBinanceFuturesPrice->setColumnWidth(0, 80);
    ui->tBinanceFuturesPrice->setColumnWidth(1, 80);
    ui->tBinanceFuturesPrice->setColumnWidth(2, 80);

    askPriceItemUpbit = new QTableWidgetItem[numQuotes];
    askSizeItemUpbit = new QTableWidgetItem[numQuotes];
    bidPriceItemUpbit = new QTableWidgetItem[numQuotes];
    bidSizeItemUpbit = new QTableWidgetItem[numQuotes];

    askPriceItemBinance = new QTableWidgetItem[numQuotes];
    askSizeItemBinance = new QTableWidgetItem[numQuotes];
    bidPriceItemBinance = new QTableWidgetItem[numQuotes];
    bidSizeItemBinance = new QTableWidgetItem[numQuotes];

    askPriceItemBinanceFutures = new QTableWidgetItem[numQuotes];
    askSizeItemBinanceFutures = new QTableWidgetItem[numQuotes];
    bidPriceItemBinanceFutures = new QTableWidgetItem[numQuotes];
    bidSizeItemBinanceFutures = new QTableWidgetItem[numQuotes];

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

    for(int i=0; i<numQuotes; ++i) {
        ui->tUpbitPrice->setItem(tblRowCount-(i + numQuotes + 1), colAsk, askSizeItemUpbit + i);
        ui->tUpbitPrice->setItem(tblRowCount-(i + numQuotes + 1), colPrice, askPriceItemUpbit + i);
        ui->tUpbitPrice->setItem(tblRowCount-(numQuotes - i), colPrice, bidPriceItemUpbit + i);
        ui->tUpbitPrice->setItem(tblRowCount-(numQuotes - i), colBid, bidSizeItemUpbit + i);

        ui->tBinancePrice->setItem(tblRowCount-(i + numQuotes + 1), colAsk, askSizeItemBinance + i);
        ui->tBinancePrice->setItem(tblRowCount-(i + numQuotes + 1), colPrice, askPriceItemBinance + i);
        ui->tBinancePrice->setItem(tblRowCount-(numQuotes - i), colPrice, bidPriceItemBinance + i);
        ui->tBinancePrice->setItem(tblRowCount-(numQuotes - i), colBid, bidSizeItemBinance + i);

        ui->tBinanceFuturesPrice->setItem(tblRowCount-(i + numQuotes + 1), colAsk, askSizeItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(tblRowCount-(i + numQuotes + 1), colPrice, askPriceItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(tblRowCount-(numQuotes - i), colPrice, bidPriceItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(tblRowCount-(numQuotes - i), colBid, bidSizeItemBinanceFutures + i);
    }
}

// Destructor - 클래스 인스턴스가 종료/파괴
CDlgMain::~CDlgMain()
{
    delete[] askPriceItemUpbit;
    delete[] askSizeItemUpbit;
    delete[] bidPriceItemUpbit;
    delete[] bidSizeItemUpbit;

    delete[] askPriceItemBinance;
    delete[] askSizeItemBinance;
    delete[] bidPriceItemBinance;
    delete[] bidSizeItemBinance;

    delete[] askPriceItemBinanceFutures;
    delete[] askSizeItemBinanceFutures;
    delete[] bidPriceItemBinanceFutures;
    delete[] bidSizeItemBinanceFutures;
    delete ui;
}

void CDlgMain::slotBtnStart(void)
{
    emit sigLog1("Start the Application!");
    mpThMktUpbit->start();
    mpThMktBinance->start();
    mpThMktBinanceFutures->start();
}

void CDlgMain::upbitBTCPrice(QString price) {
    auto json_doc = QJsonDocument::fromJson(price.toUtf8());

    QLocale locale(QLocale::English);

    QJsonArray arr = json_doc.object()["obu"].toArray();
    for(int i=0; i<arr.size(); ++i) {
        QJsonObject tempItem = arr[i].toObject();
        auto ask_price = tempItem["ap"].toDouble();
        QString strAskPrice = locale.toString(ask_price, 'f', 0);

        auto ask_size = tempItem["as"].toDouble();
        QString strAskSize = locale.toString(ask_size, 'f', 3);

        auto bid_price = tempItem["bp"].toDouble();
        QString strBidPrice = locale.toString(bid_price, 'f', 0);

        auto bid_size = tempItem["bs"].toDouble();
        QString strBidSize = locale.toString(bid_size, 'f', 3);

        ui->tUpbitPrice->item(tblRowCount - (i + numQuotes + 1), colAsk)->setText(strAskSize);
        ui->tUpbitPrice->item(tblRowCount - (i + numQuotes + 1), colPrice)->setText(strAskPrice);
        ui->tUpbitPrice->item(tblRowCount - (numQuotes - i), colPrice)->setText(strBidPrice);
        ui->tUpbitPrice->item(tblRowCount - (numQuotes - i), colBid)->setText(strBidSize);
    }
}

void CDlgMain::binanceBTCPrice(QString imessage) {
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto data = json_doc.object()["data"].toObject();
    auto askData = data["asks"].toArray();
    auto bidData = data["bids"].toArray();

    QLocale locale(QLocale::English);
    for(int i=0; i<numQuotes; ++i) {
        auto ask_price = askData[i][0];
        auto ask_size = askData[i][1];
        auto bid_price = bidData[i][0];
        auto bid_size = bidData[i][1];

        ui->tBinancePrice->item(tblRowCount - (i + numQuotes + 1), colAsk)->setText(ask_size.toString());
        ui->tBinancePrice->item(tblRowCount - (i + numQuotes + 1), colPrice)->setText(ask_price.toString());
        ui->tBinancePrice->item(tblRowCount - (numQuotes - i), colPrice)->setText(bid_price.toString());
        ui->tBinancePrice->item(tblRowCount - (numQuotes - i), colBid)->setText(bid_size.toString());
    }
}

void CDlgMain::binanceFuturesBTCPrice(QString imessage) {
    auto json_doc = QJsonDocument::fromJson(imessage.toUtf8());
    auto data = json_doc.object()["data"].toObject();
    auto askData = data["a"].toArray();
    auto bidData = data["b"].toArray();

    QLocale locale(QLocale::English);
    for(int i=0; i<numQuotes; ++i) {
        auto ask_price = askData[i][0];
        auto ask_size = askData[i][1];
        auto bid_price = bidData[i][0];
        auto bid_size = bidData[i][1];

        ui->tBinanceFuturesPrice->item(tblRowCount - (i + numQuotes + 1), colAsk)->setText(ask_size.toString());
        ui->tBinanceFuturesPrice->item(tblRowCount - (i + numQuotes + 1), colPrice)->setText(ask_price.toString());
        ui->tBinanceFuturesPrice->item(tblRowCount - (numQuotes - i), colPrice)->setText(bid_price.toString());
        ui->tBinanceFuturesPrice->item(tblRowCount - (numQuotes - i), colBid)->setText(bid_size.toString());
    }
}

void CDlgMain::slotLog1(QString iStr) {
    ui->teLog1->moveCursor(QTextCursor::End);
    ui->teLog1->insertPlainText(iStr + "\n");

    ui->teLog1->moveCursor(QTextCursor::End);
}

void CDlgMain::slotCreatePairsUpbit(Pairs_um* pairs) {
    upbitPairs = pairs;

    ui->upbitPairList->clear();

    for (auto item : *upbitPairs)
        ui->upbitPairList->addItem(QString(item.second.name));

    ui->upbitPairList->sortItems(Qt::AscendingOrder);
}

void CDlgMain::slotCreatePairsBinance(Pairs_um* pairs) {
    binancePairs = pairs;

    ui->binancePairList->clear();

    for (auto item : *binancePairs) {
        ui->binancePairList->addItem(QString(item.second.name));
    }

    ui->binancePairList->sortItems(Qt::AscendingOrder);
}

void CDlgMain::slotCreatePairsBinanceFutures(Pairs_um* pairs) {
    binanceFuturesPairs = pairs;

    ui->binanceFuturesPairList->clear();

    for (auto item : *binanceFuturesPairs) {
        ui->binanceFuturesPairList->addItem(QString(item.second.name));
    }

    ui->binanceFuturesPairList->sortItems(Qt::AscendingOrder);
}

