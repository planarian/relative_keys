#ifndef KEY_H
#define KEY_H

#include <QWidget>
#include <QString>
#include "customColor.h"

class Switcher;

class Key : public QWidget  //abstract base class
{
    Q_OBJECT
public:
    explicit Key(const Switcher *, const CustomColor & , const CustomColor &, QWidget *parent = 0);
    virtual ~Key() {}
    virtual void press(bool directClick = false, bool playChord = false) = 0;
    virtual void release(bool directClick = false) = 0;
    void checkIfVisible();
    QPoint absPosition() const;
    CustomColor activeColor, inactiveColor;
    QColor textColor;
    unsigned int midiNumber, keyNumber;
    bool directCl; //i.e. direct-click flag
    QString solValue;

signals:
    void switcherSignal(Key *);
    void notVisibleSignal(Key *); //must climb three classes to reach framed solfege slot

protected:
    bool isPressed;
    const Switcher * toggles;
    virtual void paintEvent(QPaintEvent *);
    short int keyType; //0 = white key, 1 = full black, 2= basic black

};

#endif // KEY_H
