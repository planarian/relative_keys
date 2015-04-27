#ifndef FULLBLACKKEY_H
#define FULLBLACKKEY_H

#include "basicBlackKey.h"

//for some reason these three MUST be included
#include "pianoSynth.h"
#include "instrument.h"
#include "switcher.h"


class FullBlackKey : public BasicBlackKey
{
    Q_OBJECT
public:
    explicit FullBlackKey(const PianoSynth *, const Instrument *, const Switcher *,
                          const CustomColor &, const CustomColor &, QWidget * parent = 0);
    void press(bool directClick = false, bool chordPlay = false);
    void release(bool directClick = false);

protected:
    const PianoSynth * audio;
    const Instrument * instr;

};

#endif // FULLBLACKKEY_H
