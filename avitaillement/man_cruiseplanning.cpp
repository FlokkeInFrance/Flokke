#include "man_cruiseplanning.h"
#include "debugger.h"

man_cruiseplanning::man_cruiseplanning() :
    basemanager(),
    activeDate(),
    //aCruise(),
    aPlanningItem(),
    aMenuType(),
    aPlanning(),
    aComposition(),
    aPlat(),
    aQuantites(),
    aMenu(),
    aIComposition(),
    allCruises(),
    cruisesToShow(nullptr),
    cruiseCommited(false)
{

}

man_cruiseplanning::~man_cruiseplanning(){

}

QStringList man_cruiseplanning::CruisesToShow(int ordered){
    allCruises.clear();
    allCruises.setTable(BaseId(tCroisiere));
    switch (ordered) {
    case 0:{
        allCruises.setFilter("");
        allCruises.sort(4,Qt::AscendingOrder);
    }
    case 1:{
        allCruises.setFilter(QString("%1 = %2").arg(BaseId(terminee),"0"));
        allCruises.sort(4,Qt::AscendingOrder);
    }
    }
    allCruises.select();

    cruisesToShow.clear();
    for (int reccount(0);reccount<allCruises.rowCount();reccount++){
        QString cn(""),dd(""),df("");
        cn=allCruises.record(reccount).value(3).toString();
        dd=datawizard::PrintSqlJulian(allCruises.record(reccount).value(4));
        df=datawizard::PrintSqlJulian(allCruises.record(reccount).value(5));
        cruisesToShow << (cn+" "+dd+"-> "+df);
    }
    return cruisesToShow;
}

sCroisiere man_cruiseplanning::GetSelectedCruise(){
    return aCruise;
}

sCroisiere man_cruiseplanning::SelectCruiseFromList(int pos){
    aCruise.idCroisiere=allCruises.record(pos).value(0).toInt();
    ReadRecord(aCruise);
    aPlanning.idCroisiere=aCruise.idCroisiere;
    if (ReadRecord(aPlanning)) cruiseCommited=true;
    else cruiseCommited=false;
    return aCruise;
}

QVector<sMenutype> man_cruiseplanning::GetMenuTypeForCruise(){
    QVector<sMenutype> retV;
    qDebug()<<"begin getmenutypeforcruise";
    sMenutype holder;
    intvect ordr{idCroisiere};
    holder.idCroisiere=aCruise.idCroisiere;
    holder.idMenuType=0;
    holder.nomMenuType="";
    holder.nombrePersonnes=aCruise.nombrePersonnes;
    DoSimpleSelectAllFields(tMenutype,idCroisiere,aCruise.idCroisiere,ordr);
    while (quest.next()){
        holder.idMenuType=quest.value(1).toInt();
        holder.nomMenuType=quest.value(2).toString();
        holder.nombrePersonnes=quest.value(3).toInt();
        retV<<holder;
    }
    return retV;
}

sCroisiere man_cruiseplanning::CreateCruise(){
    cruiseCommited=false;
    Initialise(aCruise);
    int incr=FindFreeSlot(tCroisiere,idCroisiere,0,"");
    aCruise.idCroisiere=incr;
    Insert(aCruise);
    BuildNewPlanType();
    return aCruise;
}

void man_cruiseplanning::EditACruise(sCroisiere inCruise){
    aCruise=inCruise;
    Modify(aCruise);
}

void man_cruiseplanning::CommitNewCruise(){
    if (!cruiseCommited){
        LoadOrCreateBoatType(aCruise.typeBateau);
        GenerateNewCruisePlan(aCruise.dateDepart,aCruise.dateFin);
        GenerateNewMenuPlan(aCruise.dateDepart,aCruise.dateFin);
        cruiseCommited=true;
    }
    else
    {
        LoadOrCreateBoatType(aCruise.typeBateau);
    }
    //Close cruise Editor, go for Planning
}

sCroisiere man_cruiseplanning::ChangeCruiseTiming(QDate departure, QDate arrival, int duration){
    int dys(duration),shiftD(0),diff(0);
    if (!dys) dys=1;

    if (!departure.isValid()&&!arrival.isValid()) {
        debug->ShowVariable("man_cruisepl : two dates are invalid detected");
        aCruise.nombreJours=dys;
        Modify(aCruise);
        return aCruise;}

    if (departure.isValid()&&departure!=aCruise.dateDepart){
        shiftD=aCruise.dateDepart.toJulianDay()-departure.toJulianDay();
        aCruise.dateDepart=departure;
        aCruise.dateFin=departure.addDays(dys-1);
        Modify(aCruise);
        if (cruiseCommited){
                ShiftCruiseNDays(shiftD);
            }
        return aCruise;
    }

    if (dys!=aCruise.nombreJours){
        if (!arrival.isValid()) aCruise.dateDepart=aCruise.dateFin.addDays(1-dys);
        else
            aCruise.dateFin=aCruise.dateDepart.addDays(dys-1);
        diff=dys-aCruise.nombreJours;
        aCruise.nombreJours=dys;
        if (!cruiseCommited) {
            Modify(aCruise);
            return aCruise;}
    }

    if (arrival.isValid()&&arrival!=aCruise.dateFin) {

        aCruise.dateFin=arrival;
        dys=aCruise.dateDepart.daysTo(aCruise.dateFin);
        diff=dys-aCruise.nombreJours;
        aCruise.nombreJours=(dys+1);
    }

     if( diff>0 && cruiseCommited){
         GenerateNewCruisePlan(aCruise.dateFin.addDays(1-diff),aCruise.dateFin);
         GenerateNewMenuPlan(aCruise.dateFin.addDays(1),arrival);
     }
      else{
        if (cruiseCommited) DeleteAfter(arrival);
     }
    Modify(aCruise);
    return aCruise;
}

void man_cruiseplanning::GenerateNewCruisePlan(QDate d1,QDate d2){
    if (d1<=d2){
        sLieux leg;
        leg.idCroisiere=aCruise.idCroisiere;
        leg.lieu="lieu";
        leg.commerce=false;
        leg.depannage=true;
        leg.amarre="type amarrage";

        for (QDate cd=d1;cd<=d2;cd=cd.addDays(1)){
            leg.date=cd;
            Insert(leg);
        }
    }
}

void man_cruiseplanning::GenerateNewMenuPlan(QDate d1,QDate d2){
    if (d1<=d2){
        QVector<sMenutype> listMT=GetMenuTypeForCruise();
        sPlanning nPlan;
        nPlan.idCroisiere=aCruise.idCroisiere; void AddMenuToDay(int pos);
        int sz=listMT.size();
        int idPlanng(1);
        if (sz){
            for (QDate cd=d1;cd<=d2;cd=cd.addDays(1)){
                for (int mealT=0;mealT<sz;mealT++){
                    nPlan.idPlanning=idPlanng;
                    nPlan.dateRepas=cd;
                    nPlan.idMenuType=listMT[mealT].idMenuType;
                    nPlan.idMenu=0;
                    nPlan.numRepas=mealT+1;
                    nPlan.momentRepas=listMT[mealT].nomMenuType;
                    nPlan.nombrePersonnes=listMT[mealT].nombrePersonnes;
                    Insert(nPlan);
                    idPlanng++;
                }
            }
        }
    }
 }

void man_cruiseplanning::DeleteAfter(QDate sErase){
    if (sErase!=QDate()){
        QString dString="DELETE FROM %1 WHERE %2 AND %3";
        QString exString=dString.arg(BaseId(tPlanning),
                                     MakeWhereString(idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere)),
                                     MakeWhereString(dateRepas,dw->ToSqlJulian(sErase),">"));
        OrderAndCommit(exString);
        exString=dString.arg(BaseId(tLieux),
                             MakeWhereString(idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere)),
                             MakeWhereString(date,dw->ToSqlJulian(sErase),">"));
        //Delete for tIcomposition, tprépesés, tquantités
    }
}

void man_cruiseplanning::ShiftCruiseNDays(int nDays){
//shift Tplanning
    QString shiftQ="UPDATE %1 SET %2=%2+%3 WHERE %4";
    OrderAndCommit(shiftQ.arg(BaseId(tPlanning),BaseId(dateRepas),dw->ToSqlIntegerString(nDays),MakeWhereString(idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere))));
//shift tLieux
    OrderAndCommit(shiftQ.arg(BaseId(tLieux),BaseId(date),dw->ToSqlIntegerString(nDays),MakeWhereString(idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere))));
//shift tListeCourse
    OrderAndCommit(shiftQ.arg(BaseId(tListeCourse),BaseId(dateCourses),dw->ToSqlIntegerString(nDays),MakeWhereString(idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere))));
}

bool man_cruiseplanning::LoadCruise(int idC){
    /*tCroisiere :
     idCroisiere, nomBateau, typeBateau, intitule, dateDepart, dateFin, nombreJours,nombrePersonnes, terminee
     */
    bool cruiseLoaded(true);
    Initialise(aCruise);
    aCruise.idCroisiere=idC;
    ReadRecord(aCruise);
    LoadOrCreateBoatType(aCruise.typeBateau);
    return cruiseLoaded;
}

void man_cruiseplanning::LoadOrCreateBoatType(const QString &bType){
    if (!bType.isEmpty()) {

        QSqlQuery quest(db);
        QString selc="SELECT %1.%2 FROM %1 WHERE %1.%3=%4";
        selc=selc.arg(BaseId(tBateau),BaseId(idBateau),BaseId(typeBateau),datawizard::ToSqlString(bType));
        quest.exec(selc);
        if (quest.next()){
            aCruise.typeBateau=bType;
        }
        else{
            int nextBoat=FindFreeSlot(tBateau,idBateau,0,"");
            sBateau nBoat;
            nBoat.idBateau=nextBoat;
            nBoat.typeBateau=dw->ToSqlString(bType);
            Insert(nBoat);
            aCruise.typeBateau=bType;
        }
    }
    else
        aCruise.typeBateau="";
}

void man_cruiseplanning::ChangeBoatName( const QString &nName){
    if (nName.isEmpty()) return;
    aCruise.nomBateau=nName;
    Modify(aCruise);
}

void man_cruiseplanning::ChangeBoatType( const QString &nType){
    if (nType.isEmpty()) return;
    LoadOrCreateBoatType(nType);
    Modify(aCruise);
}

void man_cruiseplanning::ChangeNumberPersons(int nPers){
    //int activeCruise,activeMenuType,activeDishType,activePlanningLine,activeMenu,activeDish,activeInventory,activeIngredient,activeTool,activeShopList;
    //int activeLocker,idActiveBoat;
    if(!nPers) return;
    int oldNP = aCruise.nombrePersonnes;
    if (oldNP!=nPers){
        aCruise.nombrePersonnes=nPers;
        Modify(aCruise);
        //Modify and recompute where nPersons=oldNP: tQuantites
        //Modify only : tIComposition,tMEnutype,tPlattype,tPlanning
        //first get list of dishes in tIComposition where nbPersons = oldNP idPlanning and idPlat
        QString snPers=dw->ToSqlIntegerString(nPers);
        intvect fields;
        stringvect vals;
        fields<<idCroisiere<<nombrePersonnes;
        vals<<dw->ToSqlIntegerString(aCruise.idCroisiere)<<dw->ToSqlIntegerString(oldNP);
        Modify(tMenutype,nombrePersonnes,snPers,fields,vals);
        Modify(tPlattype,nombrePersonnes,snPers,fields,vals);
        if (cruiseCommited){
            QString whereP=MakeAndWhereString(fields,vals);
            whereP=whereP+" AND "+MakeWhereString(dateRepas,dw->ToSqlJulian(QDate::currentDate()),">=");
            Modify(tPlanning,nombrePersonnes,snPers,whereP);
        }
//Select idPlat from tIComposition Where numberPersons=oldNP Join tPlanning on tIcomposition.idPlanning=tPlanning.idPlanning AND tPlanning.dateRepas>=Today
    }
}

void man_cruiseplanning::DeleteActiveCruise(){
    DeleteCruiseId(aCruise.idCroisiere);
    //reintialise all status vars
    aCruise.nombreJours=0;
    aCruise.nombrePersonnes=0;
    activeDate=QDate();
    aCruise.dateDepart=QDate();
    aCruise.dateFin=QDate();
    aCruise.nomBateau="";
    aCruise.intitule="";
    aCruise.nomBateau="";
}

void man_cruiseplanning::DeleteCruiseId(int inId){

     Delete(tCroisiere,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tLieux,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tMenutype,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tPlattype,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tPlanning,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tInventaire,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tListeCourse,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tItemsListeCourse,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tQuantites,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tListePrepeses,idCroisiere,datawizard::ToSqlIntegerString(inId));
     Delete(tIComposition,idCroisiere,datawizard::ToSqlIntegerString(inId));

}

void man_cruiseplanning::FinishCruise(){
    aCruise.terminee=true;
    Modify(aCruise);
}

void man_cruiseplanning::BuildNewPlanType(){

    sMenutype mt;
    sPlattype pt;
    Initialise(mt);
    Initialise(pt);
    mt.idCroisiere=aCruise.idCroisiere;
    mt.idMenuType=1;
    mt.nombrePersonnes=aCruise.nombrePersonnes;
    pt.idCroisiere=aCruise.idCroisiere;
    pt.nombrePersonnes=aCruise.nombrePersonnes;
    if (!ReadRecord(mt)){
        QStringList pmenus=paramMan->GetMeals();
        QStringList dishs=paramMan->GetDishes();
        for (int nmenu(1);nmenu<=3;nmenu++){
            mt.idMenuType=nmenu;
            mt.nomMenuType=pmenus[nmenu-1];
            Insert(mt);
            pt.idMenuType=nmenu;
            if(nmenu==1){
                pt.idPlatType=1;
                pt.nomPlatType=dishs[0];
                Insert(pt);
            }
            else
                for (int ndish(2);ndish<=4;ndish++){
                    pt.idPlatType=ndish-1;
                    pt.nomPlatType=dishs[ndish-1];
                    Insert(pt);
                }
        }
    }
}

void man_cruiseplanning::AddNewMenuToType(QString inType, int inPos){
    int posinsert(0);
//add the type to tMenutype nPos=0 : append, nPos=1 prepend
    if (inPos>0){
        posinsert=inPos;
        ShiftIndex(tMenutype,idMenuType,inPos,idCroisiere,datawizard::ToSqlIntegerString(aCruise.idCroisiere));
        ShiftIndex(tPlattype,idMenuType,inPos,idCroisiere,datawizard::ToSqlIntegerString(aCruise.idCroisiere));
    }
    else {
        posinsert=FindLastIdx(tMenutype,idMenuType,idCroisiere,datawizard::ToSqlIntegerString(aCruise.idCroisiere));
    }
    sMenutype mt;
    Initialise(mt);
    mt.idCroisiere=aCruise.idCroisiere;
    mt.idMenuType=posinsert;
    mt.nomMenuType=inType;
    mt.nombrePersonnes=aCruise.nombrePersonnes;
    Insert(mt);
//Add the menu to the planning, excluding days before this one if the cruise is ongoing
}

void man_cruiseplanning::AddNewDishToMenuType(int pos){
    intvect fields;
    stringvect vals;

    fields<<idCroisiere<<idMenuType;
    vals<<dw->ToSqlIntegerString(aCruise.idCroisiere)<<dw->ToSqlIntegerString(aMenuType.idMenuType);

    ShiftIndex(tPlattype,idPlatType,pos,fields,vals);
    sPlattype aPT;
    Initialise(aPT);
    aPT.idCroisiere=aCruise.idCroisiere;
    aPT.idMenuType=aMenuType.idMenuType;
    aPT.nombrePersonnes=aCruise.nombrePersonnes;
    Insert(aPT);
}

void man_cruiseplanning::ModifyMenuNameInType(QString inNName){
    aMenuType.nomMenuType=inNName;
    Modify(aMenuType);
}

void man_cruiseplanning::SelectMenuType(int mT){
    Initialise(aMenuType);
    aMenuType.idCroisiere=aCruise.idCroisiere;
    aMenuType.idMenuType=mT;
    ReadRecord(aMenuType);
}

void man_cruiseplanning::AddMenuToDay(int pos){
    //find idPlanning for pos on activeDate
    QString reqPl("SELECT %1, %2 FROM %3 WHERE %4 AND %5");
    reqPl=reqPl.arg(BaseId(idPlanning),
                    BaseId(numRepas),
                    BaseId(tPlanning),
                    MakeWhereString(idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere),
                    MakeWhereString(dateRepas,dw->ToSqlJulian(activeDate))));
    ExecuteSelect(reqPl);
    int posId(0);
    while (quest.next())
    {
        if (quest.value(1).toInt()==pos)
            posId=quest.value(0).toInt();
    }
    if (!posId){
        posId=quest.value(0).toInt();
    }
    //shift idPlanning

    ShiftIndex(tPlanning,idPlanning,posId,idCroisiere,dw->ToSqlIntegerString(aCruise.idCroisiere));
    //shift pos for day
    intvect flds;
    stringvect vals;
    flds<<idCroisiere;vals<<dw->ToSqlIntegerString(aCruise.idCroisiere);
    flds<<dateRepas;vals<<dw->ToSqlJulian(activeDate);
    ShiftIndex(tPlanning,numRepas,pos,flds,vals);
    //insert value
    sPlanning aPlanning;
    Initialise(aPlanning);
    aPlanning.idCroisiere=aCruise.idCroisiere;
    aPlanning.dateRepas=activeDate;
    aPlanning.numRepas=pos;
    aPlanning.idPlanning=posId;
    aPlanning.nombrePersonnes=aCruise.nombrePersonnes;
    Insert(aPlanning);
    //add quantities (and prepesés if already computed for another location)
}

void man_cruiseplanning::ClearCurrentPlanningSlot(){
    int oMenu=aPlanning.idMenu;
    sMenu mm;
    Initialise(mm);
    mm.idMenu=oMenu;
    ReadRecord(mm);
    if (!mm.favori) DeleteMenu(oMenu);
    aPlanning.idMenu=0;
    //Delete quantites and ListePrepeses
    intvect flds; stringvect vals;
    flds<<idCroisiere<<idPlanning;
    vals<<dw->ToSqlIntegerString(aCruise.idCroisiere)<<dw->ToSqlIntegerString(aPlanning.idPlanning);
    Delete(tListePrepeses,flds,vals);
    Delete(tQuantites,flds,vals);
}

int  man_cruiseplanning::MakeNewMenu(int idMenuT){//Makes a new menu with placeholders according to menu type, at current position
    int newM(0);
    if (idMenuT){
        //seek types of dishes for current menu type
        QString seltypes("");
        intvect flds,fflds;
        stringvect vals;
        flds<<idCroisiere<<idMenuType;
        vals<<dw->ToSqlIntegerString(aPlanning.idCroisiere)<<dw->ToSqlIntegerString(idMenuT);
        QString whereS=MakeAndWhereString(flds,vals);
        //get ordered list of generic dishes
        DoSelectOneField(tPlattype,nomPlatType,whereS,idPlatType);
        stringvect nommenus;
        while (quest.next()){
            nommenus<<quest.value(0).toString();
        }
        //construct menu according to menu type's names
        if (nommenus.count()){
            //find number of new menu
            newM=FindFreeSlot(tMenu,idMenu);
            //create new menu with generic name
            QString nMName="Menu"+QString::number(newM);
            Initialise(aMenu);
            aMenu.nomMenu=nMName;
            aMenu.idMenu=newM;
            aMenu.favori=false;
            Insert(aMenu);
            //create composition
            for (int cpt(0);cpt<nommenus.count();cpt++){
                sComposition nP;
                Initialise(nP);
                nP.idMenu=newM;
                nP.numero=cpt+1;
                nP.position=nommenus[cpt];
                Insert(nP);
            }
        }
    }
    else{
        newM=FindFreeSlot(tMenu,idMenu);
        //create new menu with generic name
        QString nMName="Menu"+QString::number(newM);
        Initialise(aMenu);
        aMenu.nomMenu=nMName;
        aMenu.idMenu=newM;
        aMenu.favori=false;
        Insert(aMenu);
        sComposition nP;
        Initialise(nP);
        nP.idMenu=newM;
        nP.numero=1;
        nP.position=langue->GetWord(FirstDish);
        Insert(nP);
    }
    return newM;
}

int man_cruiseplanning::MakeNewMenu(QString mName){
    int nMen=MakeNewMenu(aPlanning.idMenuType);
    if (nMen){
        aMenu.nomMenu=mName;
        Modify(aMenu);
    }
    return nMen;
}

void man_cruiseplanning::DeleteActiveMenu(){
    DeleteMenu(aMenu.idMenu);
    Initialise(aMenu);
}
void man_cruiseplanning::DeleteMenu(int inMenu){
    Delete(tMenu, idMenu,dw->ToSqlIntegerString(inMenu));
    Delete(tComposition, idMenu,dw->ToSqlIntegerString(inMenu));
    Delete(tIComposition,idMenu,dw->ToSqlIntegerString(inMenu));
    Delete(tQuantites,idMenu,dw->ToSqlIntegerString(inMenu));
    Delete(tListePrepeses,idMenu,dw->ToSqlIntegerString(inMenu));
}

void man_cruiseplanning::InsertMenuInDay(int inLoc,int inMenu){
    SelectPlanningItem(inLoc);
    aPlanning.idMenu=inMenu;
    Modify(aPlanning);
    Initialise(aMenu);
    aMenu.idMenu=inMenu;
    ReadRecord(aMenu);
    aMenu.favori=true;
    Modify(aMenu);
}

void man_cruiseplanning::InsertDishIntoMenu(int pos, int nDish, int nPlan){
    if (aMenu.favori){
        //vérifier si on peut modifier un menu déjà utilisé avec un dialogue

    }

    sPlat locdish;
    locdish.idPlat=nDish;
    ReadRecord(locdish);
    //lire en composition l'enregistrement nmenu
    sComposition ncompo;
    ncompo.idMenu=aMenu.idMenu;
    ncompo.numero=pos;
    ReadRecord(ncompo);
    ncompo.idPlat=nDish;
    Modify(ncompo);
    sPlanning locplan;
    locplan.idCroisiere=aCruise.idCroisiere;
    locplan.idPlanning=nPlan;
    ReadRecord(locplan);
    sIComposition locCompo;
    locCompo.idCroisiere=aCruise.idCroisiere;
    locCompo.idPlanning=nPlan;
    locCompo.numero=pos;
    ReadRecord(locCompo);
    int oldDish=locCompo.idPlat;
    if (oldDish){
        //Delete Quantities for oldDish
        //Delete Prepeses for oldDish
    }
    locCompo.idPlat=nDish;
    Modify(locCompo);
    GenerateQuantities(nDish,nPlan,locCompo.nombrePersonnes);
}

void man_cruiseplanning::GenerateQuantities(int inDish,int nPlan,int nbPers){
 //read dish info
 sPlat mp;
 Initialise(mp);
 mp.idPlat=inDish;
 ReadRecord(mp);
 intvect fields;
 stringvect vals;
 fields<<idCroisiere;vals<<dw->ToSqlIntegerString(aCruise.idCroisiere);
 fields<<idPlat;vals<<dw->ToSqlIntegerString(inDish);

//try to find quantities and/or prepesés in current cruise for this dish
 DoSelectAllFields(tQuantites, MakeAndWhereString(fields,vals));
 if (quest.next()){
     // if found, just copy
     sQuantites rQ;
     rQ.idCroisiere=aCruise.idCroisiere;
     rQ.idPlanning=nPlan;
     rQ.idPlat=inDish;
     int posingr=quest.record().indexOf(BaseId((idIngredient)));
     int posQt=quest.record().indexOf(BaseId((quantite)));
     int posunite=quest.record().indexOf(BaseId((unite)));
     int posnb(0);
     do{
         rQ.idIngredient=quest.value(posingr).toInt();
         rQ.quantite=quest.value(posQt).toFloat();
         rQ.unite=quest.value(posunite).toString();
         Insert(rQ);
     } while(quest.next());
     //look for prepeses as well, and copy if found
     DoSelectAllFields(tListePrepeses,MakeAndWhereString(fields,vals));
     if (quest.next()){
         sListePrepeses rP;
         rP.idCroisiere=aCruise.idCroisiere;
         rP.idPlanning=aPlanning.idPlanning;
         rP.idPlat=inDish;
          posingr=quest.record().indexOf(BaseId((idIngredient)));
          posQt=quest.record().indexOf(BaseId((quantite)));
          posnb=quest.record().indexOf(BaseId((idListPP)));
          posunite=quest.record().indexOf(BaseId((unite)));
         do {
              rP.idIngredient=quest.value(posingr).toInt();
              rP.quantite=quest.value(posQt).toFloat();
              rP.idListPP=quest.value(posnb).toInt();
              rP.unite=quest.value(posunite).toString();
              Insert(rP);
         }while (quest.next());
     }
 }
 else{
     //if quantities do not exist elsewhere create them for the menu
     //get ingredients for the dish...

     DoSelectAllFields(tListeIngredients,MakeWhereString(idPlat,dw->ToSqlIntegerString(inDish)));
     if (quest.next()){
         sQuantites nQ;
         nQ.idCroisiere=aCruise.idCroisiere;
         nQ.idPlat=inDish;

         int idI=quest.record().indexOf(BaseId(idIngredient));
         int qty=quest.record().indexOf(BaseId(quantite));
         int uty=quest.record().indexOf(BaseId(unite));
         do {
             nQ.idIngredient=quest.value(idI).toInt();
             nQ.unite=quest.value(uty).toString();
             nQ.quantite=quest.value(qty).toFloat();
             Insert(nQ);
         }while (quest.next());
     }
     //look if nb persons for the dish is different from nb of persons in instance of menu
     AdaptQuantitiesToNbOfPersons(aPlanning.idPlanning,mp.idPlat,nbPers,mp.nbPersonnes);

 }
}

void man_cruiseplanning::AdaptQuantitiesToNbOfPersons(int inPlanning, int inPlat, int nbPformeal, int nbPersonsfordish){
    if (nbPformeal!=nbPersonsfordish){
        DoSelectOneField(tPlat,fractionnable,MakeWhereString(idPlat,dw->ToSqlIntegerString(inPlat)));
        if (quest.next()){
            if (quest.value(0).toInt()){
                intvect fields,wheref;
                stringvect vals;
                fields<<idIngredient<<quantite<<unite;
                wheref<<idCroisiere<<idPlanning<<idPlat;
                vals<<dw->ToSqlIntegerString(aCruise.idCroisiere)<<dw->ToSqlIntegerString(inPlanning)<<dw->ToSqlIntegerString(inPlat);
                DoSelectSomeFields(tQuantites,fields,MakeAndWhereString(wheref,vals),0);
                if (quest.next()){
                    int qtf=quest.record().indexOf(BaseId(quantite));
                    int igrd=quest.record().indexOf(BaseId(idIngredient));
                    int uni=quest.record().indexOf(BaseId(unite));
                    sQuantites nqt;
                    nqt.idCroisiere=aCruise.idCroisiere;
                    nqt.idPlanning=inPlanning;
                    nqt.idPlat=inPlat;
                    do{
                        nqt.quantite=quest.value(qtf).toFloat();
                        nqt.unite=quest.value(uni).toString();
                        nqt.idIngredient=quest.value(igrd).toInt();
                        nqt.unite=dw->AdjustForNbPersons(nqt.unite,nqt.quantite,nbPformeal,nbPersonsfordish);
                        Modify(nqt);
                    } while (quest.next());
                }
            }
        }
    }
}

void man_cruiseplanning::ReplaceDishInMenu(int nDish){}//will delete the menu if it has no dishes
void man_cruiseplanning::RemoveDishFromMenu(int nDish){}
void man_cruiseplanning::SetCurrentDate(QDate cDate){
    activeDate=cDate;
}
void man_cruiseplanning::SelectPlanningItem(int pNum){//Selects the line but also the Menu in ActiveMenu
    Initialise(aPlanning);
    aPlanning.idCroisiere=aCruise.idCroisiere;
    aPlanning.idPlanning=pNum;
    ReadRecord(aPlanning);
    activeDate=aPlanning.dateRepas;
}
void man_cruiseplanning::SelectMenu(int menuNb){}
void man_cruiseplanning::SelectDish(int dishNb){}

void man_cruiseplanning::ReadDish(int numb){
    Initialise(aPlat);
    aPlat.idPlat=numb;
    ReadRecord(aPlat);
}

void man_cruiseplanning::InsertIngrToDish(int inIngr,int inPos){
   /* tListeIngredients
       idPlat :  INTEGER
       idIngredient :  INTEGER
       conditionnement :  TEXT
       quantite :  NUMERIC
       unite :  TEXT
   */
ShiftIndex(tListeIngredients,idIngredient,inPos,idPlat,dw->ToSqlIntegerString(aPlat.idPlat));
sIngredients ingr;
ingr.idIngredient=inIngr;
ReadRecord(ingr);
sListeIngredients nI;
nI.idIngredient=inPos;
nI.idIngredient=inIngr;
nI.conditionnement=ingr.conditionnement;
nI.quantite=0;
nI.unite="";
Insert(ingr);

}

int  man_cruiseplanning::MakeNewIngr(QString inIngr){
   /* tIngredients
       idIngredient :  INTEGER PRIMARY KEY
       nomIngredient :  TEXT
       frais :  BOOLEAN
       conditionnement :  TEXT
       conservationFrais :  BOOLEAN
       quantiteBase :  NUMERIC
       unite :  TEXT
       fractionnable :  BOOLEAN
       limInf :  NUMERIC
       idRayon :  INTEGER
*/
    sIngredients nwIngr;
    Initialise(nwIngr);
    nwIngr.nomIngredient=inIngr;
   int rIdx=FindFreeSlot(tIngredients,idIngredient);
   nwIngr.idIngredient=rIdx;
   Insert(nwIngr);
   return rIdx;
}
