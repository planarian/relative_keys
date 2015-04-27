#ifndef SOLFEGEKEYSET_H
#define SOLFEGEKEYSET_H

#include "keySet.h"

class PianoSynth;
class Instrument;
class Switcher;
class BasicBlackKey;


class SolfegeKeySet : public KeySet
{
    Q_OBJECT

public:
    explicit SolfegeKeySet(const PianoSynth *, const Instrument *, const Switcher *, const int, QWidget * parent = 0);
    ~SolfegeKeySet() {}
    void setInnerMargins(int);
    const int getInnerMargin() const;

signals:
    void notVisibleSignal(Key *); //relays TO FramedSolfege

public slots:
    void notVisibleSlot(Key *); //relays FROM key signal

private:
    void createBlackKeys();
    void createWhiteKeys();
    void arrangeBlackKeys();
    void assignNumbers();
    void pitchTranspose();
    void colorTranspose();

    short int pitchOffset;
    int *top, *bottom, *left, *right;

    QList<BasicBlackKey *> blackKeys_lower;

};

#endif // SOLFEGEKEYSET_H
