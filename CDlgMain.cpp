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
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(sigUpbitTextLabel(QString)), this, SLOT(upbitBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigUpbitTextLabel(QString)), this, SLOT(upbitBTCPrice(QString)), Qt::QueuedConnection);

    ui->tUpbitPrice->horizontalHeader()->setVisible(false);
    ui->tUpbitPrice->verticalHeader()->setVisible(false);
    ui->tUpbitPrice->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tUpbitPrice->setColumnCount(4);
    ui->tUpbitPrice->setRowCount(30);
    ui->tUpbitPrice->setColumnWidth(0, 0);
    ui->tUpbitPrice->setColumnWidth(1, 80);
    ui->tUpbitPrice->setColumnWidth(2, 80);
    ui->tUpbitPrice->setColumnWidth(3, 80);

    QTableWidgetItem *upbitBidSizeTitleItem = new QTableWidgetItem;
    upbitBidSizeTitleItem->setText("매수 잔량");
    ui->tUpbitPrice->setItem(0, 1, upbitBidSizeTitleItem);

    QTableWidgetItem *upbitPriceTitleItem = new QTableWidgetItem;
    upbitPriceTitleItem->setText("호가");
    ui->tUpbitPrice->setItem(0, 2, upbitPriceTitleItem);

    QTableWidgetItem *upbitAskSizeTitleItem = new QTableWidgetItem;
    upbitAskSizeTitleItem->setText("매도 잔량");
    ui->tUpbitPrice->setItem(0, 3, upbitAskSizeTitleItem);

    QObject::connect(this, SIGNAL(sigBinanceTextLabel(QString)), this, SLOT(binanceBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigBinanceTextLabel(QString)), this, SLOT(binanceBTCPrice(QString)), Qt::QueuedConnection);

    ui->tBinancePrice->horizontalHeader()->setVisible(false);
    ui->tBinancePrice->verticalHeader()->setVisible(false);
    ui->tBinancePrice->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tBinancePrice->setColumnCount(4);
    ui->tBinancePrice->setRowCount(30);
    ui->tBinancePrice->setColumnWidth(0, 0);
    ui->tBinancePrice->setColumnWidth(1, 80);
    ui->tBinancePrice->setColumnWidth(2, 80);
    ui->tBinancePrice->setColumnWidth(3, 80);

    QTableWidgetItem *binanceBidSizeTitleItem = new QTableWidgetItem;
    binanceBidSizeTitleItem->setText("매수 잔량");
    ui->tBinancePrice->setItem(0, 1, binanceBidSizeTitleItem);

    QTableWidgetItem *binancePriceTitleItem = new QTableWidgetItem;
    binancePriceTitleItem->setText("호가");
    ui->tBinancePrice->setItem(0, 2, binancePriceTitleItem);

    QTableWidgetItem *binanceAskSizeTitleItem = new QTableWidgetItem;
    binanceAskSizeTitleItem->setText("매도 잔량");
    ui->tBinancePrice->setItem(0, 3, binanceAskSizeTitleItem);

    QObject::connect(this, SIGNAL(sigBinanceFuturesTextLabel(QString)), this, SLOT(binanceFuturesBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinanceFutures.get(), SIGNAL(sigBinanceFuturesTextLabel(QString)), this, SLOT(binanceFuturesBTCPrice(QString)), Qt::QueuedConnection);

    ui->tBinanceFuturesPrice->horizontalHeader()->setVisible(false);
    ui->tBinanceFuturesPrice->verticalHeader()->setVisible(false);
    ui->tBinanceFuturesPrice->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tBinanceFuturesPrice->setColumnCount(4);
    ui->tBinanceFuturesPrice->setRowCount(30);
    ui->tBinanceFuturesPrice->setColumnWidth(0, 0);
    ui->tBinanceFuturesPrice->setColumnWidth(1, 80);
    ui->tBinanceFuturesPrice->setColumnWidth(2, 80);
    ui->tBinanceFuturesPrice->setColumnWidth(3, 80);

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

    rowCount = ui->tUpbitPrice->rowCount();

    QTableWidgetItem *binanceFuturesBidSizeTitleItem = new QTableWidgetItem;
    binanceFuturesBidSizeTitleItem->setText("매수 잔량");
    ui->tBinanceFuturesPrice->setItem(0, 1, binanceFuturesBidSizeTitleItem);

    QTableWidgetItem *binanceFuturesPriceTitleItem = new QTableWidgetItem;
    binanceFuturesPriceTitleItem->setText("호가");
    ui->tBinanceFuturesPrice->setItem(0, 2, binanceFuturesPriceTitleItem);

    QTableWidgetItem *binanceFuturesAskSizeTitleItem = new QTableWidgetItem;
    binanceFuturesAskSizeTitleItem->setText("매도 잔량");
    ui->tBinanceFuturesPrice->setItem(0, 3, binanceFuturesAskSizeTitleItem);

    for(int i=0; i<numQuotes; ++i) {
        ui->tUpbitPrice->setItem(rowCount-(i + numQuotes + 1), colAsk, askSizeItemUpbit + i);
        ui->tUpbitPrice->setItem(rowCount-(i + numQuotes + 1), colPrice, askPriceItemUpbit + i);
        ui->tUpbitPrice->setItem(rowCount-(numQuotes - i), colPrice, bidPriceItemUpbit + i);
        ui->tUpbitPrice->setItem(rowCount-(numQuotes - i), colBid, bidSizeItemUpbit + i);

        ui->tBinancePrice->setItem(rowCount-(i + numQuotes + 1), colAsk, askSizeItemBinance + i);
        ui->tBinancePrice->setItem(rowCount-(i + numQuotes + 1), colPrice, askPriceItemBinance + i);
        ui->tBinancePrice->setItem(rowCount-(numQuotes - i), colPrice, bidPriceItemBinance + i);
        ui->tBinancePrice->setItem(rowCount-(numQuotes - i), colBid, bidSizeItemBinance + i);

        ui->tBinanceFuturesPrice->setItem(rowCount-(i + numQuotes + 1), colAsk, askSizeItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(rowCount-(i + numQuotes + 1), colPrice, askPriceItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(rowCount-(numQuotes - i), colPrice, bidPriceItemBinanceFutures + i);
        ui->tBinanceFuturesPrice->setItem(rowCount-(numQuotes - i), colBid, bidSizeItemBinanceFutures + i);
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

        ui->tUpbitPrice->item(rowCount - (i + numQuotes + 1), colAsk)->setText(strAskSize);
        ui->tUpbitPrice->item(rowCount - (i + numQuotes + 1), colPrice)->setText(strAskPrice);
        ui->tUpbitPrice->item(rowCount - (numQuotes - i), colPrice)->setText(strBidPrice);
        ui->tUpbitPrice->item(rowCount - (numQuotes - i), colBid)->setText(strBidSize);
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

        ui->tBinanceFuturesPrice->item(rowCount - (i + numQuotes + 1), colAsk)->setText(ask_size.toString());
        ui->tBinanceFuturesPrice->item(rowCount - (i + numQuotes + 1), colPrice)->setText(ask_price.toString());
        ui->tBinanceFuturesPrice->item(rowCount - (numQuotes - i), colPrice)->setText(bid_price.toString());
        ui->tBinanceFuturesPrice->item(rowCount - (numQuotes - i), colBid)->setText(bid_size.toString());
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
        \
        ui->tBinanceFuturesPrice->item(rowCount - (i + numQuotes + 1), colAsk)->setText(ask_size.toString());
        ui->tBinanceFuturesPrice->item(rowCount - (i + numQuotes + 1), colPrice)->setText(ask_price.toString());
        ui->tBinanceFuturesPrice->item(rowCount - (numQuotes - i), colPrice)->setText(bid_price.toString());
        ui->tBinanceFuturesPrice->item(rowCount - (numQuotes - i), colBid)->setText(bid_size.toString());
    }
}

void CDlgMain::slotLog1(QString iStr) {
    ui->teLog1->moveCursor(QTextCursor::End);
    ui->teLog1->insertPlainText(iStr + "\n");

    ui->teLog1->moveCursor(QTextCursor::End);
}
