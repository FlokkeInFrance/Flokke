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
    //initiate other helpers
    phraseo.LoadLanguage();
    datawizard::SetLanguage(params.GetLanguage());   
    datawizard::SetFormats(params.GetDateSep(),params.GetDateFormat(),params.GetDecimalSep());

    //initiate the basemanager with helpers and datawizard
    myBuilder.KeepDataWizard(&wiz);
    myBuilder.SetHelpers(this,&phraseo,&params,&myDebugs,&wiz);
    cruiseHandler.SetHelpers(this,&phraseo,&params,&myDebugs,&wiz);

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
