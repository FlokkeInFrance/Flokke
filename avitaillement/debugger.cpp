#include "debugger.h"

const QString eol("\n");

debugger::debugger() :
    databuilder(nullptr)

{

}

void debugger::ShowList(QStringList mylist){
    for (auto &lst:mylist){
        qDebug()<<(lst);
    }
}

void debugger::ShowVariable(QVariant myVar){
    qDebug()<<myVar;
}

void debugger::SetDataManager(basemanager *inbase){
    databuilder=inbase;
}

void debugger::ShowTables(){
    ShowList(databuilder->AllTableList());
}

void debugger::PrintBaseStructure(){
    QStringList tablN,allD,fieldL;
    tablN=databuilder->AllTableList();
    for (auto &tn:tablN){
        allD<<tn+eol;
        fieldL=databuilder->ColumnNames(tn);
        for (auto &fn:fieldL) allD<<"    "+fn+eol;
        fieldL.clear();
    }
    QFile dL(datawizard::nomStructure);
    dL.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream textW(&dL);
    for (auto &ln:allD) textW<<ln;
    dL.close();
}

void debugger::DumpTableContent(QString intblName){
    QStringList content(databuilder->AllRecordsAsList(intblName));
    QFile dL(datawizard::nomDataDump);
    dL.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream textW(&dL);
    textW<<"all records of table "+intblName+eol;
    textW<<"";
    for (auto &ln:content) textW<<ln+eol;
    dL.close();
}
