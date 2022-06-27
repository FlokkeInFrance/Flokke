#include "datawizard.h"
QString datawizard::decimalSep=",";
QString datawizard::dateFormat="jma";
QString datawizard::dateSeparator="/";
QString datawizard::nomDb="/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/BDAvitaillement.sqlite";
QString datawizard::nomLinguist="/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/francais.txt";
QString datawizard::nomParams="/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/params.txt";
QString datawizard::nomStructure="/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/dbStructure.txt";
QString datawizard::nomDataDump="/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/dbContent.txt";
intvect datawizard::milligrams;
intvect datawizard::milliliters;
intvect datawizard::idconv;
stringvect datawizard::abbr;
stringvect datawizard::names;


stringvect datawizard::dbIdentifiers({
 "tCroisiere", //0
 "idCroisiere",
 "nomBateau",
 "typeBateau",
 "intitule",
 "dateDepart",
 "dateFin",
 "nombreJours",
 "nombrePersonnes",
 "terminee",
 "tMenutype",//10
 "idMenu",
 "titreMenu",
 "tPlattype",
 "idPlat",
 "titrePlat",
 "tPlanning ",
 "idPlanning",
 "dateRepas",
 "momentRepas",
 "tLieux",//20
 "date",
 "lieu",
 "amarre",
 "depannage",
 "commerce",
 "tMenu",
 "nomMenu",
 "favori",
 "tComposition",
 "numero",
 "position",
 "tPlat",
 "categoriePlat",
 "recette",
 "auFour",
 "auFrigo",
 "tempsPreparation",
 "tempsCuisson",
 "nbPersonnes",
 "flagsDiet",
 "flagsNav",
 "recetteBase",
 "tListePrepeses",
 "idListPP",
 "idIngredient",
 "idQuantite",
 "tListeIngredients",
 "quantite",
 "unite",
 "conditionnement",
 "tListeUstensiles",
 "idUstensile",
 "tIngredients",
 "nomIngredient",
 "frais",
 "quantiteBase",
 "fractionnable",
 "limInf",
 "idRayon",
 "tQuantites",
 "tUstensiles",
 "nomUstensile",
 "lieuDeRangement",
 "tInventaire",
 "dateEntree", //initiate the debugger (with basemanager)
 "dateSortie",
 "dateReutilisation",
 "datePeremption",
 "aCommander",
 "tListeCourse",
 "idListeCourse",
 "dateEcriture",
 "dateCourses",
 "nomListe",
 "faite",
 "tItemsListeCourse",
 "platsOrigine",
 "trouve",
 "tConversions",
 "idParametre",
 "abbreviation",
 "nomComplet",
 "enMillilitres",
 "enMilligrammes",
 "tEquipets",
 "idEquipet",
 "nomEquipet",
 "idBateau",
 "idPhoto",
 "pointX",
 "pointY",
 "labelX",
 "labelY",
 "tBateau",
 "tPhotos",
 "fichierPhoto",
 "duree",
 "typeAchats",
 "attribue",
 "numRepas",
 "idMenuType",
 "idPlatType",
 "nomMenuType",
 "nomPlatType",
 "conservationFrais",
 "tRayon",
 "nomRayon",
 "tLocUstensiles",
 "tpavillons",
 "idPavillon",//110
 "categoriePavillon",
 "bitpos",
 "nomPavillon",
 "idPlatInclus",
 "tIComposition",
 "contact",
 "prepese"
});


datawizard::datawizard()
{

}

QString datawizard::DbId(int inId){
return dbIdentifiers[inId];
}

void datawizard::SetFormats(QString dateSep, QString dateF, QString decSep){
    decimalSep=decSep;
    dateFormat=dateF;
    dateSeparator=dateSep;
}

void datawizard::GetConversions( QVector<sConversions> &convs){
    for (sConversions &cv:convs){
        idconv<<cv.idParametre;
        milligrams<<cv.enMilligrammes;
        milliliters<<cv.enMillilitres;
        abbr<<cv.abbreviation;
        names<<cv.nomComplet;
    }
}

void datawizard::SetConversion(sConversions cv){
    milligrams<<cv.enMilligrammes;
    milliliters<<cv.enMillilitres;
    abbr<<cv.abbreviation;
    names<<cv.nomComplet;
}

void datawizard::CorrectConversion(const sConversions &ov,const sConversions &nv){
    //find index
    int pos=idconv.indexOf(ov.idParametre);
    //substitute values
    if (pos>-1){
        milligrams[pos]=nv.enMilligrammes;
        milliliters[pos]=nv.enMillilitres;
        abbr[pos]=nv.abbreviation;
        names[pos]=nv.nomComplet;
    }
}

float     datawizard::AdjustForNbPersons(const QString &unite, float quantite, int intededNbPersons, int &actualNbPersons){
    float retconv(0);
    if (unite==""){
        retconv=quantite*float(actualNbPersons)/float(intededNbPersons);
    }
    int idx=abbr.indexOf(unite);
    if (idx==-1) idx=names.indexOf(unite);
    if (idx==-1) return retconv;
    retconv=quantite*float(actualNbPersons)/float(intededNbPersons);
    return retconv;
}

void datawizard::SetLanguage(QString lg){
    nomLinguist=QString("/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/%1.txt").arg(lg);
}

QString datawizard::RightSplit(QString toSplit, QString splitter){
    QString workS=toSplit.trimmed();
    return workS.section(splitter,1);
}

QString datawizard::RightSplit(QString toSplit, QString splitter, QString &leftString){
    QString workS=toSplit.trimmed();
leftString=workS.section(splitter,0,0);
return workS.section(splitter,1);
}

QString datawizard::ToSqlIntegerString(int inInt,bool withComma){
    QString toret(QString::number(inInt));
    if (withComma) toret=toret+",";
    return toret;
}

QString datawizard::ToSqlString(QString inString,bool withComma){
    QString toO=inString;
    toO.replace("'","''");
    toO="'"+toO+"'";
    if (withComma) toO=toO+",";
    return toO;
}

QString datawizard::ToSqlJulian(QDate inDate, bool withComma){
    QString toO=QString::number(inDate.toJulianDay());
    if (withComma) toO=toO+",";
    return toO;
}

QDate datawizard::FromSqlJulian(QVariant inDate){
    qint64 jd(inDate.toInt());
    QDate toret(QDate::fromJulianDay(jd));
    return toret;
}

QString datawizard::PrintSqlJulian(QVariant inDate){
    return(inDate.toDate().toString(dateFormat));
}

QString datawizard::ToSqlNumeric(float inNum,bool withComma){
    QString toret(QString::number(inNum,'f',3));
    if (withComma) toret=toret+",";
    return toret;
}

QString datawizard::ToSqlBool (bool inBool,bool withComma){
    QString toret(QString::number(inBool));
    if (withComma) toret=toret+",";
    return toret;
}
