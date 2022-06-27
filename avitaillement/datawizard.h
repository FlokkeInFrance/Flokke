 /*
 * This class, a collection of static functions, transforms data, mainly date, integer, numbers to text and conversely.
 * Can convert volumes and weights, knows about units of measurement and can make equivalents
 * knows about fractions and how they're written
 *
 * need to initialize date separator, date format and decimal separator.
 */
#ifndef DATAWIZARD_H
#define DATAWIZARD_H

#include <QStringList>
#include <QDate>
#include <QVariant>
#include <QVector>
#include <QDebug>

using intvect=QVector<int>;
using stringvect=QVector<QString>;
using floatvect=QVector<QString>;

enum dataNames{
    tCroisiere,//0
    idCroisiere,
    nomBateau,
    typeBateau,
    intitule,
    dateDepart,
    dateFin,
    nombreJours,
    nombrePersonnes,
    terminee,
    tMenutype,//10
        idMenu,
    titreMenu,
    tPlattype,
        idPlat,
    titrePlat,
    tPlanning ,
         idPlanning,
    dateRepas,
    momentRepas,
    tLieux,
        date,
    lieu,
    amarre,
    depannage,
    commerce,
    tMenu,
         nomMenu,
    favori,
    tComposition,
          numero,
    position,
    tPlat,
         categoriePlat,
    recette,
    auFour,
    auFrigo,
    tempsPreparation,
    tempsCuisson,
    nbPersonnes,
    flagsDiet,
    flagsNav,
    recetteBase,
    tListePrepeses,
    idListPP,
    idIngredient,
    idQuantite,
    tListeIngredients,
    quantite,
    unite,
    conditionnement,
    tListeUstensiles,
    idUstensile,
    tIngredients,
    nomIngredient,
    frais,
    quantiteBase,
    fractionnable,
    limInf,
    idRayon,
    tQuantites,
    tUstensiles ,
    nomUstensile,
    lieuDeRangement,
    tInventaire,
    dateEntree,
    dateSortie,
    dateReutilisation,
    datePeremption,
    aCommander,
    tListeCourse,
    idListeCourse,
    dateEcriture,
    dateCourses,
    nomListe,
    faite,
    tItemsListeCourse,
    platsOrigine,
    trouve,
    tConversions,
    idParametre,
    abbreviation,
    nomComplet,
    enMillilitres,
    enMilligrammes,
    tEquipets,
    idEquipet,
    nomEquipet,
    idBateau,
    idPhoto,
    pointX,
    pointY,
    labelX,
    labelY,
    tBateau,
    tPhotos,
    fichierPhoto,
    duree,
    typeAchats,
    attribue,
    numRepas,
    idMenuType,
    idPlatType,
    nomMenuType,
    nomPlatType,
    conservationFrais,
    tRayon,
    nomRayon,
    tLocUstensiles,
    tpavillons,
    idPavillon,//110
    categoriePavillon,
    bitpos,
    nomPavillon,
    idPlatInclus,
    tIComposition,
    contact,
    prepese
};


 struct sCroisiere{
   int idCroisiere;
   QString nomBateau;
   QString typeBateau;
   QString intitule;
   QDate dateDepart;
   QDate dateFin;
   int nombreJours;
   int nombrePersonnes;
   bool terminee;
   };
 struct sMenutype{
   int idCroisiere;
   int idMenuType;
   QString nomMenuType;
   int nombrePersonnes;
   };
 struct sPlattype{
   int idCroisiere;
   int idMenuType;
   int idPlatType;
   QString nomPlatType;
   int nombrePersonnes;
   };
struct sLieux{
   int idCroisiere;
   QDate date;
   QString lieu;
   QString amarre;
   QString contact;
   bool depannage;
   bool commerce;
   };
struct sPlanning{
   int idCroisiere;
   int idPlanning;
   QDate dateRepas;
   int idMenuType;
   int idMenu;
   int numRepas;
   QString momentRepas;
   int nombrePersonnes;
   };
struct sMenu{
   int idMenu;
   QString nomMenu;
   bool favori;
};
struct sComposition{
   int idMenu;
   int numero;
   QString position;
   int idPlat;
};
struct sIComposition{
         int idCroisiere;
         int idPlanning;
         int idPlatType;
         int idMenu;
         int numero;
     QString position;
         int idPlat;
         int nombrePersonnes;
};

struct sPlat{
   int idPlat;
   QString titrePlat;
   QString categoriePlat;
   QString recette;
   bool auFour;
   bool auFrigo;
   int tempsPreparation;
   int tempsCuisson;
   int nbPersonnes;
   int flagsNav;
   int flagsDiet;
   int idPlatInclus;
   bool recetteBase;
   bool fractionnable;
};
struct sListePrepeses{
   int idCroisiere;
   int idPlanning;
   int idListPP;
   int idPlat;
   int idIngredient;
   float quantite;
   QString unite;
};
struct sListeIngredients{
   int idPlat;
   int idIngredient;
   QString conditionnement;
   float quantite;
   QString unite;
};
struct sListeUstensiles{
   int idPlat;
   int idUstensile;
};
struct sIngredients{
   int idIngredient;
   QString nomIngredient;
   bool frais;
   QString conditionnement;
   bool conservationFrais;
   float quantiteBase;
   QString unite;
   bool fractionnable;
   float limInf;
   int idRayon;
};
struct sRayon{
   int idRayon;
   QString nomRayon;
};
struct sQuantites{
   int idCroisiere;
   int idPlanning;
   int idPlat;
   int idIngredient;
   float quantite;
   QString unite;
   bool prepese;
};
struct sUstensiles{
   int idUstensile;
   QString nomUstensile;
};

struct sInventaire{
   int idCroisiere;
   int idIngredient;
   QString lieuDeRangement;
   QString conditionnement;
   float quantite;
   QDate dateEntree;
   QDate dateSortie;
   QDate dateReutilisation;
   QDate datePeremption;
   bool aCommander;
};
struct sListeCourse{
   int idCroisiere;
   int idListeCourse;
   QDate dateEcriture;
   QDate dateCourses;
   int duree;
   QString typeAchats;
   bool faite;
};
struct sItemsListeCourse{
   int idCroisiere;
   int idListeCourse;
   int idIngredient;
   int idRayon;
   float quantite;
   bool trouve;
   QString platsOrigine;
};
struct sConversions{
   int idParametre;
   QString abbreviation;
   QString nomComplet;
   float enMillilitres;
   float enMilligrammes;
};
struct sEquipets{
   int idBateau;
   QString nomEquipet;
   int idEquipet;
   int idPhoto;
   int pointX;
   int pointY;
   int labelX;
   int labelY;
};
struct sBateau{
   int idBateau;
   QString typeBateau;
};
struct sPhotos{
    int idBateau;
    int idPhoto;
QString fichierPhoto;
};

struct sLocUstensiles{
    int idCroisiere;
    int idUstensile;
    int idEquipet;
};

struct sPavillons{
    int idPavillon;
QString categoriePavillon;
    int bitpos;
QString nomPavillon;

};

class datawizard

{
public:
    datawizard();

    static void    SetFormats(QString dateSep,QString dateF,QString decSep);
    static void    SetLanguage(QString lg);
    static void    GetConversions(QVector<sConversions> &convs);
    static void    SetConversion(sConversions cv);
    static void    CorrectConversion(const sConversions &ov,const sConversions &nv);
    static float   AdjustForNbPersons(const QString &unite, float quantite, int intededNbPersons, int &actualNbPersons);
    static QString RightSplit(QString toSplit, QString splitter);
    static QString RightSplit(QString toSplit, QString splitter, QString &leftString);
    static QString ToSqlIntegerString(int inInt,bool withComma=false);
    static QString ToSqlString(QString inString,bool withComma=false);
    static QString ToSqlNumeric(float inNum,bool withComma=false);
    static QString ToSqlBool (bool inBool,bool withComma=false);
    static QString ToSqlJulian(QDate inDate,bool withComma=false);
    static QDate   FromSqlJulian(QVariant inDate);
    static QString PrintSqlJulian(QVariant inDate);
    static void    MakeDbIdentifiers();
    static QString DbId(int inId);



    static QString dateSeparator,dateFormat,decimalSep;
    static QString nomDb,nomLinguist,nomParams,nomStructure,nomDataDump;
    static stringvect dbIdentifiers;
    static intvect milliliters,milligrams;
    static intvect idconv;
    static stringvect abbr,names;
};

#endif // DATAWIZARD_H
