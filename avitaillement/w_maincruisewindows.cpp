#include "w_maincruisewindows.h"
#include "w_cruiseorganiser.h"

w_MainCruiseWindows::w_MainCruiseWindows(QWidget *parent) : QMainWindow(parent),

    cruises(new W_CruiseOrganiser(this)),
    debugs(nullptr),
    a_quit(nullptr),
    a_newCruise(nullptr),
    a_deleteCruise(nullptr),
    a_copyMenus(nullptr),
    a_pasteMenus(nullptr),
    a_exportMenus(nullptr),
    a_sortByName(nullptr),
    a_onlyNotFinished(nullptr),
    a_sortByStatus(nullptr),
    a_sortByStart(nullptr),
    a_sortByEnd(nullptr),
    a_parameters(nullptr),
    a_ShowCalender(nullptr),
    m_Cruise(nullptr),
    m_Edit(nullptr),
    m_Sort(nullptr),
    worldText(nullptr),
    myCop(nullptr)


{

}

QString w_MainCruiseWindows::wT(int toTranslate){
    return worldText->GetWord(toTranslate);
}

void w_MainCruiseWindows::SetOperations(CentreOp *inOp){

    myCop=inOp;
    worldText=myCop->GetLinguist();
    debugs=myCop->getDebugger();
    cruises->setCenterOp(myCop);

    setCentralWidget(cruises);
    BuildActions();
    debugs->ShowVariable(wT(Cruise));
}

void w_MainCruiseWindows::BuildActions(){

    a_ShowCalender=new QAction(wT(Calender));

    m_Cruise=menuBar()->addMenu(wT(Cruise));
        a_quit=m_Cruise->addAction(wT(Quit));
        a_newCruise=m_Cruise->addAction(wT(NewCruise));
        a_deleteCruise=m_Cruise->addAction(wT(DeleteCruise));
        a_exportMenus=m_Cruise->addAction(wT(ExportMenus));

    m_Edit=menuBar()->addMenu(wT(Edit));
        a_copyMenus=m_Edit->addAction(wT(CopyMenus));
        a_pasteMenus=m_Edit->addAction(wT(PasteMenus));
        a_parameters=m_Edit->addAction(wT(Parameters));

    m_Sort=menuBar()->addMenu(wT(Sort));
        a_sortByName=m_Sort->addAction(wT(ByName));
        a_onlyNotFinished=m_Sort->addAction(wT(Ongoing));
        a_sortByStatus=m_Sort->addAction(wT(Ongoing)+"/"+wT(Finished));
        a_sortByStart=m_Sort->addAction(wT(ByStartDate));
        a_sortByEnd=m_Sort->addAction(wT(ByEndDate));

   setWindowTitle(wT(Cruise));
}

