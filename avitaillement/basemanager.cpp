#include "basemanager.h"

basemanager::basemanager() :

    dw(nullptr),
    langue(nullptr),
    db(QSqlDatabase::database()),
    paramMan(nullptr),
    quest(QSqlDatabase::database())
{

}

void basemanager::MeetParameterManager(myparameters* inparam){
    paramMan=inparam;
}

void basemanager::KeepDataWizard(datawizard *indatawizard){
    dw=indatawizard;
    QVector<sConversions> convs;
    DoSelectAllFields(tConversions,"");
    if (quest.next()){
        int nidP=quest.record().indexOf(BaseId(idParametre));
        int nabbr=quest.record().indexOf(BaseId(abbreviation));
        int ncompl=quest.record().indexOf(BaseId(nomComplet));
        int nml=quest.record().indexOf(BaseId(enMillilitres));
        int nmg=quest.record().indexOf(BaseId(enMilligrammes));
        do{
            sConversions ncv;
            ncv.idParametre=quest.value(nidP).toInt();
            ncv.abbreviation=quest.value(nabbr).toString();
            ncv.nomComplet=quest.value(ncompl).toString();
            ncv.enMilligrammes=quest.value(nmg).toInt();
            ncv.enMillilitres=quest.value(nml).toInt();
            convs<<ncv;
        }while (quest.next());
    }
    dw->GetConversions(convs);
}

QString basemanager::BaseId(int inId){
    return datawizard::DbId(inId);
}

void basemanager::SetLinguist(linguist *inlang){
    langue=inlang;
}

QString basemanager::CommaList(const stringvect &inList, bool withParethesis){
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

QString basemanager::SpacedCommaList(const stringvect &inList, bool withParethesis){
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

QString basemanager::SpacedCommaFieldList(const intvect &inList,bool withParethesis){
    QString ret("");
    if (withParethesis) ret=" (";
    int sz=inList.size();
    for (int idx(0);idx<sz;idx++){
        ret=ret+BaseId(inList[idx]);
        if (idx<sz-1) ret=ret+", ";
        else ret=ret+" ";
    }
    if (withParethesis) ret=ret+")";
    return ret;
}

QString basemanager::MakeWhereClause(int wherefield, const QString &whereVal,const QString &comp){
    QString whereClause=QString("WHERE %1 %2 %3").arg(BaseId(wherefield),comp,whereVal);
    return whereClause;
}

QString basemanager::MakeWhereClause(intvect const &wherefield, const stringvect &whereVal, const QString &comp){
    QString whereList("WHERE ");
    int sz=wherefield.size();
    for (int idx(0);idx<sz;idx++){
        whereList=whereList+QString("%1 %2 %3").arg(BaseId(wherefield[idx]),comp,whereVal[idx]);
        if (idx<sz-1) whereList=whereList+(" AND ");
    }
    return whereList;
}

QString basemanager::MakeWhereString(int field, const QString &rightPart, const QString &comp){
    if (!field) return ("");
    QString ret=QString("%1 %2 %3").arg(BaseId(field),rightPart,comp);
    return ret;
}

QString basemanager::MakeAndWhereString(const intvect &wherefield, const stringvect &whereVal, const stringvect &comp){
    QString whereList("");
    int sz=wherefield.size();
    for (int idx(0);idx<sz;idx++){
        whereList=whereList+QString("%1 %2 %3").arg(BaseId(wherefield[idx]),comp[idx], whereVal[idx]);
        if (idx<sz-1) whereList=whereList+(" AND ");
    }
    return whereList;
}

QString basemanager::MakeAndWhereString(const intvect &wherefield, const stringvect &whereVal){
    QString whereList("");
    int sz=wherefield.size();
    for (int idx(0);idx<sz;idx++){
        whereList=whereList+QString("%1 = %2").arg(BaseId(wherefield[idx]), whereVal[idx]);
        if (idx<sz-1) whereList=whereList+(" AND ");
    }
    return whereList;
}

int  basemanager::FindFreeSlot(int tableN,int targetField, int whereField, const QString &whereValue){
    int free(0);
    bool notFound(true);
    intvect targetF;
    targetF<<targetField;
    DoSelectOneField(tableN,targetField,MakeWhereString(whereField,whereValue),targetF);

    while (quest.next()&&notFound){
        free++;
        if (quest.value(0).toInt()!=free) notFound=false;
    }
    if (notFound) free++;
    return free;
}

int  basemanager::FindFreeSlot(int tableN, int targetField, intvect whereField, const stringvect &whereValue){
    int free(0);
    intvect targetF;
    targetF<<targetField;
    DoSelectOneField(tableN,targetField,MakeAndWhereString(whereField,whereValue),targetF);
    bool found(false);
    while (quest.next()&&!found){
        free++;
        if (quest.value(0).toInt()!=free) found=true;
    }
    return free;
}

int basemanager::FindLastIdx(int tableN,int targetField, int whereField, const QString &whereValue){
    int free(0);
    intvect targetF;
    targetF<<targetField;
    DoSelectOneField(tableN,targetField,MakeWhereString(whereField,whereValue),targetF);
    while (quest.next()){
        free=quest.value(0).toInt();
    }
    free++;
    return free;
}

int basemanager::FindLastIdx(int tableN,int targetField, intvect whereField, const stringvect &whereValue){
    int free(0);
    intvect targetF;
    targetF<<targetField;
    DoSelectOneField(tableN,targetField,MakeAndWhereString(whereField,whereValue),targetF);
    while (quest.next())
        free=quest.value(0).toInt();
    free++;
    return free;
}

void basemanager::OrderAndCommit( const QString &sqlOrder){
    QSqlQuery ord(db);
    ord.exec(sqlOrder);
    qDebug()<<ord.lastQuery();
    qDebug()<<"error if any : "<<ord.lastError().databaseText();
    db.commit();
}

void basemanager::Insert(  int tableName,intvect fieldNames,const stringvect &newValues){;
    QString toInsert=QString("INSERT INTO %1 (%2) VALUES (%3)").arg(BaseId(tableName),SpacedCommaFieldList(fieldNames,false),SpacedCommaList(newValues,false));
    OrderAndCommit(toInsert);
}

void basemanager::Modify(int tableName,int fieldName, const QString &nVal, const QString &wString){
    QString updateQ=QString("UPDATE %1 SET %2=%3 WHERE %4").arg(BaseId(tableName),BaseId(fieldName),nVal,wString);
    OrderAndCommit(updateQ);
}

void basemanager::Modify(int tableName,  int fieldName,  const QString &nVal, const intvect &idFields, const stringvect &idVal){
    QString whereList(MakeAndWhereString(idFields,idVal));
    QString updateQ=QString("UPDATE %1 SET %2=%3 WHERE %4").arg(BaseId(tableName),BaseId(fieldName),nVal,whereList);
    OrderAndCommit(updateQ);
}

void basemanager::Modify(int tableName,  int fieldName,  const QString &nVal, int idField, const QString &idVal){
 QString updateQ=QString("UPDATE %1 SET %2=%3 WHERE %4=%5").arg(BaseId(tableName),BaseId(fieldName),nVal,BaseId(idField),idVal);
 OrderAndCommit(updateQ);
}

void basemanager::Modify( int tableName,  const intvect fieldNames,  const stringvect &nVals,const intvect &idFields,const stringvect &idVal){
    QString setString=MakeAndWhereString(fieldNames,nVals);
    QString whereString=MakeAndWhereString(idFields,idVal);
    QString updateQ=QString("UPDATE %1 SET %2 WHERE %3").arg(BaseId(tableName),setString,whereString);
    OrderAndCommit(updateQ);
}

void basemanager::Modify( int tableName,  const intvect fieldNames,  const stringvect &nVals, int idField, const QString &idVal){
    QString fieldN(MakeAndWhereString(fieldNames,nVals));

    QString updateQ=QString("UPDATE %1 SET %2 WHERE %4=%5").arg(BaseId(tableName),fieldN,BaseId(idField),idVal);
    OrderAndCommit(updateQ);
}

void basemanager::Delete(int tableName, const intvect &idFields, const stringvect &idVals){
    QString whereList("");
    whereList=MakeAndWhereString(idFields,idVals);

    QString deleteQ=QString("DELETE FROM %1 WHERE %2").arg(BaseId(tableName),whereList);
    OrderAndCommit(deleteQ);
}

void basemanager::Delete(int tableName, int idField, const QString &idVal){
    QString deleteQ=QString("DELETE FROM %1 WHERE %2 = %3").arg(BaseId(tableName),BaseId(idField),idVal);
    OrderAndCommit(deleteQ);
}

void basemanager::ShiftIndex(int tableName, int idField, int pos){
 QString orderU=QString("UPDATE %1 SET %2=%2+1 WHERE %2>=%3").arg(BaseId(tableName),BaseId(idField),datawizard::ToSqlIntegerString(pos));
 OrderAndCommit(orderU);
}

void basemanager::ShiftIndex (int tableName, int idField, int pos,int wherefield, const QString &whereVal){
    QString whereS("");
    whereS=MakeWhereString(wherefield,whereVal);
    QString orderU=QString("UPDATE %1 SET %2=§2+1 WHERE %2>=%3 AND %4").arg(BaseId(tableName),BaseId(idField),datawizard::ToSqlIntegerString(pos),whereS);
    OrderAndCommit(orderU);
}

void basemanager::ShiftIndex (int tableName, int idField, int pos,intvect whereField, const stringvect &whereVal){
    QString whereS("");
    whereS=MakeAndWhereString(whereField,whereVal);
    QString orderU=QString("UPDATE %1 SET %2=§2+1 WHERE %2>=%3 AND %4").arg(BaseId(tableName),BaseId(idField),datawizard::ToSqlIntegerString(pos),whereS);
    OrderAndCommit(orderU);
}

void basemanager::DoSelectAllFields(int inTable, const QString &whereString, intvect order){
    if (whereString==""){
        QString selS=QString("SELECT * FROM %1").arg(BaseId(inTable));
        if (order.size()) selS=selS+QString(" ORDER BY %1").arg(SpacedCommaFieldList(order));
        ExecuteSelect(selS);
        return;
    }
    QString selS=QString("SELECT * FROM %1 WHERE %2").arg(BaseId(inTable),whereString);
    if (order.size()) selS=selS+QString(" ORDER BY %1").arg(SpacedCommaFieldList(order));
    ExecuteSelect(selS);
}

void basemanager::DoSimpleSelectAllFields(int inTable,int inleftPart, int rightPart, intvect order){
    QString rightP=datawizard::ToSqlIntegerString(rightPart);
    QString ws=MakeWhereString(inleftPart,rightP);
    DoSelectAllFields(inTable,ws,order);
}

void basemanager::DoSelectOneField(int inTable, int inField, const QString &whereString, intvect order){
    QString selS("");
    if (whereString.isEmpty())
        selS=QString("SELECT %1 FROM %2").arg(BaseId(inTable),BaseId(inField));
    else
        selS=QString("SELECT %1 FROM %2 WHERE %3").arg(BaseId(inTable),BaseId(inField),whereString);
    if (order.size()) selS=selS+QString(" ORDER BY %1").arg(SpacedCommaFieldList(order));
    ExecuteSelect(selS);
}

void basemanager::DoSelectOneField(int inTable, int inField, const QString &whereString, int order){
    QString selS("");
    if (whereString.isEmpty())
        selS=QString("SELECT %1 FROM %2").arg(BaseId(inTable),BaseId(inField));
    else
        selS=QString("SELECT %1 FROM %2 WHERE %3").arg(BaseId(inTable),BaseId(inField),whereString);
    if (order) selS=selS+QString(" ORDER BY %1").arg(BaseId(order));
    ExecuteSelect(selS);
}

void basemanager::DoSelectSomeFields(int inTable, const intvect &inField, const QString &whereString, int order){
    QString selS=QString("SELECT %1 FROM %2 WHERE %3").arg(BaseId(inTable),SpacedCommaFieldList(inField),whereString);
    if (order) selS=selS+QString(" ORDER BY %1").arg(BaseId(order));
    ExecuteSelect(selS);
}

void basemanager::DoSelectSomeFields(int inTable, const intvect &inField, const QString &whereString, intvect order){
    QString selS=QString("SELECT %1 FROM %2 WHERE %3").arg(BaseId(inTable),SpacedCommaFieldList(inField),whereString);
    if (order.size()) selS=selS+QString(" ORDER BY %1").arg(SpacedCommaFieldList(order));
    ExecuteSelect(selS);
}

void basemanager::ExecuteSelect(QString statement){
    quest.clear();
    quest.exec(statement);
    qDebug()<<quest.lastQuery();
    qDebug()<<"error if any : "<<quest.lastError().databaseText();
}

QStringList basemanager::AllTableList(){
    QStringList allTables;
    QSqlQuery quest(db);
    quest.exec("SELECT * FROM sqlite_master");
    while (quest.next()){
        if (quest.value("type").toString()=="table")
            allTables<<(quest.value("name").toString());
    }
    return allTables;
}

QStringList basemanager::ColumnNames(const QString &intablename){
    QStringList fieldList;
    QSqlQuery quest(db);
    quest.exec("PRAGMA table_info("+intablename+")");
    while (quest.next()){
        fieldList<<quest.value(0).toString()+"   "+quest.value(1).toString();
    }
    return fieldList;
}

QStringList basemanager::AllRecordsAsList(const QString &intablename){
    QStringList allR;
    QSqlQuery quest(db);
    quest.exec("select * FROM "+intablename);
    int cols(quest.record().count());
    while (quest.next()){
        QString ln;
        for (int cl(0);cl<cols;cl++) ln=ln+quest.value(cl).toString()+"  ";
        allR<<ln;
    }
    return allR;
}
void basemanager::Insert(const sCroisiere &inR){
    intvect fields;
    stringvect vals;

    fields<<idCroisiere;vals<<dw->ToSqlIntegerString(inR.idCroisiere);
    fields<<nomBateau;vals<<dw->ToSqlString(inR.nomBateau);
    fields<<typeBateau;vals<<dw->ToSqlString(inR.typeBateau);
    fields<<intitule;vals<<dw->ToSqlString(inR.intitule);
    fields<<dateDepart;vals<<dw->ToSqlJulian(inR.dateDepart);
    fields<<dateFin;vals<<dw->ToSqlJulian(inR.dateFin);
    fields<<nombreJours;vals<<dw->ToSqlIntegerString(inR.nombreJours);
    fields<<nombrePersonnes;vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);
    fields<<terminee;vals<<dw->ToSqlIntegerString(inR.terminee);
    Insert(tCroisiere,fields,vals); 
}
void basemanager::Modify(const sCroisiere &inR){
    intvect fields;
    stringvect vals;

    fields<<nomBateau;vals<<dw->ToSqlString(inR.nomBateau);
    fields<<typeBateau;vals<<dw->ToSqlString(inR.typeBateau);
    fields<<intitule;vals<<dw->ToSqlString(inR.intitule);
    fields<<dateDepart;vals<<dw->ToSqlJulian(inR.dateDepart);
    fields<<dateFin;vals<<dw->ToSqlJulian(inR.dateFin);
    fields<<nombreJours;vals<<dw->ToSqlIntegerString(inR.nombreJours);
    fields<<nombrePersonnes;vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);
    fields<<terminee;vals<<dw->ToSqlIntegerString(inR.terminee);
    Modify(tCroisiere,fields,vals,idCroisiere,dw->ToSqlIntegerString(inR.idCroisiere));  
}

void basemanager::Initialise (sCroisiere &inR){
    inR.idCroisiere=0;
    inR.nomBateau="";
    inR.typeBateau="";
    inR.intitule="";
    inR.dateDepart=QDate();
    inR.dateFin=QDate();
    inR.nombreJours=0;
    inR.nombrePersonnes=0;
    inR.terminee=0;
}

bool basemanager::ReadRecord (sCroisiere &inR){
    bool found(false);
    DoSelectAllFields(tCroisiere,MakeWhereString(idCroisiere,dw->ToSqlIntegerString(inR.idCroisiere)));
    if (quest.next()){
        found=true;
        inR.nomBateau=                    quest.value(1).toString();
        inR.typeBateau=                   quest.value(2).toString();
        inR.intitule=                     quest.value(3).toString();
        inR.dateDepart=dw-> FromSqlJulian(quest.value(4).toInt());
        inR.dateFin=dw->    FromSqlJulian(quest.value(5).toInt());
        inR.nombreJours=                  quest.value(6).toInt();
        inR.nombrePersonnes=              quest.value(7).toInt();
        inR.terminee=                     quest.value(8).toBool();
    }
    return found;
}

void basemanager::Insert(const sMenutype &inR){
    intvect fields;
    stringvect vals;

    fields<<idCroisiere;
    fields<<idMenuType;
    fields<<nomMenuType;
    fields<<nombrePersonnes;

    vals<<dw->ToSqlIntegerString(inR.idCroisiere);
    vals<<dw->ToSqlIntegerString(inR.idMenuType);
    vals<<dw->ToSqlString(inR.nomMenuType);
    vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

    Insert(tMenutype,fields,vals);  
}

void basemanager::Modify(const sMenutype &inR){
    intvect fields,wherefields;
    stringvect vals,wherevals;

    wherefields<<idCroisiere;
    wherefields<<idMenuType;
    fields<<nomMenuType;
    fields<<nombrePersonnes;

    wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
    wherevals<<dw->ToSqlIntegerString(inR.idMenuType);
    vals<<dw->ToSqlString(inR.nomMenuType);
    vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

    Modify(tMenutype,fields,vals,wherefields,wherevals);
}

void basemanager::Initialise (sMenutype &inR){
    inR.idCroisiere=0;
    inR.idMenuType=0;
    inR.nomMenuType="";
    inR.nombrePersonnes=0;
}

bool basemanager::ReadRecord (sMenutype &inR){
    bool found(false);
    intvect fields;
    stringvect vals;
    fields<<idCroisiere;
    fields<<idMenuType;
    vals<<dw->ToSqlIntegerString(inR.idCroisiere);
    vals<<dw->ToSqlIntegerString(inR.idMenuType);
    DoSelectAllFields(tMenutype,MakeAndWhereString(fields,vals));
    if (quest.next()){
        found=true;
        inR.nomMenuType=quest.value(2).toString();
        inR.nombrePersonnes=quest.value(3).toInt();
    }
    return found;
}

void basemanager::Insert(const sPlattype &inR){
    intvect fields;
    stringvect vals;

    fields<<idCroisiere;
    fields<<idMenuType;
    fields<<idPlatType;
    fields<<nomPlatType;
    fields<<nombrePersonnes;

    vals<<dw->ToSqlIntegerString(inR.idCroisiere);
    vals<<dw->ToSqlIntegerString(inR.idMenuType);
    vals<<dw->ToSqlIntegerString(inR.idPlatType);
    vals<<dw->ToSqlString(inR.nomPlatType);
    vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

    Insert(tPlattype,fields,vals);  
}

void basemanager::Modify(const sPlattype &inR){
    intvect fields,wherefields;
    stringvect vals,wherevals;

    wherefields<<idCroisiere;
    wherefields<<idMenuType;
    wherefields<<idPlatType;
    fields<<nomPlatType;
    fields<<nombrePersonnes;

    wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
    wherevals<<dw->ToSqlIntegerString(inR.idMenuType);
    wherevals<<dw->ToSqlIntegerString(inR.idPlatType);
    vals<<dw->ToSqlString(inR.nomPlatType);
    vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

    Modify(tPlattype,fields,vals,wherefields,wherevals); 
}

void basemanager::Initialise(sPlattype &inR){
    inR.idCroisiere=0;
    inR.idMenuType=0;
    inR.idPlatType=0;
    inR.nomPlatType="";
    inR.nombrePersonnes=0;
}
bool basemanager::ReadRecord (sPlattype &inR){
    bool found(false);
    if(inR.idCroisiere&&inR.idMenuType&&inR.idPlatType){
        intvect fields;
        stringvect vals;
        fields<<idCroisiere;
        fields<<idMenuType;
        fields<<idPlatType;
        vals<<dw->ToSqlIntegerString(inR.idCroisiere);
        vals<<dw->ToSqlIntegerString(inR.idMenuType);
        vals<<dw->ToSqlIntegerString(inR.idPlatType);

        DoSelectAllFields(tPlattype,MakeAndWhereString(fields,vals));
        if (quest.next()){
            found=true;
            inR.nomPlatType=quest.value(3).toString();
            inR.nombrePersonnes=quest.value(5).toInt();
        }
    }
    return found;
}

void basemanager::Insert(const sLieux &inR){
    intvect fields;
    stringvect vals;

    fields<<idCroisiere;
    fields<<date;
    fields<<lieu;
    fields<<amarre;
    fields<<contact;
    fields<<depannage;
    fields<<commerce;

    vals<<dw->ToSqlIntegerString(inR.idCroisiere);
    vals<<dw->ToSqlJulian(inR.date);
    vals<<dw->ToSqlString(inR.lieu);
    vals<<dw->ToSqlString(inR.amarre);
    vals<<dw->ToSqlString(inR.contact);
    vals<<dw->ToSqlIntegerString(inR.depannage);
    vals<<dw->ToSqlIntegerString(inR.commerce);

    Insert(tLieux,fields,vals);    
}

void basemanager::Modify(const sLieux &inR){
    intvect fields,wherefields;
    stringvect vals,wherevals;

    wherefields<<idCroisiere;
    wherefields<<date;
    fields<<lieu;
    fields<<amarre;
    fields<<contact;
    fields<<contact;
    fields<<depannage;
    fields<<commerce;

    wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
    wherevals<<dw->ToSqlJulian(inR.date);
    vals<<dw->ToSqlString(inR.lieu);
    vals<<dw->ToSqlString(inR.amarre);
    vals<<dw->ToSqlString(inR.contact);
    vals<<dw->ToSqlIntegerString(inR.depannage);
    vals<<dw->ToSqlIntegerString(inR.commerce);

    Modify(tLieux,fields,vals,wherefields,wherevals); 
}

void basemanager::Initialise(sLieux &inR){

    inR.idCroisiere=0;
    inR.date=QDate();
    inR.lieu="";
    inR.amarre="";
    inR.contact="";
    inR.depannage=false;
    inR.commerce=false;

}
bool basemanager::ReadRecord(sLieux &inR){
    bool found(false);
    if (inR.idCroisiere && inR.date.isValid()){
        intvect fields;
        stringvect vals;

        fields<<idCroisiere;
        fields<<date;
        vals<<dw->ToSqlIntegerString(inR.idCroisiere);
        vals<<dw->ToSqlJulian(inR.date);

        DoSelectAllFields(tLieux,MakeAndWhereString(fields,vals));
        if (quest.next()){
            found=true;
            inR.idCroisiere=quest.value(0).toInt();
            inR.date=dw->FromSqlJulian(quest.value(1).toInt());
            inR.lieu=quest.value(2).toString();
            inR.amarre=quest.value(3).toString();
            inR.depannage=quest.value(4).toBool();
            inR.commerce=quest.value(5).toBool();
        }
    }
    return found;
}

void basemanager::Insert(const sPlanning &inR){
    intvect fields;
    stringvect vals;

    fields<<idCroisiere;
    fields<<idPlanning;
    fields<<dateRepas;
    fields<<idMenuType;
    fields<<idMenu;
    fields<<numRepas;
    fields<<momentRepas;
    fields<<nombrePersonnes;

    vals<<dw->ToSqlIntegerString(inR.idCroisiere);
    vals<<dw->ToSqlIntegerString(inR.idPlanning);
    vals<<dw->ToSqlJulian(inR.dateRepas);
    vals<<dw->ToSqlIntegerString(inR.idMenuType);
    vals<<dw->ToSqlIntegerString(inR.idMenu);
    vals<<dw->ToSqlIntegerString(inR.numRepas);
    vals<<dw->ToSqlString(inR.momentRepas);
    vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

    Insert(tPlanning,fields,vals);
}

 void basemanager::Modify(const sPlanning &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idPlanning;
     fields<<dateRepas;
     fields<<idMenuType;
     fields<<idMenu;
     fields<<numRepas;
     fields<<momentRepas;
     fields<<nombrePersonnes;

     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idPlanning);
     vals<<dw->ToSqlJulian(inR.dateRepas);
     vals<<dw->ToSqlIntegerString(inR.idMenuType);
     vals<<dw->ToSqlIntegerString(inR.idMenu);
     vals<<dw->ToSqlIntegerString(inR.numRepas);
     vals<<dw->ToSqlString(inR.momentRepas);
     vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

     Modify(tPlanning,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sPlanning &inR){
     inR.idCroisiere=0;
     inR.idPlanning=0;
     inR.dateRepas=QDate();
     inR.idMenuType=0;
     inR.idMenu=0;
     inR.numRepas=0;
     inR.momentRepas="";
     inR.nombrePersonnes=0;
 }
 bool basemanager::ReadRecord (sPlanning &inR){
     bool found(false);
     if (inR.idCroisiere&&inR.idPlanning){
         intvect fields;
         stringvect vals;

         fields<<idCroisiere;
         fields<<idPlanning;
         vals<<dw->ToSqlIntegerString(inR.idCroisiere);
         vals<<dw->ToSqlIntegerString(inR.idPlanning);

         DoSelectAllFields(tPlanning,MakeAndWhereString(fields,vals));
         if (quest.next()){
             found=true;
             inR.dateRepas=dw->FromSqlJulian(quest.value(2).toInt());
             inR.idMenuType=quest.value(3).toInt();
             inR.idMenu=quest.value(4).toInt();
             inR.numRepas=quest.value(5).toInt();
             inR.momentRepas=quest.value(6).toString();
             inR.nombrePersonnes=quest.value(7).toInt();
         }
     }
     return found;
 }

 void basemanager::Insert(const sMenu &inR){
     intvect fields;
     stringvect vals;

     fields<<idMenu;
     fields<<nomMenu;
     fields<<favori;

     vals<<dw->ToSqlIntegerString(inR.idMenu);
     vals<<dw->ToSqlString(inR.nomMenu);
     vals<<dw->ToSqlIntegerString(inR.favori);

     Insert(tMenu,fields,vals);
 }

 void basemanager::Modify(const sMenu &inR){
     intvect fields;
     stringvect vals;

     fields<<nomMenu;
     fields<<favori;

     vals<<dw->ToSqlString(inR.nomMenu);
     vals<<dw->ToSqlIntegerString(inR.favori);

     Modify(tMenu,fields,vals,idMenu,dw->ToSqlIntegerString(inR.idMenu));
 }

 void basemanager::Initialise(sMenu &inR){
    inR.idMenu=0;
    inR.nomMenu="";
    inR.favori=false;
 }

 bool basemanager::ReadRecord (sMenu &inR){
     bool found(false);
     if (inR.idMenu){
         DoSelectAllFields(tMenu,MakeWhereString(idMenu,dw->ToSqlIntegerString(inR.idMenu)));
         if (quest.next()){
             found=true;
             inR.nomMenu=quest.value(1).toString();
             inR.favori=quest.value(2).toBool();
         }
     }
     return found;
 }

 void basemanager::Insert(const sPlat &inR){
     intvect fields;
     stringvect vals;

     fields<<idPlat;
     fields<<titrePlat;
     fields<<categoriePlat;
     fields<<recette;
     fields<<auFour;
     fields<<auFrigo;
     fields<<tempsPreparation;
     fields<<tempsCuisson;
     fields<<nbPersonnes;
     fields<<flagsNav;
     fields<<flagsDiet;
     fields<<idPlatInclus;
     fields<<recetteBase;
     fields<<fractionnable;

     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlString(inR.titrePlat);
     vals<<dw->ToSqlString(inR.categoriePlat);
     vals<<dw->ToSqlString(inR.recette);
     vals<<dw->ToSqlBool(inR.auFour);
     vals<<dw->ToSqlBool(inR.auFrigo);
     vals<<dw->ToSqlNumeric(inR.tempsPreparation);
     vals<<dw->ToSqlNumeric(inR.tempsCuisson);
     vals<<dw->ToSqlIntegerString(inR.nbPersonnes);
     vals<<dw->ToSqlIntegerString(inR.flagsNav);
     vals<<dw->ToSqlIntegerString(inR.flagsDiet);
     vals<<dw->ToSqlIntegerString(inR.idPlatInclus);
     vals<<dw->ToSqlBool(inR.recetteBase);
     vals<<dw->ToSqlBool(inR.fractionnable);

     Insert(tPlat,fields,vals);
}

 void basemanager::Modify(const sPlat &inR){
     intvect fields;
     stringvect vals;

     fields<<titrePlat;
     fields<<categoriePlat;
     fields<<recette;
     fields<<auFour;
     fields<<auFrigo;
     fields<<tempsPreparation;
     fields<<tempsCuisson;
     fields<<nbPersonnes;
     fields<<flagsNav;
     fields<<flagsDiet;
     fields<<recetteBase;
     fields<<fractionnable;

     vals<<dw->ToSqlString(inR.titrePlat);
     vals<<dw->ToSqlString(inR.categoriePlat);
     vals<<dw->ToSqlString(inR.recette);
     vals<<dw->ToSqlBool(inR.auFour);
     vals<<dw->ToSqlBool(inR.auFrigo);
     vals<<dw->ToSqlNumeric(inR.tempsPreparation);
     vals<<dw->ToSqlNumeric(inR.tempsCuisson);
     vals<<dw->ToSqlIntegerString(inR.nbPersonnes);
     vals<<dw->ToSqlIntegerString(inR.flagsNav);
     vals<<dw->ToSqlIntegerString(inR.flagsDiet);
     vals<<dw->ToSqlBool(inR.recetteBase);
     vals<<dw->ToSqlBool(inR.recetteBase);

     Modify(tPlat,fields,vals,idPlat,dw->ToSqlIntegerString(inR.idPlat));
 }

 void basemanager::Initialise(sPlat &inR){
     inR.idPlat=0;
     inR.titrePlat="";
     inR.categoriePlat="";
     inR.recette="";
     inR.auFour=false;
     inR.auFrigo=false;
     inR.tempsPreparation=0;
     inR.tempsCuisson=0;
     inR.nbPersonnes=0;
     inR.flagsNav=0;
     inR.flagsDiet=0;
     inR.idPlatInclus=0;
     inR.recetteBase=false;
     inR.fractionnable=true;
 }
 bool basemanager::ReadRecord (sPlat &inR){
     bool found(false);
     if (inR.idPlat){
         DoSelectAllFields(tPlat,MakeWhereString(idPlat,dw->ToSqlIntegerString(inR.idPlat)));
         if (quest.next()){
             found=true;
             inR.titrePlat=quest.value(1).toString();
         inR.categoriePlat=quest.value(2).toString();
               inR.recette=quest.value(3).toString();
                inR.auFour=quest.value(4).toBool();
               inR.auFrigo=quest.value(5).toBool();
      inR.tempsPreparation=quest.value(6).toInt();
          inR.tempsCuisson=quest.value(7).toInt();
           inR.nbPersonnes=quest.value(8).toInt();
              inR.flagsNav=quest.value(9).toInt();
             inR.flagsDiet=quest.value(10).toInt();
          inR.idPlatInclus=quest.value(11).toInt();
           inR.recetteBase=quest.value(12).toBool();
           inR.fractionnable=quest.value(13).toBool();
         }
     }
     return found;
 }

 void basemanager::Insert(const sListePrepeses &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idPlanning;
     fields<<idListPP;
     fields<<idPlat;
     fields<<idIngredient;
     fields<<quantite;
     fields<<unite;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idPlanning);
     vals<<dw->ToSqlIntegerString(inR.idListPP);
     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlNumeric(inR.quantite);
     vals<<dw->ToSqlString(inR.unite);

     Insert(tListePrepeses,fields,vals);
 }

 void basemanager::Modify(const sListePrepeses &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;


     fields<<quantite;
     fields<<unite;
     wherefields<<idCroisiere;
     wherefields<<idPlanning;
     wherefields<<idListPP;
     wherefields<<idPlat;
     wherefields<<idIngredient;
     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idPlanning);
     wherevals<<dw->ToSqlIntegerString(inR.idListPP);
     wherevals<<dw->ToSqlIntegerString(inR.idPlat);
     wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlNumeric(inR.quantite);
     vals<<dw->ToSqlString(inR.unite);

     Modify(tListePrepeses,fields,vals,wherefields,wherevals); 
 }

 void basemanager::Initialise(sListePrepeses &inR){
     inR.idCroisiere=0;
     inR.idPlanning=0;
     inR.idListPP=0;
     inR.idPlat=0;
     inR.idIngredient=0;
     inR.quantite=0;
     inR.unite="";
 }
 bool basemanager::ReadRecord (sListePrepeses &inR){
     bool found(false);
     intvect fields,wherefields;
     stringvect wherevals;
     wherefields<<idCroisiere;
     wherefields<<idPlanning;
     wherefields<<idListPP;
     wherefields<<idPlat;
     wherefields<<idIngredient;
     fields<<quantite;
     fields<<unite;
     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idPlanning);
     wherevals<<dw->ToSqlIntegerString(inR.idListPP);
     wherevals<<dw->ToSqlIntegerString(inR.idPlat);
     wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
     DoSelectSomeFields(tListePrepeses,fields,MakeAndWhereString(wherefields,wherevals),0);
     if (quest.next()){
         found=true;
         inR.quantite=quest.value(0).toInt();
         inR.unite=quest.value(1).toString();
     }
     return found;
 }

 void basemanager::Insert(const sComposition &inR){
     intvect fields;
     stringvect vals;

     fields<<idMenu;
     fields<<numero;
     fields<<position;
     fields<<idPlat;

     vals<<dw->ToSqlIntegerString(inR.idMenu);
     vals<<dw->ToSqlIntegerString(inR.numero);
     vals<<dw->ToSqlString(inR.position);
     vals<<dw->ToSqlIntegerString(inR.idPlat);

     Insert(tComposition,fields,vals);
 }

 void basemanager::Modify(const sComposition &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idMenu;
     wherefields<<numero;
     fields<<position;
     fields<<idPlat;

     wherevals<<dw->ToSqlIntegerString(inR.idMenu);
     wherevals<<dw->ToSqlIntegerString(inR.numero);
     vals<<dw->ToSqlString(inR.position);
     vals<<dw->ToSqlIntegerString(inR.idPlat);

     Modify(tComposition,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sComposition &inR){
     inR.idMenu=0;
     inR.numero=0;
     inR.position="";
     inR.idPlat=0;
 }
 bool basemanager::ReadRecord (sComposition &inR){
     bool found(false);
     if (inR.idMenu&&inR.numero){
         intvect fields,wherefields;
         stringvect wherevals;
         wherefields<<idMenu;
         wherefields<<numero;
         wherevals<<dw->ToSqlIntegerString(inR.idMenu);
         wherevals<<dw->ToSqlIntegerString(inR.numero);
         fields<<position;
         fields<<idPlat;

         DoSelectSomeFields(tComposition,fields,MakeAndWhereString(wherefields,wherevals),0);
         if (quest.next()){
             found=true;
             inR.position=quest.value(0).toString();
             inR.idPlat=quest.value(1).toInt();
         }
     }
     return found;
 }
/*
 *          int idCroisiere;
         int idPlanning;
         int idPlatType;
         int idMenu;
         int numero;
     QString position;
         int idPlat;
         int nombrePersonnes;
         */
 void basemanager::Insert(const sIComposition &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idPlanning;
     fields<<idPlatType;
     fields<<idMenu;
     fields<<numero;
     fields<<position;
     fields<<idPlat;
     fields<<nombrePersonnes;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idPlanning);
     vals<<dw->ToSqlIntegerString(idPlatType);
     vals<<dw->ToSqlIntegerString(inR.idMenu);
     vals<<dw->ToSqlIntegerString(inR.numero);
     vals<<dw->ToSqlString(inR.position);
     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

     Insert(tIComposition,fields,vals);
 }

 void basemanager::Modify(const sIComposition &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idPlanning;
     wherefields<<numero;
     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idPlanning);
     wherevals<<dw->ToSqlIntegerString(inR.numero);

     fields<<idPlatType;
     fields<<idMenu;
     fields<<position;
     fields<<idPlat;
     fields<<nombrePersonnes;

     vals<<dw->ToSqlIntegerString(idPlatType);
     vals<<dw->ToSqlIntegerString(inR.idMenu);
     vals<<dw->ToSqlString(inR.position);
     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlIntegerString(inR.nombrePersonnes);

     Modify(tIComposition,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sIComposition &inR){
     inR.idMenu=0;
     inR.idPlat=0;
     inR.idPlatType=0;
     inR.nombrePersonnes=0;
     inR.numero=0;
     inR.position="";
 }

 bool basemanager::ReadRecord (sIComposition &inR){
     bool found(false);
     if (inR.idCroisiere&&inR.idPlanning&&inR.numero){
         intvect fields,wherefields;
         stringvect wherevals;

         wherefields<<idCroisiere;
         wherefields<<idPlanning;
         wherefields<<numero;

         wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
         wherevals<<dw->ToSqlIntegerString(inR.idMenu);
         wherevals<<dw->ToSqlIntegerString(inR.numero);

         fields<<idPlatType;
         fields<<idMenu;
         fields<<position;
         fields<<idPlat;
         fields<<nombrePersonnes;

         DoSelectSomeFields(tIComposition,fields,MakeAndWhereString(wherefields,wherevals),0);
         if (quest.next()){
             found=true;
             inR.idPlatType=quest.value(0).toInt();
             inR.idMenu=quest.value(1).toInt();
             inR.position=quest.value(2).toString();
             inR.idPlat=quest.value(3).toInt();
             inR.nombrePersonnes=quest.value(4).toInt();
         }
     }
     return found;
 }

 void basemanager::Insert(const sListeIngredients &inR){
     intvect fields;
     stringvect vals;

     fields<<idPlat;
     fields<<idIngredient;
     fields<<conditionnement;
     fields<<quantite;
     fields<<unite;

     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlString(inR.conditionnement);
     vals<<dw->ToSqlNumeric(inR.quantite);
     vals<<dw->ToSqlString(inR.unite);

     Insert(tListeIngredients,fields,vals);
 }

 void basemanager::Modify(const sListeIngredients &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idPlat;
     wherefields<<idIngredient;
     fields<<conditionnement;
     fields<<quantite;
     fields<<unite;

     wherevals<<dw->ToSqlIntegerString(inR.idPlat);
     wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlString(inR.conditionnement);
     vals<<dw->ToSqlNumeric(inR.quantite);
     vals<<dw->ToSqlString(inR.unite);

     Modify(tListeIngredients,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sListeIngredients &inR){
     inR.idPlat=0;
     inR.idIngredient=0;
     inR.conditionnement="";
     inR.quantite=0;
     inR.unite="";
 }

 bool basemanager::ReadRecord (sListeIngredients &inR){
     bool found(false);
     if (inR.idPlat&&inR.idIngredient){
         intvect fields,wherefields;
         stringvect wherevals;

         wherefields<<idPlat;
         wherefields<<idIngredient;
         fields<<conditionnement;
         fields<<quantite;
         fields<<unite;

         wherevals<<dw->ToSqlIntegerString(inR.idPlat);
         wherevals<<dw->ToSqlIntegerString(inR.idIngredient);

         DoSelectSomeFields(tListeIngredients,fields,MakeAndWhereString(wherefields,wherevals),0);
         if (quest.next()){
             found=true;
             inR.conditionnement=quest.value(0).toString();
             inR.quantite=quest.value(1).toFloat();
             inR.unite=quest.value(2).toString();
         }
     }
     return found;
 }

 void basemanager::Insert(const sListeUstensiles &inR){
     intvect fields;
     stringvect vals;

     fields<<idPlat;
     fields<<idUstensile;

     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlIntegerString(inR.idUstensile);

     Insert(tListeUstensiles,fields,vals);
 }

 void basemanager::Initialise(sListeUstensiles &inR){
     inR.idPlat=0;
     inR.idUstensile=0;
 }


 void basemanager::Insert(const sIngredients &inR){
     intvect fields;
     stringvect vals;

     fields<<idIngredient;
     fields<<nomIngredient;
     fields<<frais;
     fields<<conditionnement;
     fields<<conservationFrais;
     fields<<quantiteBase;
     fields<<unite;
     fields<<fractionnable;
     fields<<limInf;
     fields<<idRayon;

     vals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlString(inR.nomIngredient);
     vals<<dw->ToSqlBool(inR.frais);
     vals<<dw->ToSqlString(inR.conditionnement);
     vals<<dw->ToSqlBool(inR.conservationFrais);
     vals<<dw->ToSqlNumeric(inR.quantiteBase);
     vals<<dw->ToSqlString(inR.unite);
     vals<<dw->ToSqlBool(inR.fractionnable);
     vals<<dw->ToSqlNumeric(inR.limInf);
     vals<<dw->ToSqlIntegerString(inR.idRayon);

     Insert(tIngredients,fields,vals);
 }

 void basemanager::Modify(const sIngredients &inR){
     intvect fields;
     stringvect vals;

     fields<<nomIngredient;
     fields<<frais;
     fields<<conditionnement;
     fields<<conservationFrais;
     fields<<quantiteBase;
     fields<<unite;
     fields<<fractionnable;
     fields<<limInf;
     fields<<idRayon;

     vals<<dw->ToSqlString(inR.nomIngredient);
     vals<<dw->ToSqlBool(inR.frais);
     vals<<dw->ToSqlString(inR.conditionnement);
     vals<<dw->ToSqlBool(inR.conservationFrais);
     vals<<dw->ToSqlNumeric(inR.quantiteBase);
     vals<<dw->ToSqlString(inR.unite);
     vals<<dw->ToSqlBool(inR.fractionnable);
     vals<<dw->ToSqlNumeric(inR.limInf);
     vals<<dw->ToSqlIntegerString(inR.idRayon);

     Modify(tIngredients,fields,vals,idIngredient,dw->ToSqlIntegerString(inR.idIngredient));
 }

 void basemanager::Initialise(sIngredients &inR){
     inR.idIngredient=0;
     inR.nomIngredient="";
     inR.frais=false;
     inR.conditionnement="";
     inR.conservationFrais=false;
     inR.quantiteBase=0;
     inR.unite="";
     inR.fractionnable=0;
     inR.limInf=0;
     inR.idRayon=0;
 }

 bool basemanager::ReadRecord (sIngredients &inR){
     bool found(false);
     if (inR.idIngredient){
         DoSelectAllFields(tIngredients,MakeWhereString(idIngredient,dw->ToSqlIntegerString(inR.idIngredient)));
         if (quest.next()){
             found=true;
             inR.nomIngredient=quest.value(1).toString();
                     inR.frais=quest.value(2).toBool();
           inR.conditionnement=quest.value(3).toString();
         inR.conservationFrais=quest.value(4).toBool();
              inR.quantiteBase=quest.value(5).toInt();
                     inR.unite=quest.value(6).toString();
             inR.fractionnable=quest.value(7).toInt();
                    inR.limInf=quest.value(8).toInt();
                   inR.idRayon=quest.value(9).toInt();
         }
     }
    return found;
 }

 void basemanager::Insert(const sRayon &inR){
     intvect fields;
     stringvect vals;

     fields<<idRayon;
     fields<<nomRayon;

     vals<<dw->ToSqlIntegerString(inR.idRayon);
     vals<<dw->ToSqlString(inR.nomRayon);

     Insert(tRayon,fields,vals);
 }

 void basemanager::Modify(const sRayon &inR){
     Modify(tRayon,nomRayon,dw->ToSqlString(inR.nomRayon),idRayon,dw->ToSqlIntegerString(inR.idRayon));
 }

 void basemanager::Initialise(sRayon &inR){
     inR.idRayon=0;
     inR.nomRayon="";
 }
 bool basemanager::ReadRecord (sRayon &inR){
     bool found(false);
     if (inR.idRayon){
         DoSelectOneField(tRayon,nomRayon,MakeWhereString(idRayon,dw->ToSqlIntegerString(inR.idRayon)));
         if (quest.next()){
             found=true;
             inR.nomRayon=quest.value(0).toString();
         }
     }
     return found;
 }

 void basemanager::Insert(const sQuantites &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idPlanning;
     fields<<idPlat;
     fields<<idIngredient;
     fields<<quantite;
     fields<<nombrePersonnes;
     fields<<unite;
     fields<<prepese;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idPlanning);
     vals<<dw->ToSqlIntegerString(inR.idPlat);
     vals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlNumeric(inR.quantite);
     vals<<dw->ToSqlString(inR.unite);
     vals<<dw->ToSqlBool(inR.prepese);

     Insert(tQuantites,fields,vals);
 }

 void basemanager::Modify(const sQuantites &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idPlanning;
     wherefields<<idPlat;
     wherefields<<idIngredient;

     fields<<quantite;
     fields<<unite;
     fields<<prepese;

     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idPlanning);
     wherevals<<dw->ToSqlIntegerString(inR.idPlat);
     wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlNumeric(inR.quantite);
     vals<<dw->ToSqlString(inR.unite);
     vals<<dw->ToSqlBool(inR.prepese);

     Modify(tQuantites,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sQuantites &inR){
     inR.idCroisiere=0;
     inR.idPlanning=0;
     inR.idPlat=0;
     inR.idIngredient=0;
     inR.quantite=0;
     inR.unite="";
     inR.prepese=false;
 }

 bool basemanager::ReadRecord (sQuantites &inR){
     bool found(false);
     if (inR.idCroisiere&&inR.idPlanning&&inR.idPlat&&inR.idIngredient){
         intvect fields,wherefields;
         stringvect wherevals;

         wherefields<<idCroisiere;
         wherefields<<idPlanning;
         wherefields<<idPlat;
         wherefields<<idIngredient;

         wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
         wherevals<<dw->ToSqlIntegerString(inR.idPlanning);
         wherevals<<dw->ToSqlIntegerString(inR.idPlat);
         wherevals<<dw->ToSqlIntegerString(inR.idIngredient);

         fields<<quantite;
         fields<<unite;
         fields<<prepese;

         DoSelectSomeFields(tQuantites,fields,MakeAndWhereString(wherefields,wherevals),0);
         if (quest.next()){
             found=true;
             inR.quantite=quest.value(0).toFloat();
             inR.unite=quest.value(1).toString();
             inR.prepese=quest.value(2).toBool();
         }
     }
     return found;
 }

 void basemanager::Insert(const sUstensiles &inR){
     intvect fields;
     stringvect vals;

     fields<<idUstensile;
     fields<<nomUstensile;
     fields<<prepese;

     vals<<dw->ToSqlIntegerString(inR.idUstensile);
     vals<<dw->ToSqlString(inR.nomUstensile);

     Insert(tUstensiles,fields,vals);
 }

 void basemanager::Modify(const sUstensiles &inR){
     Modify(tUstensiles,nomUstensile,dw->ToSqlString(inR.nomUstensile),idUstensile,dw->ToSqlIntegerString(idUstensile));
 }

 void basemanager::Initialise(sUstensiles &inR){
     inR.idUstensile=0;
     inR.nomUstensile="";
 }
 bool basemanager::ReadRecord (sUstensiles &inR){
     bool found(false);
     if (inR.idUstensile){
         DoSelectOneField(tUstensiles,idUstensile,MakeWhereString(idUstensile,dw->ToSqlIntegerString(inR.idUstensile)));
         if (quest.next()){
             found=true;
             inR.nomUstensile=quest.value(0).toString();
         }
     }
     return found;
 }

 void basemanager::Insert(const sInventaire &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idIngredient;
     fields<<lieuDeRangement;
     fields<<conditionnement;
     fields<<quantite;
     fields<<dateEntree;
     fields<<dateSortie;
     fields<<dateReutilisation;
     fields<<datePeremption;
     fields<<aCommander;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlString(inR.lieuDeRangement);
     vals<<dw->ToSqlString(inR.conditionnement);
     vals<<dw->ToSqlIntegerString(inR.quantite);
     vals<<dw->ToSqlJulian(inR.dateEntree);
     vals<<dw->ToSqlJulian(inR.dateSortie);
     vals<<dw->ToSqlJulian(inR.dateReutilisation);
     vals<<dw->ToSqlJulian(inR.datePeremption);
     vals<<dw->ToSqlBool(inR.aCommander);

     Insert(tInventaire,fields,vals);
 }

 void basemanager::Modify(const sInventaire &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idIngredient;
     fields<<lieuDeRangement;
     fields<<conditionnement;
     fields<<quantite;
     fields<<dateEntree;
     fields<<dateSortie;
     fields<<dateReutilisation;
     fields<<datePeremption;
     fields<<aCommander;

     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlString(inR.lieuDeRangement);
     vals<<dw->ToSqlString(inR.conditionnement);
     vals<<dw->ToSqlIntegerString(inR.quantite);
     vals<<dw->ToSqlJulian(inR.dateEntree);
     vals<<dw->ToSqlJulian(inR.dateSortie);
     vals<<dw->ToSqlJulian(inR.dateReutilisation);
     vals<<dw->ToSqlJulian(inR.datePeremption);
     vals<<dw->ToSqlBool(inR.aCommander);

     Modify(tInventaire,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sInventaire &inR){
     inR.idCroisiere=0;
     inR.idIngredient=0;
     inR.lieuDeRangement="";
     inR.conditionnement="";
     inR.quantite=0;
     inR.dateEntree=QDate();
     inR.dateSortie=QDate();
     inR.dateReutilisation=QDate();
     inR.datePeremption=QDate();
     inR.aCommander=false;
 }
 bool basemanager::ReadRecord (sInventaire &inR){
     bool found(false);
     if (inR.idCroisiere&&inR.idIngredient){
         intvect wherefields;
         stringvect wherevals;

         wherefields<<idCroisiere;
         wherefields<<idIngredient;

         wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
         wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
         DoSelectAllFields(tInventaire,MakeAndWhereString(wherefields,wherevals));
         if (quest.next()){
             found=true;
             inR.lieuDeRangement=quest.value(2).toInt();
             inR.conditionnement=quest.value(3).toString();
             inR.quantite=quest.value(4).toFloat();
             inR.dateEntree=dw->FromSqlJulian(quest.value(5).toInt());
             inR.dateSortie=dw->FromSqlJulian(quest.value(6).toInt());
             inR.dateReutilisation=dw->FromSqlJulian(quest.value(7).toInt());
             inR.datePeremption=dw->FromSqlJulian(quest.value(8).toInt());
             inR.aCommander=quest.value(9).toBool();
         }
     }
    return found;
 }

 void basemanager::Insert(const sListeCourse &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idListeCourse;
     fields<<dateEcriture;
     fields<<dateCourses;
     fields<<duree;
     fields<<typeAchats;
     fields<<faite;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idListeCourse);
     vals<<dw->ToSqlJulian(inR.dateEcriture);
     vals<<dw->ToSqlJulian(inR.dateCourses);
     vals<<dw->ToSqlIntegerString(inR.duree);
     vals<<dw->ToSqlString(inR.typeAchats);
     vals<<dw->ToSqlIntegerString(inR.faite);

     Insert(tListeCourse,fields,vals);
 }

 void basemanager::Modify(const sListeCourse &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idListeCourse;
     fields<<dateEcriture;
     fields<<dateCourses;
     fields<<duree;
     fields<<typeAchats;
     fields<<faite;

     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idListeCourse);
     vals<<dw->ToSqlJulian(inR.dateEcriture);
     vals<<dw->ToSqlJulian(inR.dateCourses);
     vals<<dw->ToSqlIntegerString(inR.duree);
     vals<<dw->ToSqlString(inR.typeAchats);
     vals<<dw->ToSqlIntegerString(inR.faite);

     Modify(tListeCourse,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sListeCourse &inR){
     inR.idCroisiere=0;
     inR.idListeCourse=0;
     inR.dateEcriture=QDate();
     inR.dateCourses=QDate();
     inR.duree=0;
     inR.typeAchats="";
     inR.faite=false;
 }
 bool basemanager::ReadRecord (sListeCourse &inR){
     bool found(false);
     if (inR.idCroisiere&&inR.idListeCourse){
         intvect wherefields;
         stringvect wherevals;
         wherefields<<idCroisiere;
         wherefields<<idListeCourse;
         wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
         wherevals<<dw->ToSqlIntegerString(inR.idListeCourse);
         DoSelectAllFields(tListeCourse,MakeAndWhereString(wherefields,wherevals));
         if (quest.next()){
             found=true;
             inR.dateEcriture=dw->FromSqlJulian(quest.value(2).toInt());
             inR.dateCourses=dw->FromSqlJulian(quest.value(3).toInt());
             inR.duree=quest.value(4).toInt();
             inR.typeAchats=quest.value(5).toString();
             inR.faite=quest.value(6).toBool();
         }
     }
     return found;
 }

 void basemanager::Insert(const sItemsListeCourse &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idListeCourse;
     fields<<idIngredient;
     fields<<idRayon;
     fields<<quantite;
     fields<<trouve;
     fields<<platsOrigine;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idListeCourse);
     vals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlIntegerString(inR.idRayon);
     vals<<dw->ToSqlIntegerString(inR.quantite);
     vals<<dw->ToSqlIntegerString(inR.trouve);
     vals<<dw->ToSqlString(inR.platsOrigine);

     Insert(tItemsListeCourse,fields,vals);
 }

 void basemanager::Modify(const sItemsListeCourse &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idListeCourse;
     wherefields<<idIngredient;
     fields<<idRayon;
     fields<<quantite;
     fields<<trouve;
     fields<<platsOrigine;

     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idListeCourse);
     wherevals<<dw->ToSqlIntegerString(inR.idIngredient);
     vals<<dw->ToSqlIntegerString(inR.idRayon);
     vals<<dw->ToSqlIntegerString(inR.quantite);
     vals<<dw->ToSqlIntegerString(inR.trouve);
     vals<<dw->ToSqlString(inR.platsOrigine);

     Modify(tItemsListeCourse,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sItemsListeCourse &inR){
     inR.idCroisiere=0;
     inR.idListeCourse=0;
     inR.idIngredient=0;
     inR.idRayon=0;
     inR.quantite=0;
     inR.trouve=false;
     inR.platsOrigine="";
 }
 bool basemanager::ReadRecord (sItemsListeCourse &inR){
     bool found(false);
     if (inR.idCroisiere&&inR.idIngredient&&inR.idListeCourse){
         intvect fields,wherefields;
         stringvect wherevals;

         wherefields<<idCroisiere;
         wherefields<<idListeCourse;
         wherefields<<idIngredient;
         fields<<idRayon;
         fields<<quantite;
         fields<<trouve;
         fields<<platsOrigine;

         wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
         wherevals<<dw->ToSqlIntegerString(inR.idListeCourse);
         wherevals<<dw->ToSqlIntegerString(inR.idIngredient);

         DoSelectSomeFields(tItemsListeCourse,fields,MakeAndWhereString(wherefields,wherevals),0);
         if (quest.next()){
             found=true;
             inR.idRayon=quest.value(0).toInt();
             inR.quantite=quest.value(1).toFloat();
             inR.trouve=quest.value(2).toBool();
             inR.platsOrigine=quest.value(3).toString();
         }
     }
    return found;
 }

 void basemanager::Insert(const sConversions &inR){
     intvect fields;
     stringvect vals;

     fields<<idParametre;
     fields<<abbreviation;
     fields<<nomComplet;
     fields<<enMillilitres;
     fields<<enMilligrammes;

     vals<<dw->ToSqlIntegerString(inR.idParametre);
     vals<<dw->ToSqlString(inR.abbreviation);
     vals<<dw->ToSqlString(inR.nomComplet);
     vals<<dw->ToSqlNumeric(inR.enMillilitres);
     vals<<dw->ToSqlNumeric(inR.enMilligrammes);

     Insert(tConversions,fields,vals);
 }

 void basemanager::Modify(const sConversions &inR){
     intvect fields;
     stringvect vals;

     fields<<abbreviation;
     fields<<nomComplet;
     fields<<enMillilitres;
     fields<<enMilligrammes;

     vals<<dw->ToSqlString(inR.abbreviation);
     vals<<dw->ToSqlString(inR.nomComplet);
     vals<<dw->ToSqlNumeric(inR.enMillilitres);
     vals<<dw->ToSqlNumeric(inR.enMilligrammes);

     Modify(tConversions,fields,vals,idParametre,dw->ToSqlIntegerString(inR.idParametre));
 }

 void basemanager::Initialise(sConversions &inR){
     inR.idParametre=0;
     inR.abbreviation="";
     inR.nomComplet="";
     inR.enMillilitres=0;
     inR.enMilligrammes=0;
 }

 bool basemanager::ReadRecord (sConversions &inR){
     bool found(false);
     if (inR.idParametre){
         DoSelectAllFields(tConversions,MakeWhereString(idParametre,dw->ToSqlIntegerString(inR.idParametre)));
         if (quest.next()){
             found=true;
             inR.abbreviation=quest.value(1).toString();
             inR.nomComplet=quest.value(2).toString();
             inR.enMillilitres=quest.value(3).toFloat();
             inR.enMilligrammes=quest.value(4).toFloat();
         }
     }
     return found;
 }

 void basemanager::Insert(const sEquipets &inR){
     intvect fields;
     stringvect vals;

     fields<<idBateau;
     fields<<nomEquipet;
     fields<<idEquipet;
     fields<<idPhoto;
     fields<<pointX;
     fields<<pointY;
     fields<<labelX;
     fields<<labelY;

     vals<<dw->ToSqlIntegerString(inR.idBateau);
     vals<<dw->ToSqlString(inR.nomEquipet);
     vals<<dw->ToSqlIntegerString(inR.idEquipet);
     vals<<dw->ToSqlIntegerString(inR.idPhoto);
     vals<<dw->ToSqlNumeric(inR.pointX);
     vals<<dw->ToSqlNumeric(inR.pointY);
     vals<<dw->ToSqlNumeric(inR.labelX);
     vals<<dw->ToSqlNumeric(inR.labelY);

     Insert(tEquipets,fields,vals);
 }

 void basemanager::Modify(const sEquipets &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idBateau;
     fields<<nomEquipet;
     wherefields<<idEquipet;
     fields<<idPhoto;
     fields<<pointX;
     fields<<pointY;
     fields<<labelX;
     fields<<labelY;

     wherevals<<dw->ToSqlIntegerString(inR.idBateau);
     vals<<dw->ToSqlString(inR.nomEquipet);
     wherevals<<dw->ToSqlIntegerString(inR.idEquipet);
     vals<<dw->ToSqlIntegerString(inR.idPhoto);
     vals<<dw->ToSqlNumeric(inR.pointX);
     vals<<dw->ToSqlNumeric(inR.pointY);
     vals<<dw->ToSqlNumeric(inR.labelX);
     vals<<dw->ToSqlNumeric(inR.labelY);

     Modify(tEquipets,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sEquipets &inR){
     inR.idBateau=0;
     inR.nomEquipet="";
     inR.idEquipet=0;
     inR.idPhoto=0;
     inR.pointX=0;
     inR.pointY=0;
     inR.labelX=0;
     inR.labelY=0;
 }
 bool basemanager::ReadRecord (sEquipets &inR){
     bool found(false);
     if (inR.idBateau){
         DoSelectAllFields(tEquipets,MakeWhereString(idBateau,dw->ToSqlIntegerString(inR.idBateau)));
         if (quest.next()){
             found=true;
             inR.nomEquipet=quest.value(1).toString();
             inR.idEquipet=quest.value(2).toInt();
             inR.idPhoto=quest.value(3).toInt();
             inR.pointX=quest.value(4).toInt();
             inR.pointY=quest.value(5).toInt();
             inR.labelX=quest.value(6).toInt();
             inR.labelY=quest.value(7).toInt();
         }
     }
     return found;
 }

 void basemanager::Insert(const sBateau &inR){
     intvect fields;
     stringvect vals;

     fields<<idBateau;
     fields<<typeBateau;

     vals<<dw->ToSqlIntegerString(inR.idBateau);
     vals<<dw->ToSqlString(inR.typeBateau);

     Insert(tBateau,fields,vals);
 }

void basemanager::Modify(const sBateau &inR){
     Modify(tBateau,typeBateau,dw->ToSqlString(inR.typeBateau),idBateau,dw->ToSqlIntegerString(idBateau));
 }

void basemanager::Initialise(sBateau &inR){
    inR.idBateau=0;
    inR.typeBateau="";
}
bool basemanager::ReadRecord(sBateau &inR){
    bool found(false);
if (inR.idBateau){
    DoSelectOneField(tBateau,typeBateau,MakeWhereString(typeBateau,dw->ToSqlIntegerString(inR.idBateau)));
    if (quest.next()){
        found=true;
        inR.typeBateau=quest.value(0).toString();
    }

}
if (inR.typeBateau!=""){
    DoSelectOneField(tBateau,idBateau,MakeWhereString(typeBateau,dw->ToSqlString(inR.typeBateau)));
    if (quest.next()){
        found=true;
        inR.idBateau=quest.value(0).toInt();
    }
}
return found;
}

 void basemanager::Insert(const sPhotos &inR){
     intvect fields;
     stringvect vals;

     fields<<idBateau;
     fields<<idPhoto;
     fields<<fichierPhoto;

     vals<<dw->ToSqlIntegerString(inR.idBateau);
     vals<<dw->ToSqlIntegerString(inR.idPhoto);
     vals<<dw->ToSqlString(inR.fichierPhoto);

     Insert(tPhotos,fields,vals);
 }

 void basemanager::Modify(const sPhotos &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idBateau;
     wherefields<<idPhoto;
     fields<<fichierPhoto;

     wherevals<<dw->ToSqlIntegerString(inR.idBateau);
     wherevals<<dw->ToSqlIntegerString(inR.idPhoto);
     vals<<dw->ToSqlString(inR.fichierPhoto);

     Modify(tPhotos,fields,vals,wherefields,wherevals);
 }

 void basemanager::Initialise(sPhotos &inR){
     inR.idBateau=0;
     inR.idPhoto=0;
     inR.fichierPhoto="";
 }
 bool basemanager::ReadRecord (sPhotos &inR){
     bool found(false);
     intvect fields;stringvect vals;
     if (inR.idBateau&&inR.idPhoto){
         found=true;
         fields<<idBateau;
         fields<<idPhoto;
         vals<<dw->ToSqlIntegerString(inR.idBateau);
         vals<<dw->ToSqlIntegerString(inR.idPhoto);
         DoSelectOneField(tLocUstensiles,idEquipet,MakeAndWhereString(fields,vals));
         if (quest.next()) inR.fichierPhoto=quest.value(0).toString();
     }
     return found;
 }

 void basemanager::Insert(const sLocUstensiles &inR){
     intvect fields;
     stringvect vals;

     fields<<idCroisiere;
     fields<<idUstensile;
     fields<<idEquipet;

     vals<<dw->ToSqlIntegerString(inR.idCroisiere);
     vals<<dw->ToSqlIntegerString(inR.idUstensile);
     vals<<dw->ToSqlIntegerString(inR.idEquipet);

     Insert(tLocUstensiles,fields,vals);
 }

void basemanager::Modify(const sLocUstensiles &inR){
     intvect fields,wherefields;
     stringvect vals,wherevals;

     wherefields<<idCroisiere;
     wherefields<<idUstensile;
     fields<<idEquipet;

     wherevals<<dw->ToSqlIntegerString(inR.idCroisiere);
     wherevals<<dw->ToSqlIntegerString(inR.idUstensile);
     vals<<dw->ToSqlIntegerString(inR.idEquipet);

     Modify(tLocUstensiles,fields,vals,wherefields,wherevals);
 }

void basemanager::Initialise(sLocUstensiles &inR){
    inR.idCroisiere=0;
    inR.idUstensile=0;
    inR.idEquipet=0;
}
bool basemanager::ReadRecord (sLocUstensiles &inR){
    bool found(false);
    intvect fields;stringvect vals;
    if (inR.idCroisiere&&inR.idUstensile){
        fields<<idCroisiere;
        fields<<idUstensile;
        vals<<dw->ToSqlIntegerString(inR.idCroisiere);
        vals<<dw->ToSqlIntegerString(inR.idUstensile);
        DoSelectOneField(tLocUstensiles,idEquipet,MakeAndWhereString(fields,vals));
        if (quest.next()) {
            found=true;
            inR.idEquipet=quest.value(0).toInt();
        }
    }
    return found;
}

void basemanager::Insert(const sPavillons &inR){
    intvect fields;
    stringvect vals;

    fields<<idPavillon;
    fields<<categoriePavillon;
    fields<<bitpos;
    fields<<nomPavillon;

    vals<<dw->ToSqlIntegerString(inR.idPavillon);
    vals<<dw->ToSqlString(inR.categoriePavillon);
    vals<<dw->ToSqlIntegerString(inR.bitpos);
    vals<<dw->ToSqlString(inR.nomPavillon);

    Insert(tpavillons,fields,vals);
}

void basemanager::Modify(const sPavillons &inR){
    intvect fields;
    stringvect vals;

    fields<<categoriePavillon;
    fields<<bitpos;
    fields<<nomPavillon;

    vals<<dw->ToSqlString(inR.categoriePavillon);
    vals<<dw->ToSqlIntegerString(inR.bitpos);
    vals<<dw->ToSqlString(inR.nomPavillon);

    Modify(tpavillons,fields,vals,idPavillon,dw->ToSqlIntegerString(inR.idPavillon));
}

void basemanager::Initialise(sPavillons &inR){
    inR.idPavillon=0;
    inR.categoriePavillon="";
    inR.bitpos=0;
    inR.nomPavillon="";
}

bool basemanager::ReadRecord (sPavillons &inR){
    bool found(false);
    DoSelectAllFields(tpavillons,MakeWhereString(idPavillon,dw->ToSqlIntegerString(inR.idPavillon)));
    if (quest.next()){
        found=true;
        inR.categoriePavillon=quest.value(1).toString();
        inR.bitpos=quest.value(2).toInt();
        inR.nomPavillon=quest.value(3).toUInt();
    }
    return found;
}
