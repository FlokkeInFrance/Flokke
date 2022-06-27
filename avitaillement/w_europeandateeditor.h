#ifndef W_EUROPEANDATEEDITOR_H
#define W_EUROPEANDATEEDITOR_H

#include <QObject>
#include <QDateEdit>
#include <linguist.h>

class W_EuropeanDateEditor : public QDateEdit{
public:
    explicit W_EuropeanDateEditor(QWidget *parent = nullptr);
    void Initialize(linguist ling);
    void MinimumtoCurrent();
    void Maximum(QDate mDate);
};

#endif // W_EUROPEANDATEEDITOR_H
