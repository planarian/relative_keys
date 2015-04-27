#ifndef PIANOSYNTH_H
#define PIANOSYNTH_H

#include "fluidsynth.h"

class PianoSynth
{
public:
    explicit PianoSynth();
    ~PianoSynth();
    void press(int midiNumber) const;
    void release(int midiNumber) const;

private:
    fluid_synth_t * synth;
    fluid_settings_t * settings;
    fluid_audio_driver_t * driver;
    fluid_sequencer_t * sequencer;
    short synthSeqID;
};

#endif // PIANOSYNTH_H
