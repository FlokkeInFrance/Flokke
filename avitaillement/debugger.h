#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QVariant>
#include <QStringList>
#include <QDebug>
#include <basemanager.h>
#include <datawizard.h>
#include <QFile>
#include <QTextStream>

class debugger
{
public:
    debugger();

    static void ShowList(QStringList mylist);
    static void ShowVariable(QVariant myVar);
    void SetDataManager(basemanager *inbase);
    void ShowTables();
    void PrintBaseStructure();
    void DumpTableContent(QString intblName);

private:

    basemanager *databuilder;

};

#endif // DEBUGGER_H
