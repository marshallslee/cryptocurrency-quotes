#include "CDlgMain.h"
#include "ui_CDlgMain.h"
#include <QTableWidgetItem>

// Constructor
CDlgMain::CDlgMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CDlgMain)
{
    ui->setupUi(this);

    mpTh1 = std::make_unique<CTh1>();
    mpThMktUpbit = std::make_unique<CThMktUpbit>();
    mpThMktBinance = std::make_unique<CThMktBinance>();

    QObject::connect(this, SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpTh1.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(sigUpbitTextLabel(QString)), this, SLOT(upbitBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigUpbitTextLabel(QString)), this, SLOT(upbitBTCPrice(QString)), Qt::QueuedConnection);

    ui->tUpbitPrice->horizontalHeader()->setVisible(false);
    ui->tUpbitPrice->verticalHeader()->setVisible(false);
    ui->tUpbitPrice->setColumnCount(5);
    ui->tUpbitPrice->setRowCount(30);
    ui->tUpbitPrice->setColumnWidth(0, 0);
    ui->tUpbitPrice->setColumnWidth(1, 90);
    ui->tUpbitPrice->setColumnWidth(2, 90);
    ui->tUpbitPrice->setColumnWidth(3, 90);
    ui->tUpbitPrice->setColumnWidth(4, 90);

    QTableWidgetItem *upbitAskPriceTitleItem = new QTableWidgetItem;
    upbitAskPriceTitleItem->setText("매도 호가");
    ui->tUpbitPrice->setItem(0, 1, upbitAskPriceTitleItem);

    QTableWidgetItem *upbitAskSizeTitleItem = new QTableWidgetItem;
    upbitAskSizeTitleItem->setText("매도 잔량");
    ui->tUpbitPrice->setItem(0, 2, upbitAskSizeTitleItem);

    QTableWidgetItem *upbitBidPriceTitleItem = new QTableWidgetItem;
    upbitBidPriceTitleItem->setText("매수 호가");
    ui->tUpbitPrice->setItem(0, 3, upbitBidPriceTitleItem);

    QTableWidgetItem *upbitBidSizeTitleItem = new QTableWidgetItem;
    upbitBidSizeTitleItem->setText("매수 잔량");
    ui->tUpbitPrice->setItem(0, 4, upbitBidSizeTitleItem);

    QObject::connect(this, SIGNAL(sigBinanceTextLabel(QString)), this, SLOT(binanceBTCPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktBinance.get(), SIGNAL(sigBinanceTextLabel(QString)), this, SLOT(binanceBTCPrice(QString)), Qt::QueuedConnection);


    ui->tBinancePrice->horizontalHeader()->setVisible(false);
    ui->tBinancePrice->verticalHeader()->setVisible(false);
    ui->tBinancePrice->setColumnCount(5);
    ui->tBinancePrice->setRowCount(30);
    ui->tBinancePrice->setColumnWidth(0, 0);
    ui->tBinancePrice->setColumnWidth(1, 90);
    ui->tBinancePrice->setColumnWidth(2, 90);
    ui->tBinancePrice->setColumnWidth(3, 90);
    ui->tBinancePrice->setColumnWidth(4, 90);

    QTableWidgetItem *binanceAskPriceTitleItem = new QTableWidgetItem;
    binanceAskPriceTitleItem->setText("매도 호가");
    ui->tBinancePrice->setItem(0, 1, binanceAskPriceTitleItem);

    QTableWidgetItem *binanceAskSizeTitleItem = new QTableWidgetItem;
    binanceAskSizeTitleItem->setText("매도 잔량");
    ui->tBinancePrice->setItem(0, 2, binanceAskSizeTitleItem);

    QTableWidgetItem *binanceBidPriceTitleItem = new QTableWidgetItem;
    binanceBidPriceTitleItem->setText("매수 호가");
    ui->tBinancePrice->setItem(0, 3, binanceBidPriceTitleItem);

    QTableWidgetItem *binanceBidSizeTitleItem = new QTableWidgetItem;
    binanceBidSizeTitleItem->setText("매수 잔량");
    ui->tBinancePrice->setItem(0, 4, binanceBidSizeTitleItem);

    mpTh1->start();
}

// Destructor - 클래스 인스턴스가 종료/파괴
CDlgMain::~CDlgMain()
{
    delete ui;
}

void CDlgMain::slotBtnStart(void)
{
    emit sigLog1("Start DlgMain!!");
    mpThMktUpbit->start();
    mpThMktBinance->start();
}

void CDlgMain::slotBtnOrderClean(void)
{
    // Clean 작업!! - 왜 안되나요?
    // 작업중
    // 작업중 - 10분 걸림
    //for loop
    // step 1 - work thread

    // Thread 간 통신
    mpTh1->CallBack_SetOrderClean(static_cast<int32_t>(OrderToThread::Clean));
}

void CDlgMain::slotBtnOrderWash(void)
{
    mpTh1->CallBack_SetOrderClean(static_cast<int32_t>(OrderToThread::Wash));
}

void CDlgMain::slotBtnOrderShop(void)
{
    mpTh1->CallBack_SetOrderClean(static_cast<int32_t>(OrderToThread::Shop));
}

void CDlgMain::upbitBTCPrice(QString price) {
    auto json_doc = QJsonDocument::fromJson(price.toUtf8());

    QLocale locale(QLocale::English);

    QJsonArray arr = json_doc.object()["obu"].toArray();
    for(int i=0; i<arr.size(); i++) {
        QJsonObject tempItem = arr[i].toObject();
        auto ask_price = tempItem["ap"].toDouble();
        auto ask_size = tempItem["as"].toDouble();
        auto bid_price = tempItem["bp"].toDouble();
        auto bid_size = tempItem["bs"].toDouble();

        QTableWidgetItem *askPriceItem = new QTableWidgetItem;
        QString strAskPrice = locale.toString(ask_price, 'f', 0);
        askPriceItem->setText(strAskPrice);
        ui->tUpbitPrice->setItem(i+1, 1, askPriceItem);

        QTableWidgetItem *askSizeItem = new QTableWidgetItem;
        QString strAskSize = locale.toString(ask_size, 'f');
        askSizeItem->setText(strAskSize);
        ui->tUpbitPrice->setItem(i+1, 2, askSizeItem);

        QTableWidgetItem *bidPriceItem = new QTableWidgetItem;
        QString strBidPrice = locale.toString(bid_price, 'f', 0);
        bidPriceItem->setText(strBidPrice);
        ui->tUpbitPrice->setItem(i+1, 3, bidPriceItem);

        QTableWidgetItem *bidSizeItem = new QTableWidgetItem;
        QString strBidSize = locale.toString(bid_size, 'f');
        bidSizeItem->setText(strBidSize);
        ui->tUpbitPrice->setItem(i+1, 4, bidSizeItem);
    }
}

void CDlgMain::binanceBTCPrice(QString price) {

}

void CDlgMain::slotLog1(QString iStr)
{
    ui->teLog1->moveCursor(QTextCursor::End);
    ui->teLog1->insertPlainText(iStr + "\n");

    ui->teLog1->moveCursor(QTextCursor::End);
}
