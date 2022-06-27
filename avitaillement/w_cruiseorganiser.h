#ifndef W_CRUISEORGANISER_H
#define W_CRUISEORGANISER_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>
#include <QTreeWidget>
#include "w_europeandateeditor.h"
#include "centreop.h"
#include "linguist.h"
#include "w_maincruisewindows.h"
#include "datawizard.h"

class w_MainCruiseWindows;

class W_CruiseOrganiser : public QWidget
{
    Q_OBJECT
public:
    W_CruiseOrganiser(QWidget *parent = 0);

    QString wT(int toTranslate);
    void    setCenterOp(CentreOp *ctr);
    void    setupWidget();
    QString getWord(int inCode);
    QString getWordSC(int inCode);
    void    resetViews();
    void    getCruiseList();
    void    getMenuTypes();
    void    showActiveCruise();
    void    ObjectModel();
private:
    //Layouts
        QHBoxLayout *mainLayout, *buttonEditZone,*buttonListZone;
        QVBoxLayout *detailZone,*listZone,*typeMenuZone;
        QFormLayout *editZone;

    //Widgets
      QLabel               *title,*headerOfEdit, *headerOfList;
      QLineEdit            *cruiseName, *boatName, *boatType;
      W_EuropeanDateEditor *startCruise, *endCruise;
      QCheckBox            *cruiseFinished;
      QSpinBox             *nbOfSailors,*nbDays;
      QListWidget          *listCruises;
      QTreeWidget          *menutypes;
      QPushButton          *bNew, *bDelete,*bPlan,*bCancel;
      QPushButton          *bFinished,*bBNotFinished,*bAll;



    //variables
      CentreOp            *center;
      man_cruiseplanning  *cruiseHandler;
      linguist            *internat;
      sCroisiere          aCruise;
      bool                toRefresh;

public slots:

     void UpdateCruise();
     void UpdateDates();
     void UpdateCrew();
     void UpdateBoatType();
     void NewCruise();
     void CommitAndPlan();
     void DeleteCruise();
     void ListClicked(int el);

};
#endif // W_CRUISEORGANISER_H
