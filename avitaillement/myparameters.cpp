
#include "myparameters.h"

myparameters::myparameters():
    meals(),
    dishes(),
    moorings(),
    dietetics(),
    navStyle(),
    language("francais"),
    decSep(","),
    dateSep("/"),
    dateFormat("dmy")
{

}

bool myparameters::ReadParameters(){
    bool valid(true);
    QFile rp(datawizard::nomParams);
    if (rp.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString nextline(""),paramTag(""),paramVal("");
        while (! rp.atEnd()){
            nextline=rp.readLine();
            paramVal=datawizard::RightSplit(nextline,"/",paramTag);
            if (paramTag.isEmpty()||paramVal.isEmpty()) valid=false;
            else{
                if (paramTag=="language") language=paramVal;
                if (paramTag=="dec sep")  decSep=paramVal;
                if (paramTag=="date sep") dateSep=paramVal;
                if (paramTag=="date format") dateFormat=paramVal;
                if (paramTag=="meal") {meals<<paramVal;}
                if (paramTag=="dish") dishes<<paramVal;
                if (paramTag=="navigation") navStyle<<paramVal;
                if (paramTag=="diet") dietetics<<paramVal;
                if (paramTag=="moor") moorings<<paramVal;
            }
        }
        rp.close();
    }
    else valid=false;
    return valid;
}

QStringList myparameters::GetMeals(){
    return meals;
}

QStringList myparameters::GetDishes(){
    return dishes;
}

QStringList myparameters::GetMoorings(){
    return moorings;
}

QStringList myparameters::GetDietetics(){
    return dietetics;
}

QString myparameters::GetLanguage(){
    return language;
}

QString myparameters::GetDecimalSep(){
    return decSep;
}

QString myparameters::GetDateSep(){
    return dateSep ;
}

QString myparameters::GetDateFormat(){
    return dateFormat;
}

void myparameters::AddMeal (QString addP){
    AddParameter("meal",addP);
}

void myparameters::ModifyMeal (QString oM,QString nM){
    ModifyParameter("meal",oM,nM);
}

void myparameters::AddDish(QString addP){
    AddParameter("dish",addP);
}

void myparameters::ModifyDish (QString oM,QString nM){
    ModifyParameter("dish",oM,nM);
}

void myparameters::AddMooring(QString addP){
    AddParameter("moor",addP);
}

void myparameters::ModifyMooring(QString oM, QString nM){
    ModifyParameter("moor",oM,nM);
}

void myparameters::AddDietFlag(QString addP){
    AddParameter("diet",addP);
}

void myparameters::SetDecimalSep(QString setP){
    decSep=setP;
}

void myparameters::SetDateSep(QString setP){
    dateSep=setP;
}

void myparameters::SetDateFromat(QString setP){
    dateFormat=setP;
}

void myparameters::AddParameter(QString tag,QString valueP){
    QString toAdd=tag+"/"+valueP+"n";
    QFile wf(datawizard::nomParams);
    wf.open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream textW(&wf);
    wf.readAll();
    textW<<toAdd;
    wf.close();
}

void myparameters::ModifyParameter(QString tag,QString oldP, QString newP){
    QFile rp(datawizard::nomParams);
    QStringList rewrite;
    if (rp.open(QIODevice::ReadWrite | QIODevice::Text)){
        QString nextline(""),paramTag(""),paramVal("");
        while (! rp.atEnd()){
            nextline=rp.readLine();
            paramVal=datawizard::RightSplit(nextline,"/,",paramTag);
            if (paramTag!=tag) rewrite<<nextline;
            else{
                if (paramVal==oldP){
                    rewrite<<QString("%1/%2").arg(tag,newP);
                }
                else rewrite<<nextline;
            }
        }
        rp.seek(0);
        QTextStream rewt(&rp);
        for (auto &ln:rewrite) rewt<<ln;
        rp.close();
    }
}
