/*
 * creates/destroys/modifies records in tables of the database
 * No zero values for first entries : zero means unassigned
 * ensures for data integrity between tables
 * maintains a set of "active" data : active croisière, active menu, active date,
 *  active dish, active inventory, active ingredient, active tools, active shopping list, active locker,
   can feed raw data to other classes like the parameters in string/record lists
   can write a bunch of raw data received in record lists like the parameters
   uses datawizard to format the data (perhaps with static functions)

   As an additional feature, can check if a single cruise is under way, selects it and returns true.
   If no cruise is underway, sets a flag accordingly, if several are underway make a list and set a flag.
   If no cruise exists, set a flag

*/

#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlIndex>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QSqlError>
#include <QSqlField>
#include <QDate>
#include "datawizard.h"
#include "myparameters.h"
#include "linguist.h"

using intvect=QVector<int>;
using stringvect=QVector<QString>;

class debugger;
class CentreOp;

class basemanager
{

public:

    //Initializations
    basemanager();

    void KeepDataWizard(datawizard *indatawizard);
    void MeetParameterManager(myparameters* inparam);
    void SetLinguist (linguist* inlang);
    void SetHelpers (CentreOp* inCenter,linguist* inlang,myparameters* inparam, debugger* indebug, datawizard* indata);


    //General Base managers
    QStringList AllTableList();
    QStringList ColumnNames(const QString &intablename);
    QStringList AllRecordsAsList(const QString &intablename);
    void        MakeBackUpOfDb();

    //sql syntax functions
    QString BaseId(int inId);
    QString CommaList(const stringvect &inList,bool withParethesis=true);
    QString SpacedCommaList(const stringvect &inList,bool withParethesis=true);
    QString SpacedCommaFieldList(const intvect &inList,bool withParethesis=true);

    QString MakeWhereClause(int wherefield, const QString &whereVal, const QString &comp="=");
    QString MakeWhereClause(const intvect &wherefield, const stringvect &whereVal, const QString &comp="=");
    QString MakeWhereString(int field, const QString &rightPart, const QString &comp="=");
    QString MakeAndWhereString(const intvect &wherefield, const stringvect &whereVal, const stringvect &comp);
    QString MakeAndWhereString(const intvect &wherefield, const stringvect &whereVal);
    QString MakeSetString(const intvect &wherefield, const stringvect &whereVal);

    void    DoSelectAllFields(int inTable, const QString &whereString, intvect order=intvect());
    void    DoSimpleSelectAllFields(int inTable,int inleftPart, int rightPart, intvect order=intvect());
    void    DoSelectOneField(int inTable, int inField, const QString &whereString, intvect order=intvect());
    void    DoSelectOneField(int inTable, int inField, const QString &whereString, int order);
    void    DoSelectSomeFields(int inTable, const intvect &inField, const QString &whereString, int order=0);
    void    DoSelectSomeFields(int inTable, const intvect &inField, const QString &whereString, intvect order=intvect());

    //Indexators
    int  FindFreeSlot(int tableN,int targetField, int whereField=0, const QString &whereValue="");
    int  FindFreeSlot(int tableN, int targetField, intvect whereField, const stringvect &whereValue);
    int  FindLastIdx(int tableN, int targetField, int whereField=0, const QString &whereValue="");
    int  FindLastIdx(int tableN,int targetField, intvect whereField, const stringvect &whereValue);

    //General statements
    void ExecuteSelect(QString statement);
    void OrderAndCommit (const QString &sqlOrder);

    void Insert( int tableName,intvect fieldNames,const stringvect &newValues);
    void Modify(int tableName,int fieldName, const QString &nVal, const QString &wString);
    void Modify( int tableName,  int fieldName,  const QString &nVal,const intvect &idFields,const stringvect &idVal);
    void Modify( int tableName,  int fieldName,  const QString &nVal, int idField, const QString &idVal);
    void Modify( int tableName,  const intvect fieldNames,  const stringvect &nVals,const intvect &idFields,const stringvect &idVal);
    void Modify( int tableName,  const intvect fieldNames,  const stringvect &nVals, int idField, const QString &idVal);
    void Delete(int tableName, const intvect &idFields, const stringvect &idVals);
    void Delete( int tableName, int idField, const QString &idVal);
    void ShiftIndex (int tableName, int idField, int pos);
    void ShiftIndex (int tableName, int idField, int pos, int wherefield, const QString &whereVal);
    void ShiftIndex (int tableName, int idField, int pos,intvect whereField, const stringvect &whereVal);

    //Table Managers
    void Insert(const sCroisiere &inR);
    void Modify(const sCroisiere &inR);
    void Initialise (sCroisiere &inR);
    bool ReadRecord (sCroisiere &inR);

    void Insert(const sMenutype &inR);
    void Modify(const sMenutype &inR);
    void Initialise (sMenutype &inR);
    bool ReadRecord (sMenutype &inR);

    void Insert(const sPlattype &inR);
    void Modify(const sPlattype &inR);
    void Initialise(sPlattype &inR);
    bool ReadRecord (sPlattype &inR);

    void Insert(const sLieux &inR);
    void Modify(const sLieux &inR);
    void Initialise(sLieux &inR);
    bool ReadRecord(sLieux &inR);

    void Insert(const sPlanning &inR);
     void Modify(const sPlanning &inR);
     void Initialise(sPlanning &inR);
     bool ReadRecord (sPlanning &inR);

     void Insert(const sMenu &inR);
     void Modify(const sMenu &inR);
     void Initialise(sMenu &inR);
     bool ReadRecord (sMenu &inR);

     void Insert(const sPlat &inR);
     void Modify(const sPlat &inR);
     void Initialise(sPlat &inR);
     bool ReadRecord (sPlat &inR);

     void Insert(const sListePrepeses &inR);
     void Modify(const sListePrepeses &inR);
     void Initialise(sListePrepeses &inR);
     bool ReadRecord (sListePrepeses &inR);

     void Insert(const sComposition &inR);
     void Modify(const sComposition &inR);
     void Initialise(sComposition &inR);
     bool ReadRecord (sComposition &inR);

     void Insert(const sIComposition &inR);
     void Modify(const sIComposition &inR);
     void Initialise(sIComposition &inR);
     bool ReadRecord (sIComposition &inR);

     void Insert(const sListeIngredients &inR);
     void Modify(const sListeIngredients &inR);
     void Initialise(sListeIngredients &inR);
     bool ReadRecord (sListeIngredients &inR);

     void Insert(const sListeUstensiles &inR);
     void Initialise(sListeUstensiles &inR);
     bool ReadRecord (sListeUstensiles &inR);

     void Insert(const sIngredients &inR);
     void Modify(const sIngredients &inR);
     void Initialise(sIngredients &inR);
     bool ReadRecord (sIngredients &inR);

     void Insert(const sRayon &inR);
     void Modify(const sRayon &inR);
     void Initialise(sRayon &inR);
     bool ReadRecord (sRayon &inR);

     void Insert(const sQuantites &inR);
     void Modify(const sQuantites &inR);
     void Initialise(sQuantites &inR);
     bool ReadRecord (sQuantites &inR);

     void Insert(const sUstensiles &inR);
     void Modify(const sUstensiles &inR);
     void Initialise(sUstensiles &inR);
     bool ReadRecord (sUstensiles &inR);

     void Insert(const sInventaire &inR);
     void Modify(const sInventaire &inR);
     void Initialise(sInventaire &inR);
     bool ReadRecord (sInventaire &inR);

     void Insert(const sListeCourse &inR);
     void Modify(const sListeCourse &inR);
     void Initialise(sListeCourse &inR);
     bool ReadRecord (sListeCourse &inR);

     void Insert(const sItemsListeCourse &inR);
     void Modify(const sItemsListeCourse &inR);
     void Initialise(sItemsListeCourse &inR);
     bool ReadRecord (sItemsListeCourse &inR);

     void Insert(const sConversions &inR);
     void Modify(const sConversions &inR);
     void Initialise(sConversions &inR);
     bool ReadRecord (sConversions &inR);

     void Insert(const sEquipets &inR);
     void Modify(const sEquipets &inR);
     void Initialise(sEquipets &inR);
     bool ReadRecord (sEquipets &inR);

     void Insert(const sBateau &inR);
     void Modify(const sBateau &inR);
     void Initialise(sBateau &inR);
     bool ReadRecord(sBateau &inR);

     void Insert(const sPhotos &inR);
     void Modify(const sPhotos &inR);
     void Initialise(sPhotos &inR);
     bool ReadRecord (sPhotos &inR);

     void Insert(const sLocUstensiles &inR);
     void Modify(const sLocUstensiles &inR);
     void Initialise(sLocUstensiles &inR);
     bool ReadRecord (sLocUstensiles &inR);

     void Insert(const sPavillons &inR);
     void Modify(const sPavillons &inR);
     void Initialise(sPavillons &inR);
     bool ReadRecord (sPavillons &inR);

     void selectCruise(sCroisiere inCruise);

static sCroisiere aCruise;

protected:
    CentreOp *myCenter;
    debugger *debug;
    datawizard *dw;
    linguist *langue;
    QSqlDatabase db;
    myparameters* paramMan;
    QSqlQuery quest;

};

#endif // BASEMANAGER_H
