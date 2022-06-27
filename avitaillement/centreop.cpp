#include "centreop.h"


CentreOp::CentreOp():
    myBase(),
    wiz(),
    myDebugs(),
    phraseo(),
    params(),
    myBuilder(),
    cruiseHandler()
{

}

void CentreOp::StartOperating(){

    if (myBase.isNewBase())
    {//Make the database if it is empty
       myBase.MakeNewBase();
    }
     //initiate the debugger (with basemanager)
    myDebugs.SetDataManager(&myBuilder);
    //read the parameters
    params.ReadParameters();
    datawizard::SetLanguage(params.GetLanguage());
    datawizard::SetFormats(params.GetDateSep(),params.GetDateFormat(),params.GetDecimalSep());
    //initiate the basemanager with parameter reader
    myBuilder.MeetParameterManager(&params);
    myBuilder.KeepDataWizard(&wiz);
    //initiate the basepicker (with basemanager)
    //Check the database

    //fake instruction
  /* if (myBuilder.LoadCruise(4)){ qDebug()<<"centreOP/startOP cruise succesfully loaded";myBuilder.BuildNewPlanType();}
    else
    {
        myBuilder.DeleteCruiseId(3);
        myBuilder.CreateNewCruise(QDate(2022,2,25),QDate(),"Corse",14,3,"toCome","SO 410");
        qDebug()<<"centre OP/startop cruise";
    }

*/

    //initiate the datawizard (with h the right language)

    //initiate the linguist
    phraseo.LoadLanguage();

    qDebug()<<"CentreOP/startoperatingg intialized linguist";

    //Select the appropriate display (splashscreen if new, new trip if no one is active-or select an old one if available, open plan if one is active or projected).
    //Every display gets a pointer to the picker, the manager, the linguist and the debugger.
    return;
}

linguist* CentreOp::GetLinguist(){
    return &phraseo;
}

debugger* CentreOp::getDebugger(){
    return &myDebugs;
}

man_cruiseplanning* CentreOp::GetCruiseHandler(){
    return &cruiseHandler;
}
