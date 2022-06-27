#include "checkbase.h"
/*
tCroisiere :
     idCroisiere, nomBateau, typeBateau, intitule, dateDepart, dateFin, nombreJours,nombrePersonnes, terminee

tMenutype:
    idCroisiere,idMenu,titreMenu

tPlattype:
    idCroisiere,idMenu,idPlat,titrePlat

tPlanning :
     idCroisiere, idPlanning , idMenu,dateRepas,momentRepas,nombrePersonnes

tLieux :
    idCroisiere, date, lieu, amarre, contact, depannage, commerce

tMenu :
     idMenu, nomMenu, favori

tComposition
      idMenu, numero, position, idPlat
tIComposition
    idCroisiere,idPlanning,idMenu,idPlatType,nbPersonnes,idPlat

tPlat :
     idPlat,titrePlat,  categoriePlat, recette, auFour, auFrigo, tempsPreparation, tempsCuisson, nbPersonnes, flagsDiet,flagsNav,recetteBase,fractionnable

tListeIngredients :
     idPlat,idIngredient, conditionnement, quantite, unite

tListeUstensiles :
    idPlat, idUstensile

tIngredients :
    idIngredient,nomIngredient, frais,conditionnement,quantiteBase, unite, fractionnable,limInf,idRayon

tQuantites:
    idCroisiere, idPlat, idMenu, idIngredient, quantite, unite, prepese

tUstensiles :
    idUstensile, nomUstensile, lieuDeRangement

tInventaire :
    idCroisiere,lieuDeRangement,conditionnement,quantite,dateEntree,dateSortie,dateReutilisation,datePeremption,aCommander

tListeCourse :
    idCroisiere,idListeCourse,dateEcriture,duree,typeAchats,dateCourses,nomListe,faite

tItemsListeCourse :
    idListeCourse,idIngredient,idRayon,quantite,platsOrigine,trouve

tConversions,
        idParametre,abbreviation,nomComplet,enMillilitres,enMilligrammes,

tEquipets:
    idEquipet, nomEquipet, idBateau, idPhoto, pointX, pointY, labelX, labelY

tBateau:
     idBateau, typeBateau

tPhotos:
     idBateau, idPhoto, fichierPhoto
*/

CheckBase::CheckBase() :
    db(QSqlDatabase::database()),
    newBase(false),
    structureList()

{

}

QString CheckBase::BaseId(int inId){
    return datawizard::dbIdentifiers[inId];
}

void CheckBase::MakeNewBase(){
    QStringList statements;
    //Define the tables in the database
    QStringList fieldList,typeList;
    structureList.clear();

//tbCroisière : general data for the cruise to prepare or maintain
     fieldList<<BaseId(idCroisiere); typeList<<("INTEGER PRIMARY KEY");
       fieldList<<BaseId(nomBateau); typeList<<("TEXT");
      fieldList<<BaseId(typeBateau); typeList<<("TEXT");
        fieldList<<BaseId(intitule); typeList<<("TEXT");
      fieldList<<BaseId(dateDepart); typeList<<("INTEGER");
         fieldList<<BaseId(dateFin); typeList<<("INTEGER");
     fieldList<<BaseId(nombreJours); typeList<<("INTEGER");
 fieldList<<BaseId(nombrePersonnes); typeList<<("INTEGER");
        fieldList<<BaseId(terminee); typeList<<("BOOLEAN");

    statements<<MakeCreateStatement(BaseId(tCroisiere),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tMenutype : generic menu layout, will be used when making a new plan, can be modified on the cruise creation window
     fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
          fieldList<<BaseId(idMenuType); typeList<<("INTEGER"); //orders the successions of menus in a day
       fieldList<<BaseId(nomMenuType); typeList<<("TEXT");
 fieldList<<BaseId(nombrePersonnes); typeList<<("INTEGER"); //this propagates through the whole process, but will be adjusted if special cases arise

    statements<<MakeCreateStatement(BaseId(tMenutype),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tPlatType : generic dish layout for every generic menu, can also be modified
    fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
         fieldList<<BaseId(idMenuType); typeList<<("INTEGER");
         fieldList<<BaseId(idPlatType); typeList<<("INTEGER");
      fieldList<<BaseId(nomPlatType); typeList<<("TEXT");
      fieldList<<BaseId(nombrePersonnes);typeList<<("INTEGER");

    statements<<MakeCreateStatement(BaseId(tPlattype),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tLieux : keeps track of places which will be visited, type of mooring, type of shops (for full shopping, or emergeny shopping only)
     fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
            fieldList<<BaseId(date); typeList<<("INTEGER");
            fieldList<<BaseId(lieu); typeList<<("TEXT");
          fieldList<<BaseId(amarre); typeList<<("TEXT");
         fieldList<<BaseId(contact); typeList<<("TEXT");
       fieldList<<BaseId(depannage); typeList<<("BOOLEAN");
        fieldList<<BaseId(commerce); typeList<<("BOOLEAN");

    statements<<MakeCreateStatement(BaseId(tLieux),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tPlanning : general layout of menus per day, pointer to those menus (idMenu)
     fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
      fieldList<<BaseId(idPlanning); typeList<<("INTEGER");
       fieldList<<BaseId(dateRepas); typeList<<("INTEGER");
      fieldList<<BaseId(idMenuType); typeList<<("INTEGER");
          fieldList<<BaseId(idMenu); typeList<<("INTEGER");
        fieldList<<BaseId(numRepas); typeList<<("INTEGER"); //generally follows menutype but additionnal menus can be added, thus these numbers will be reordered for the day
     fieldList<<BaseId(momentRepas); typeList<<("TEXT"); //only used when a punctual menu is added to the day
 fieldList<<BaseId(nombrePersonnes); typeList<<("INTEGER");

    statements<<MakeCreateStatement(BaseId(tPlanning),fieldList,typeList);
    fieldList.clear(); typeList.clear();

    statements<<"CREATE INDEX menus ON tPlanning (idCroisiere,dateRepas)";
    statements<<"CREATE INDEX compos ON tPlanning (idCroisiere,idPlanning)";

//tMenu : header of the menu with its name and possibility for re use (favori)
          fieldList<<BaseId(idMenu); typeList<<("INTEGER PRIMARY KEY");
         fieldList<<BaseId(nomMenu); typeList<<("TEXT");
          fieldList<<BaseId(favori); typeList<<("BOOLEAN"); //=1 si le menu sera réutilisé souvent

    statements<<MakeCreateStatement(BaseId(tMenu),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tComposition : layout of the menu, chronologically ordered list of dishes
          fieldList<<BaseId(idMenu); typeList<<("INTEGER");
          fieldList<<BaseId(numero); typeList<<("INTEGER");// indique l'ordre du plat, servira au tri des requetes
        fieldList<<BaseId(position); typeList<<("TEXT"); //définit le nom de la position (entrée, plat, dessert...)
          fieldList<<BaseId(idPlat); typeList<<("INTEGER");

    statements<<MakeCreateStatement(BaseId(tComposition),fieldList,typeList);
    fieldList.clear(); typeList.clear();

    statements<<"CREATE INDEX composite ON tComposition (idMenu)";

//tIComposition : instanciation of a menu, with type of dishes and number of persons
     fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
      fieldList<<BaseId(idPlanning); typeList<<("INTEGER");
      fieldList<<BaseId(idPlatType); typeList<<("INTEGER");
          fieldList<<BaseId(idMenu); typeList<<("INTEGER");
          fieldList<<BaseId(numero); typeList<<("INTEGER");
        fieldList<<BaseId(position); typeList<<("TEXT"); //définit le nom de la position (entrée, plat, dessert...)
          fieldList<<BaseId(idPlat); typeList<<("INTEGER");
 fieldList<<BaseId(nombrePersonnes); typeList<<("INTEGER");

statements<<MakeCreateStatement(BaseId(tIComposition),fieldList,typeList);
fieldList.clear(); typeList.clear();

statements<<"CREATE INDEX composite ON tIComposition (idCroisiere,idPlanning,idPlatType,idMenu)";

//tPlat : definition of a dish, name, descriptive flags, time to prepare, time to cook, receipt, pointers to ingredients, intended number of persons
          fieldList<<BaseId(idPlat); typeList<<("INTEGER");
       fieldList<<BaseId(titrePlat); typeList<<("TEXT");
   fieldList<<BaseId(categoriePlat); typeList<<("TEXT"); //Entrée, Plate, Dessert...
         fieldList<<BaseId(recette); typeList<<("TEXT");
          fieldList<<BaseId(auFour); typeList<<("BOOLEAN");
         fieldList<<BaseId(auFrigo); typeList<<("BOOLEAN");
fieldList<<BaseId(tempsPreparation); typeList<<("NUMERIC");
    fieldList<<BaseId(tempsCuisson); typeList<<("NUMERIC");
     fieldList<<BaseId(nbPersonnes); typeList<<("INTEGER");
        fieldList<<BaseId(flagsNav); typeList<<("INTEGER");//Flag system
       fieldList<<BaseId(flagsDiet); typeList<<("INTEGER");//Flag system
    fieldList<<BaseId(idPlatInclus); typeList<<("INTEGER");//Point to another dish if necessary otherwise 0
     fieldList<<BaseId(recetteBase); typeList<<("BOOLEAN");//AS BOOL
   fieldList<<BaseId(fractionnable); typeList<<("BOOLEAN");

    statements<<MakeCreateStatement(BaseId(tPlat),fieldList,typeList);
    fieldList.clear(); typeList.clear();

    statements<<"CREATE INDEX uFour ON tPlat (auFour)";
    statements<<"CREATE INDEX uFrigo ON tPlat (auFrigo)";
    statements<<"CREATE INDEX fNav ON tPlat (flagsNav)";
    statements<<"CREATE INDEX fDiet ON tPlat(flagsDiet)";
    statements<<"CREATE INDEX plats ON tPlat (categoriePlat)";
    statements<<"CREATE INDEX twoflags ON tPlat (categoriePlat,flagsNav)";

//tListeIngredients : list of ingredients for a dish
            fieldList<<BaseId(idPlat); typeList<<("INTEGER");
      fieldList<<BaseId(idIngredient); typeList<<("INTEGER");
   fieldList<<BaseId(conditionnement); typeList<<("TEXT");
          fieldList<<BaseId(quantite); typeList<<("NUMERIC");
             fieldList<<BaseId(unite); typeList<<("TEXT");

    statements<<MakeCreateStatement(BaseId(tListeIngredients),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tListeUstensiles : for a dish lists all ustensils
            fieldList<<BaseId(idPlat); typeList<<("INTEGER");
       fieldList<<BaseId(idUstensile); typeList<<("INTEGER");

    statements<<MakeCreateStatement(BaseId(tListeUstensiles),fieldList,typeList);
        fieldList.clear(); typeList.clear();

        statements<<"CREATE INDEX ust ON tListeUstensiles(idPlat)";

//tIngredients : list of all ingredients necessary for the preparation of the dishes in the database
          fieldList<<BaseId(idIngredient); typeList<<("INTEGER PRIMARY KEY");
         fieldList<<BaseId(nomIngredient); typeList<<("TEXT");
                 fieldList<<BaseId(frais); typeList<<("BOOLEAN");
       fieldList<<BaseId(conditionnement); typeList<<("TEXT");
     fieldList<<BaseId(conservationFrais); typeList<<("BOOLEAN");
          fieldList<<BaseId(quantiteBase); typeList<<("NUMERIC");
                 fieldList<<BaseId(unite); typeList<<("TEXT");
         fieldList<<BaseId(fractionnable); typeList<<("BOOLEAN");
                fieldList<<BaseId(limInf); typeList<<("NUMERIC");
               fieldList<<BaseId(idRayon); typeList<<("INTEGER");

        statements<<MakeCreateStatement(BaseId(tIngredients),fieldList,typeList);
        fieldList.clear(); typeList.clear();

       statements<<"CREATE INDEX ingr ON tListeIngredients (idPlat)";

// tRayon : list of all shelves
       fieldList<<BaseId(idRayon); typeList<<("INTEGER PRIMARY KEY");
      fieldList<<BaseId(nomRayon); typeList<<("TEXT");

      statements<<MakeCreateStatement(BaseId(tRayon),fieldList,typeList);
      fieldList.clear(); typeList.clear();

//tQuantites : here are stored the quantities finally necessary for this cruise and the tagged dish
           fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
            fieldList<<BaseId(idPlanning); typeList<<("INTEGER");
                fieldList<<BaseId(idPlat); typeList<<("INTEGER");
          fieldList<<BaseId(idIngredient); typeList<<("INTEGER");
              fieldList<<BaseId(quantite); typeList<<("NUMERIC");
                 fieldList<<BaseId(unite); typeList<<("TEXT");
               fieldList<<BaseId(prepese); typeList<<("BOOLEAN");

        statements<<MakeCreateStatement(BaseId(tQuantites),fieldList,typeList);
        fieldList.clear(); typeList.clear();

        statements<<"CREATE INDEX quants ON tQuantites(idCroisiere,idPlanning,idPlat)";

//tUstensiles
           fieldList<<BaseId(idUstensile); typeList<<("INTEGER PRIMARY KEY");
          fieldList<<BaseId(nomUstensile); typeList<<("TEXT");

        statements<<MakeCreateStatement(BaseId(tUstensiles),fieldList,typeList);
        fieldList.clear(); typeList.clear();

//tInventaire
           fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
          fieldList<<BaseId(idIngredient); typeList<<("INTEGER");
       fieldList<<BaseId(lieuDeRangement); typeList<<("TEXT");
       fieldList<<BaseId(conditionnement); typeList<<("TEXT");
              fieldList<<BaseId(quantite); typeList<<("NUMERIC");
            fieldList<<BaseId(dateEntree); typeList<<("INTEGER");
            fieldList<<BaseId(dateSortie); typeList<<("INTEGER");
     fieldList<<BaseId(dateReutilisation); typeList<<("INTEGER");
        fieldList<<BaseId(datePeremption); typeList<<("INTEGER");
            fieldList<<BaseId(aCommander); typeList<<("BOOLEAN");

      statements<<MakeCreateStatement(BaseId(tInventaire),fieldList,typeList);
        fieldList.clear(); typeList.clear();

      statements<<"CREATE INDEX inv ON tInventaire(idCroisiere,idIngredient)";
      statements<<"CREATE INDEX comm ON tInventaire(idCroisiere,aCommander)";

//tListeCourse
           fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
         fieldList<<BaseId(idListeCourse); typeList<<("INTEGER");
          fieldList<<BaseId(dateEcriture); typeList<<("INTEGER");
           fieldList<<BaseId(dateCourses); typeList<<("INTEGER");
                 fieldList<<BaseId(duree); typeList<<("INTEGER");//duree à partir de la date où la liste a été établie
            fieldList<<BaseId(typeAchats); typeList<<("TEXT");
                 fieldList<<BaseId(faite); typeList<<("BOOLEAN");

      statements<<MakeCreateStatement(BaseId(tListeCourse),fieldList,typeList);
        fieldList.clear(); typeList.clear();

      statements<<"CREATE INDEX crses ON tListeCourse(idCroisiere,dateCourses)";
      statements<<"CREATE INDEX lcourse ON tListeCourse(idCroisiere,idListeCourse)";

//tItemsListeCourse
           fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
         fieldList<<BaseId(idListeCourse); typeList<<("INTEGER");
          fieldList<<BaseId(idIngredient); typeList<<("INTEGER");
               fieldList<<BaseId(idRayon); typeList<<("INTEGER");
              fieldList<<BaseId(quantite); typeList<<("NUMERIC");
                fieldList<<BaseId(trouve); typeList<<("BOOLEAN");
          fieldList<<BaseId(platsOrigine); typeList<<("TEXT");

     statements<<MakeCreateStatement(BaseId(tItemsListeCourse),fieldList,typeList);
        fieldList.clear(); typeList.clear();

//tConversions
           fieldList<<BaseId(idParametre); typeList<<("INTEGER PRIMARY KEY");
          fieldList<<BaseId(abbreviation); typeList<<("TEXT");
            fieldList<<BaseId(nomComplet); typeList<<("TEXT");
         fieldList<<BaseId(enMillilitres); typeList<<("REAL");
        fieldList<<BaseId(enMilligrammes); typeList<<("REAL");

     statements<<MakeCreateStatement(BaseId(tConversions),fieldList,typeList);
        fieldList.clear(); typeList.clear();

//tEquipets
               fieldList<<BaseId(idBateau); typeList<<("INTEGER");
             fieldList<<BaseId(nomEquipet); typeList<<("TEXT");
              fieldList<<BaseId(idEquipet); typeList<<("INTEGER");
                fieldList<<BaseId(idPhoto); typeList<<("INTEGER");
                 fieldList<<BaseId(pointX); typeList<<("INTEGER");
                 fieldList<<BaseId(pointY); typeList<<("INTEGER");
                 fieldList<<BaseId(labelX); typeList<<("INTEGER");
                 fieldList<<BaseId(labelY); typeList<<("INTEGER");

    statements<<MakeCreateStatement(BaseId(tEquipets),fieldList,typeList);
    fieldList.clear(); typeList.clear();

    statements<<"CREATE INDEX equipets ON tEquipets(idBateau,idEquipet)";

//tBateau
           fieldList<<BaseId(idBateau); typeList<<("INTEGER PRIMARY KEY");
         fieldList<<BaseId(typeBateau); typeList<<("TEXT");

    statements<<MakeCreateStatement(BaseId(tBateau),fieldList,typeList);
    fieldList.clear(); typeList.clear();

//tPhotos
           fieldList<<BaseId(idBateau); typeList<<("INTEGER");
            fieldList<<BaseId(idPhoto); typeList<<("INTEGER");
       fieldList<<BaseId(fichierPhoto); typeList<<("TEXT");

    statements<<MakeCreateStatement(BaseId(tPhotos),fieldList,typeList);
    fieldList.clear(); typeList.clear();

    statements<<"CREATE INDEX photos ON tPhotos(idBateau,idPhoto)";

//tLocUstensiles
        fieldList<<BaseId(idCroisiere); typeList<<("INTEGER");
        fieldList<<BaseId(idUstensile); typeList<<("INTEGER");
          fieldList<<BaseId(idEquipet); typeList<<("TEXT");

     statements<<MakeCreateStatement(BaseId(tLocUstensiles),fieldList,typeList);

     fieldList.clear(); typeList.clear();

     statements<<"CREATE INDEX outils ON tLocUstensiles(idCroisiere,idUstensile,idEquipet)";

//tPavillons
        fieldList<<BaseId(idPavillon);typeList<<("INTEGER PRIMARY KEY");
        fieldList<<BaseId(categoriePavillon);typeList<<("INTEGER");
        fieldList<<BaseId(bitpos);typeList<<("INTEGER");
        fieldList<<BaseId(nomPavillon);typeList<<("INTEGER");

        statements<<MakeCreateStatement(BaseId(tpavillons),fieldList,typeList);
        fieldList.clear(); typeList.clear();
        statements<<"CREATE INDEX Pavillons ON tPavillons(categoriePavillon)";

//Fast creation of the tables
    QSqlQuery mkTables(db);
    mkTables.exec("PRAGMA synchronous=0");
    for (auto &creation:statements){
        mkTables.exec(creation);
        qDebug()<<"Checkbase/Création statement : "<<mkTables.lastQuery();
        qDebug()<<"error if applies : "<<mkTables.lastError().databaseText();
    }
    mkTables.exec("PRAGMA synchronous=1");

    QFile dL(datawizard::nomStructure);
    dL.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream textW(&dL);
    for (auto &ln:structureList) textW<<ln;
    dL.close();

}

QString CheckBase::MakeCreateStatement(QString inTableName, QStringList inFieldList, QStringList inTypeList){
    QString newStatemt("");
    newStatemt=QString("CREATE TABLE %1 (").arg(inTableName);
    structureList<<inTableName<<("\n");
    int sz=inFieldList.size();
    for (int idx(0);idx<sz;idx++){
        newStatemt=newStatemt+inFieldList[idx]+" "+inTypeList[idx];
        if (idx<(sz-1)) newStatemt=newStatemt+",";
        structureList<<"   "<<inFieldList[idx]<<" :  "<<inTypeList[idx]<<"\n";
    }
    newStatemt=newStatemt+")";
    return newStatemt;
}

QString CheckBase::MakeIndexStatement(QString indexName, QString inTableName, QStringList inFieldNames){
    QString idxSt("");
    idxSt="CREATE INDEX "+indexName+" ON "+inTableName+"(";
    int sz=inFieldNames.size();
    for (int idx(0);idx<sz;idx++){
        idxSt=idxSt+inFieldNames[idx];
        if (idx<(sz-1)) idxSt=idxSt+",";
    }
    return idxSt;
}

bool CheckBase::isNewBase(){
    QStringList tables=db.tables();
    if (!tables.count()){
        newBase=true;
    }
    return newBase;

}

void CheckBase::GetScheduleInfo(){

}
