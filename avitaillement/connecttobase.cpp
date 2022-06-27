#include "connecttobase.h"

ConnectToBase::ConnectToBase()

{

}

bool ConnectToBase::MakeConnection() {
    QString baseName("/home/jeroen/apps perso/avitaillement/Avitaillement/Executables/ressources/BDAvitaillement.sqlite");
    /*QFile tst(baseName);
    if (!tst.exists()) baseName="../ressources/BDAvitaillement.sqlite";
    tst.close();*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(baseName);
    if (!db.open())
    {QMessageBox::critical(nullptr,"erreur SQlite","Sqlite n'est pas correctement installé",QMessageBox::Cancel);
        return false;
    }
    return true;
}
