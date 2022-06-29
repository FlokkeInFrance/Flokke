/*this is going to hold actions, and connects. Menus for general cruise, menu, receipt generator, boat layouts, and inventory management
 */

#ifndef W_MAINCRUISEWINDOWS_H
#define W_MAINCRUISEWINDOWS_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include <QLabel>
#include <QVBoxLayout>
#include "centreop.h"


class W_CruiseOrganiser;

class w_MainCruiseWindows : public QMainWindow
{
    Q_OBJECT
public:
    explicit w_MainCruiseWindows(QWidget *parent = nullptr);

    void BuildActions();
    void SetOperations(CentreOp *inOp);
    QString wT(int);

    W_CruiseOrganiser *cruises;
    debugger *debugs;
    QAction *a_quit,*a_newCruise,*a_deleteCruise,*a_copyMenus,*a_pasteMenus,*a_exportMenus;
    QAction *a_sortByName,*a_onlyNotFinished,*a_sortByStatus,*a_sortByStart,*a_sortByEnd,*a_parameters;
    QAction *a_ShowCalender;
    QMenu *m_Cruise, *m_Edit, *m_Sort;
    linguist *worldText;
    CentreOp *myCop;

signals:

};

#endif // W_MAINCRUISEWINDOWS_H
