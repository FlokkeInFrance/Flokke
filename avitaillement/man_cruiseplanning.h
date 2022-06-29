#ifndef MAN_CRUISEPLANNING_H
#define MAN_CRUISEPLANNING_H

#include "basemanager.h"
#include "QSqlTableModel"
#include "QStringListModel"


class man_cruiseplanning : public basemanager
{
public:
    man_cruiseplanning();
   ~man_cruiseplanning();
    void CommitNewCruise();
sCroisiere CreateCruise();
sCroisiere GetSelectedCruise();
sCroisiere SelectCruiseFromList(int pos);
    bool LoadCruise(int idC);
    void EditACruise (sCroisiere inCruise);
    void DeleteActiveCruise();
    void DeleteCruiseId(int inId);
    void ChangeNumberPersons(int nPers);
QStringList CruisesToShow(int ordered=0);

    sCroisiere ChangeCruiseTiming(QDate departure, QDate arrival, int duration);
    void DeleteAfter(QDate sErase); //Maintains beginCruise, endCruise, daysInPlan, tLieux,tPlanning, tMenu, tCroisière
    void ShiftCruiseNDays(int nDays);   //Maintains beginCruise, endCruise, tLieux,tPlanning, tMenu, tCroisière
    void FinishCruise();                //Maintains tCroisiere perhaps should Shorten Remaining Days,or perhaps only if no menus are set ?

    void LoadOrCreateBoatType( const QString &bType);
    void ChangeBoatName( const QString &nName);
    void ChangeBoatType( const QString &nType);

    QVector<sMenutype> GetMenuTypeForCruise();

    void BuildNewPlanType();            //Define the general menu layout for the cruise, by default the first 3 items from menu list : breakfast, lunch, dinner, 3 first dishes for the 2 laste meals
    void AddNewMenuToType(QString inType, int inPos=0);            //For the menu types, add a menu (which will be added to the remaining days in the active cruise
    void AddNewDishToMenuType(int pos);        //Adds a Dish to a menu in general (recorded in the type), adds the dish to every corresponding menu in the remaining menu if it exists
    void ModifyMenuNameInType(QString inNName);        //Modifies the name in type and will also modify the name in the plan (maintains tPlanning)
    void ModifyDishNameInType(int pos, QString inDish);
    void SelectMenuType(int mT);


    void GenerateNewMenuPlan(QDate d1,QDate d2);         //For every day in the cruise, set the menu layout according to the general meny layout
    void GenerateNewCruisePlan(QDate d1, QDate d2);       //For every day in the cruise, set the lockers for the cruise information : location, mooring type, possibility to shop
    void AddMenuToDay(int pos); //At Active date, insert menu at pos'th position 0 means at the end, 1 shift all the rest
    void ClearCurrentPlanningSlot();

    //Modification to Planning
    int  MakeNewMenuInstance(int idPlan,int idMenu);//instanciate a given Menu at planning place idPlan
    void DeleteActiveMenu();
    void InsertMenuInDay(int inLoc,int inMenu);
    void GenerateQuantities(int inDish, int nPlan, int nbPers);
    void AdaptQuantitiesToNbOfPersons(int inPlanning, int inPlat, int nbPformeal, int nbPersonsfordish);
    void SetCurrentDate(QDate cDate);//Selects the line but also the Menu in ActiveMenu
    void SelectPlanningItem(int pNum);
    void InsertDishIntoPlannedMenu(int dishNb,int posInCompo,int idPlanning=0);//in pos is empty, add the dish, if not make a place for it, if idPlann=0 use current aPlanning

    //Modification to Menu
    int  MakeNewMenu(int idMenuT);//Makes a new menu with placeholders according to menu type idMenuT
    int  MakeNewMenu(QString mName);
    void DeleteMenu(int inMenu=0);  //if inMenu=0, delete aMenu, initialise aMenu,erase quantities, prépesés which refer to it, erase IComposition which refer to it, idPlanning to 0
    void InsertDishIntoMenu(int pos, int nMenu=0, int nDish=0);//if nMenu=0 use current aMenu, if nDish=0 use aPlat
    void ReplaceDishInMenu(int nDish);//will delete the menu if it has no dishes
    void RemoveDishFromMenu(int nDish);
    void SelectMenu(int menuNb);
    void SelectDish(int dishNb);
    void ReadDish(int numb);
    void InsertIngrToDish(int inIngr, int inPos);
    int  MakeNewIngr(QString inIngr);

    //int activeCruise,activeMenuType,activeDishType,activePlanningLine,activeMenu,activeDish,activeInventory,activeIngredient,activeTool,activeShopList;
    //int activeLocker,idActiveBoat;
    QDate activeDate;
    //sCroisiere aCruise;
    sPlanning aPlanningItem;

private:
    sMenutype aMenuType;
    sPlanning aPlanning;
    sComposition aComposition;
    sPlat aPlat;
    sQuantites aQuantites;
    sMenu aMenu;
    sIComposition aIComposition;
    QSqlTableModel allCruises;
    QStringList cruisesToShow;
    bool cruiseCommited;
};

#endif // MAN_CRUISEPLANNING_H
