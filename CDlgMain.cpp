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

    QObject::connect(this, SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpTh1.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigLog1(QString)), this, SLOT(slotLog1(QString)), Qt::QueuedConnection);

    QObject::connect(this, SIGNAL(sigTextLabel(QString)), this, SLOT(slotPrice(QString)), Qt::QueuedConnection);
    QObject::connect(mpThMktUpbit.get(), SIGNAL(sigTextLabel(QString)), this, SLOT(slotPrice(QString)), Qt::QueuedConnection);

    ui->tPrice->setColumnCount(5);
    ui->tPrice->setRowCount(30);
    ui->tPrice->setColumnWidth(0, 0);
    ui->tPrice->setColumnWidth(1, 160);
    ui->tPrice->setColumnWidth(2, 160);
    ui->tPrice->setColumnWidth(3, 160);
    ui->tPrice->setColumnWidth(4, 160);

    QTableWidgetItem *askPriceTitleItem = new QTableWidgetItem;
    askPriceTitleItem->setText("매도 호가");
    ui->tPrice->setItem(0, 1, askPriceTitleItem);

    QTableWidgetItem *askSizeTitleItem = new QTableWidgetItem;
    askSizeTitleItem->setText("매도 잔량");
    ui->tPrice->setItem(0, 2, askSizeTitleItem);

    QTableWidgetItem *bidPriceTitleItem = new QTableWidgetItem;
    bidPriceTitleItem->setText("매수 호가");
    ui->tPrice->setItem(0, 3, bidPriceTitleItem);

    QTableWidgetItem *bidSizeTitleItem = new QTableWidgetItem;
    bidSizeTitleItem->setText("매수 잔량");
    ui->tPrice->setItem(0, 4, bidSizeTitleItem);

    ui->tPrice->horizontalHeader()->setVisible(false);
    ui->tPrice->verticalHeader()->setVisible(false);

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

void CDlgMain::slotPrice(QString price) {
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
        ui->tPrice->setItem(i+1, 1, askPriceItem);

        QTableWidgetItem *askSizeItem = new QTableWidgetItem;
        QString strAskSize = locale.toString(ask_size, 'f');
        askSizeItem->setText(strAskSize);
        ui->tPrice->setItem(i+1, 2, askSizeItem);

        QTableWidgetItem *bidPriceItem = new QTableWidgetItem;
        QString strBidPrice = locale.toString(bid_price, 'f', 0);
        bidPriceItem->setText(strBidPrice);
        ui->tPrice->setItem(i+1, 3, bidPriceItem);

        QTableWidgetItem *bidSizeItem = new QTableWidgetItem;
        QString strBidSize = locale.toString(bid_size, 'f');
        bidSizeItem->setText(strBidSize);
        ui->tPrice->setItem(i+1, 4, bidSizeItem);
    }
}

void CDlgMain::slotLog1(QString iStr)
{
    ui->teLog1->moveCursor(QTextCursor::End);
    ui->teLog1->insertPlainText(iStr + "\n");

    ui->teLog1->moveCursor(QTextCursor::End);
}
