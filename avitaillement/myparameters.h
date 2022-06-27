/*
 * As an input, it receives a pointer to the datamanager which will read the database
 *This class reads the parameters from a stringlist received by datamanager

 *will order replacement of  values when needed
 *stores all relevant values internally :
 * - current language (which is also the linguist's file name)
 * - decimal separator
 * - date separator
 * - date format
 * - list of mooring types
 * - list of navigation styles for cooking
 * - list of diet values
 * - list of all generic menu types (breakfast, midday, dinner...)
 * - list of dishes per menu (entry, main dish, dessert...)
 * - list of typical menu plan, list of dishes per typical menu
 *
 * to order the list of menus and dishes, tags are constructed like Menu1, Menu2, Menu3, Menu1-Dish1,...
 * A typical list of default values is found in file params'language name' (like paramsfrancais) for instance.
 * The class makes lists of choices when necessary for implemented languages, separators, date, mooring types
 * date formats
 */
#ifndef MYPARAMETERS_H
#define MYPARAMETERS_H

#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <datawizard.h>
#include <QDebug>


class myparameters
{
public:
    myparameters();

    bool ReadParameters();
    QStringList GetMeals();
    QStringList GetDishes();
    QStringList GetMoorings();
    QStringList GetDietetics();
    QString GetLanguage();
    QString GetDecimalSep();
    QString GetDateSep();
    QString GetDateFormat();
    void AddMeal       (QString addP);
    void ModifyMeal    (QString oM,QString nM);
    void AddDish       (QString addP);
    void ModifyDish    (QString oM,QString nM);
    void AddMooring    (QString addP);
    void ModifyMooring (QString oM,QString nM);
    void AddDietFlag   (QString addP);
    void SetDecimalSep (QString setP);
    void SetDateSep    (QString setP);
    void SetDateFromat (QString setP);

private:
    QStringList meals, dishes, moorings, dietetics,navStyle;
    QString language,decSep,dateSep,dateFormat;
    void AddParameter(QString tag,QString valueP);
    void ModifyParameter(QString tag,QString oldP, QString newP);
};

#endif // MYPARAMETERS_H
