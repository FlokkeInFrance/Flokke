#include "w_europeandateeditor.h"

W_EuropeanDateEditor::W_EuropeanDateEditor(QWidget *parent): QDateEdit(parent)

{
setCalendarPopup(true);
}

void W_EuropeanDateEditor::Initialize(linguist ling){
    setDisplayFormat(ling.GetWord(DateFormat));
}
