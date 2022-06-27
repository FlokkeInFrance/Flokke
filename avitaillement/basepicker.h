/*
 * This class makes select statements and puts results out as such (qsqlquery) or as models to display
 */
#ifndef BASEPICKER_H
#define BASEPICKER_H

#include <basemanager.h>
#include <QDebug>

class basepicker
{
public:
    basepicker();

   void SetBaseManager(basemanager *inbase);

   //Sql helpers
   QString BaseId(int inId);
   QString CommaList(const sqlvect &inList,bool withParethesis=true);
   QString SpacedCommaList(const sqlvect &inList,bool withParethesis=true);

private:
    basemanager *myBm;
    QSqlDatabase db;
    QSqlQuery quest;

};

#endif // BASEPICKER_H
