/*
 * makes an array of n strings, strictly numbered, reads all strings from a file where lines are numbered
 * to insert them at a strict place in the array
 * This ensures consistency when making translations
 * Some integers might be named in a definition, to retrieve strings more conveniently like Byes, Bno, BCancel
 * BOk,BModify, etc...
 */

#ifndef LINGUIST_H
#define LINGUIST_H

#include <QFile>
#include <QString>
#include <QVector>
#include <QDebug>
#include <datawizard.h>

enum{
    Byes, //0
    BNo,
    FirstDish,
    BoatsName,
    CruiseName,
    DofDeparture,
    DofArrival,
    Duration,
    NbOfPersons,
    Delete,
    recordedCruises,//10
    Modify,
    Finished,
    Or,
    Quit,//14
    Cancel,
    Welcome,
    NewCruise,
    DeleteCruise,
    CopyMenus,
    PasteMenus, //20
    ExportMenus,
    ByStartDate,
    ByEndDate,
    Ongoing,
    Ended,//25
    Parameters,
    ByName,
    Calender,
    Cruise,
    Edit,
    Sort,
    OurCruises,
    DateFormat, //33
    BoatsType,
    New,
    Plan,
    All

};
class linguist
{
public:
    linguist();
    void LoadLanguage();
    QString GetWord(qint16 indexWord);
    QString currentLanguage;

    QVector<QString> words;
};

#endif // LINGUIST_H
