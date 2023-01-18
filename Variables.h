#ifndef VARIABLES_H
#define VARIABLES_H

#include <QThread>
#include <QTimer>
#include <QWebSocket>
#include <QtNetwork>
#include <QListWidgetItem>

struct TradingPair_st
{
    QString name;
    QString base_symbol;
    QString quote_symbol;
    QString orgName;
    int base_precision;
    int quote_precision;
};

typedef std::unordered_map<QString, TradingPair_st> Pairs_um;

#endif // VARIABLES_H
