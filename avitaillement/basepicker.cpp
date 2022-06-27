
#include "basepicker.h"
#include "debugger.h"
basepicker::basepicker():
    myBm(nullptr),
    db(),
    quest(QSqlDatabase::database())

{

}

void basepicker::SetBaseManager(basemanager *inbase){
    myBm=inbase;
}

QString basepicker::BaseId(int inId){
    return datawizard::GetDbIdentifier(inId);
}

QString basepicker::CommaList(const sqlvect &inList,bool withParethesis){
    QString ret("");
    if (withParethesis) ret=" (";
    int sz=inList.size();
    for (int idx(0);idx<sz;idx++){
        ret=ret+inList[idx];
        if (idx<sz-1) ret=ret+",";
    }
    if (withParethesis) ret=ret+")";
    return ret;
}

QString basepicker::SpacedCommaList(const sqlvect &inList,bool withParethesis){
    QString ret("");
    if (withParethesis) ret=" (";
    int sz=inList.size();
    for (int idx(0);idx<sz;idx++){
        ret=ret+inList[idx];
        if (idx<sz-1) ret=ret+", ";
        else ret=ret+" ";
    }
    if (withParethesis) ret=ret+")";
    return ret;
}

QString basepicker::MakeWhereClause(const QString &wherefield, const QString &whereVal, const QString &comp){
    QString whereClause=QString("WHERE %1 %2 %3").arg(wherefield,comp,whereVal);
    return whereClause;
}

QString basepicker::MakeWhereClause(const sqlvect &wherefield, const sqlvect &whereVal, const QString &comp){
    QString whereList("WHERE ");
    int sz=wherefield.size();
    for (int idx(0);idx<sz;idx++){
        whereList=whereList+QString("%1 %2 %3").arg(wherefield[idx],comp,whereVal[idx]);
        if (idx<sz-1) whereList=whereList+(" AND ");
    }
    return whereList;
}

QString basepicker::MakeWhereString(const QString &leftPart, const QString &rightPart, const QString &comp){
    QString whereClause=QString("%1 %2 %3").arg(leftPart,comp,rightPart);
    return whereClause;
}

QString basepicker::MakeWhereString(const sqlvect &wherefield, const sqlvect &whereVal, const QString &comp){
    QString whereList("");
    int sz=wherefield.size();
    for (int idx(0);idx<sz;idx++){
        whereList=whereList+QString("%1 %2 %3").arg(wherefield[idx],comp,whereVal[idx]);
        if (idx<sz-1) whereList=whereList+(" AND ");
    }
    return whereList;
}

QString basepicker::MakeWhereString(const sqlvect &wherefield, const sqlvect &whereVal, const sqlvect &comp){
    QString whereList("WHERE ");
    int sz=wherefield.size();
    for (int idx(0);idx<sz;idx++){
        whereList=whereList+QString("%1 %2 %3").arg(wherefield[idx],comp[idx],whereVal[idx]);
        if (idx<sz-1) whereList=whereList+(" AND ");
    }
    return whereList;
}

