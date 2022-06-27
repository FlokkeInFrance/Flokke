#ifndef CENTREOP_H
#define CENTREOP_H
/*
class to launch other classes, pass on pointers to helper classes
On startup checks with the checkbase class if the base exists or needs to be created
Then goes on with basemanager, then parameters.
Then datawizard and feeds it the datesep, decsep and date format.
Then linguist, and basepicker.
Questions if a cruise is underway (basemanager)
Then launches the cruise selector window (or directly a planning if a cruise is under way)
If no cruise exists, launch introduction and create a cruise (cruise window in creation mode)
 */

#include <connecttobase.h>
#include <checkbase.h>
#include <debugger.h>
#include <linguist.h>
#include <datawizard.h>
#include <myparameters.h>
#include <debugger.h>
#include <basemanager.h>
#include "man_cruiseplanning.h"

class CentreOp
{
public:
    CentreOp();
    void StartOperating();
    linguist* GetLinguist();
    debugger* getDebugger();
    man_cruiseplanning* GetCruiseHandler();


private:
    CheckBase myBase;
    datawizard wiz;
    debugger myDebugs;
    linguist phraseo;
    myparameters params;
    basemanager myBuilder;
    man_cruiseplanning cruiseHandler;
};

#endif // CENTREOP_H
