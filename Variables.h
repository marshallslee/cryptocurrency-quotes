#ifndef VARIABLES_H
#define VARIABLES_H

#include <QThread>
#include <QTimer>
#include <QWebSocket>
#include <QtNetwork>

struct TradingPair_st
{
    QString name;
    QString base_symbol;
    QString quote_symbol;
    QString orgName;
};



#endif // VARIABLES_H
