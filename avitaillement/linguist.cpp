#include "linguist.h"

linguist::linguist():
    words()
{

}

void linguist::LoadLanguage(){
    QString fileName=datawizard::nomLinguist;
    QFile learnLanguage(fileName);
    if (learnLanguage.open(QIODevice::ReadOnly | QIODevice::Text)){
        while (!learnLanguage.atEnd()){
            words<<datawizard::RightSplit(learnLanguage.readLine(),"/").trimmed();
        }
    };
}
QString linguist::GetWord(qint16 indexWord){
    return words[indexWord];

}
