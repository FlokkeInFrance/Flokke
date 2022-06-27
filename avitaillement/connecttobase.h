/*
 * make contact with the database
 * launch sql
 */

#ifndef CONNECTTOBASE_H
#define CONNECTTOBASE_H

#include <QSqlDatabase>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QtDebug>

class ConnectToBase
{
public:
    ConnectToBase();
    bool MakeConnection();

private:
    bool connectedToDB();
};

#endif // CONNECTTOBASE_H
