#include "w_cruiseorganiser.h"

#include "w_maincruisewindows.h"

W_CruiseOrganiser::W_CruiseOrganiser(QWidget *parent) : QWidget(parent),


    //Layouts
        mainLayout(new QHBoxLayout), buttonEditZone(new QHBoxLayout),buttonListZone(new QHBoxLayout),
        detailZone(new QVBoxLayout),listZone(new QVBoxLayout),typeMenuZone(new QVBoxLayout),
        editZone (new QFormLayout),

    //Widgets
       title (new QLabel()),headerOfEdit(new QLabel()),headerOfList(new QLabel()),
       cruiseName(new QLineEdit()),boatName(new QLineEdit()),boatType(new QLineEdit()),
       startCruise(new W_EuropeanDateEditor()),endCruise (new W_EuropeanDateEditor()),
       cruiseFinished(new QCheckBox()),
       nbOfSailors(new QSpinBox()),nbDays(new QSpinBox()),
       listCruises(new QListWidget()),
       menutypes(new QTreeWidget()),

     //buttons
       bNew (new QPushButton()), bDelete(new QPushButton()), bPlan(new QPushButton()),bCancel(new QPushButton()),
       bFinished(new QPushButton()),bBNotFinished(new QPushButton()),bAll(new QPushButton()),
    //Actions

    //variables
       center(nullptr),
       cruiseHandler(nullptr),
       internat(nullptr),
       aCruise(),
       toRefresh(true)
{

}

QString W_CruiseOrganiser::getWord(int inCode){
 return internat->GetWord(inCode);
}

QString W_CruiseOrganiser::getWordSC(int inCode){
 return QString(internat->GetWord(inCode)+" : ");
}

void W_CruiseOrganiser::ObjectModel(){
    connect(listCruises,SIGNAL(currentRowChanged(int)),SLOT(ListClicked(int)));
    connect(cruiseName,SIGNAL(editingFinished()),SLOT(UpdateCruise()));
    connect(boatName,SIGNAL(editingFinished()),SLOT(UpdateCruise()));
    connect(boatType,SIGNAL(editingFinished()),SLOT(UpdateBoatType()));
    connect(startCruise,SIGNAL(editingFinished()),SLOT(UpdateDates()));
    connect(endCruise,SIGNAL(editingFinished()),SLOT(UpdateDates()));
    connect(nbDays,SIGNAL(editingFinished()),SLOT(UpdateDates()));
    connect(cruiseFinished,SIGNAL(editingFinished()),SLOT(UpdateCruise()));
    connect(nbOfSailors,SIGNAL(editingFinished()),SLOT(UpdateCrew()));
}

void W_CruiseOrganiser::setCenterOp(CentreOp *ctr){
    center=ctr;
    internat=center->GetLinguist();
    setLayout(mainLayout);
    title->setText(wT(OurCruises));
    cruiseHandler=center->GetCruiseHandler();
    setupWidget();
    resetViews();
    ObjectModel();
}

QString W_CruiseOrganiser::wT(int toTranslate){
    return internat->GetWord(toTranslate);
}

void W_CruiseOrganiser::setupWidget(){
    //place Layouts
    mainLayout->addLayout(detailZone);
    mainLayout->addLayout(listZone);
    mainLayout->addLayout(typeMenuZone);

    detailZone->addLayout(editZone);
    detailZone->addLayout(buttonEditZone);

    listZone->addWidget(title);title->setAlignment(Qt::AlignCenter) ;
    listZone->addWidget(listCruises);
    listZone->addLayout(buttonListZone);

    //place Widgets
    editZone->addRow(getWordSC(CruiseName),cruiseName);
    editZone->addRow(getWordSC(BoatsName),boatName);
    editZone->addRow(getWordSC(BoatsType),boatType);
    editZone->addRow(getWordSC(NbOfPersons),nbOfSailors);
    editZone->addRow(getWordSC(DofDeparture),startCruise);
    editZone->addRow(getWordSC(Duration),nbDays);
    editZone->addRow(getWordSC(DofArrival),endCruise);
    editZone->addRow(getWordSC(Finished),cruiseFinished);

    bNew->setText(getWord(New));
    bDelete->setText(getWord(Delete));
    bPlan->setText(getWord(Plan));
    bCancel->setText(getWord(Cancel));
    bBNotFinished->setText(getWord(Ongoing));
    bFinished->setText(getWord(Ended));
    bAll->setText(getWord(All));


    buttonEditZone->addWidget(bPlan);
    buttonEditZone->addWidget(bNew);
    buttonEditZone->addWidget(bCancel);
    buttonEditZone->addWidget(bDelete);

    buttonListZone->addWidget(bBNotFinished);
    buttonListZone->addWidget(bAll);
    buttonListZone->addWidget(bFinished);

    typeMenuZone->addWidget(menutypes);

    listCruises->setSelectionMode(QAbstractItemView::SingleSelection);
    listCruises->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void W_CruiseOrganiser::resetViews(){
    toRefresh=false;
    cruiseName->setText("");
    boatName->setText("");
    boatType->setText("");
    nbOfSailors->setValue(1);
    nbDays->setValue(0);
    cruiseFinished->setChecked(false);
    startCruise->setDate(QDate::currentDate());
    endCruise->setDate(QDate::currentDate());
    menutypes->clear();


    toRefresh=true;
}

void W_CruiseOrganiser::showActiveCruise(){
    aCruise=cruiseHandler->GetSelectedCruise();
    cruiseName->setText(aCruise.intitule);
    boatName->setText(aCruise.nomBateau);
    boatType->setText(aCruise.typeBateau);
    nbOfSailors->setValue(aCruise.nombrePersonnes);
    nbDays->setValue(aCruise.nombreJours);
    cruiseFinished->setChecked(aCruise.terminee);
    startCruise->setDate(aCruise.dateDepart);
    endCruise->setDate(aCruise.dateFin);
}

void W_CruiseOrganiser::getCruiseList(){
    listCruises->clear();
    listCruises->addItems(cruiseHandler->CruisesToShow());
    if (listCruises->count()){
         cruiseHandler->SelectCruiseFromList(0);
         listCruises->setCurrentRow(0);
         showActiveCruise();
    }else
    listCruises->addItem("croisières");
}


void W_CruiseOrganiser::getMenuTypes(){

}

/*---------------------------------------------------------
 *                  SLOTS
 *---------------------------------------------------------
 */

void W_CruiseOrganiser::UpdateCruise(){
    if (toRefresh){
        aCruise.dateDepart=startCruise->date();
        aCruise.dateFin=startCruise->date();
        aCruise.intitule=cruiseName->text();
        aCruise.nomBateau=boatName->text();
        aCruise.typeBateau=boatType->text();
        aCruise.terminee=false;
        if (cruiseFinished->isChecked()) aCruise.terminee=true;
        aCruise.nombreJours=nbDays->value();
        aCruise.nombrePersonnes=nbOfSailors->value();
        cruiseHandler->Modify(aCruise);
    }
}

void W_CruiseOrganiser::UpdateDates(){
    cruiseHandler->ChangeCruiseTiming(startCruise->date(),endCruise->date(),nbDays->value());
    showActiveCruise();
}

void W_CruiseOrganiser::UpdateCrew(){
    cruiseHandler->ChangeNumberPersons(nbOfSailors->value());
}

void W_CruiseOrganiser::UpdateBoatType(){
    cruiseHandler->ChangeBoatType(boatType->text());
}

void W_CruiseOrganiser::NewCruise(){
    cruiseHandler->CreateCruise();
    showActiveCruise();
    //update CruiseList
}

void W_CruiseOrganiser::CommitAndPlan(){
    cruiseHandler->CommitNewCruise();
}

void W_CruiseOrganiser::DeleteCruise(){
    
}

void W_CruiseOrganiser::ListClicked(int el){
    aCruise=cruiseHandler->SelectCruiseFromList(el);
}
