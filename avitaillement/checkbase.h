/*
 * Checks it the database exists
 * If not, will be ordered to create it
 */
#ifndef CHECKBASE_H
#define CHECKBASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QtDebug>
#include <datawizard.h>
#include <QVector>
#include <QFile>



class CheckBase
{
public:
    CheckBase();
    QString BaseId(int inId);
    void MakeNewBase();
    bool isNewBase();
    void GetScheduleInfo();
    QString MakeCreateStatement(QString inTableName,QStringList inFieldList,QStringList inTypeList);
    QString MakeIndexStatement (QString indexName, QString inTableName, QStringList inFieldNames);

    QSqlDatabase db;
    QDate now,lastProjectsDate;
    bool newBase,hasASchedule;
    QStringList structureList;
};

#endif // CHECKBASE_H
