#include "mainwindow.h"
#include <QApplication>
#include <connecttobase.h>
#include <centreop.h>
#include <w_maincruisewindows.h>

int main(int argc, char *argv[])
{
    QApplication cruiseApp(argc, argv);
    ConnectToBase dbc;
    if (dbc.MakeConnection()){
        CentreOp ops;
        ops.StartOperating();
        w_MainCruiseWindows mcw;
        mcw.SetOperations(&ops);
        mcw.show();
        return cruiseApp.exec();
    }
}
