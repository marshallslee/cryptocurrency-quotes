#include "CDlgMain.h"
#include "ui_CDlgMain.h"

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

void CDlgMain::slotLog1(QString iStr)
{
    ui->teLog1->moveCursor(QTextCursor::End);
    ui->teLog1->insertPlainText(iStr + "\n");

    ui->teLog1->moveCursor(QTextCursor::End);
}
