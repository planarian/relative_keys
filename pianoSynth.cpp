#include "pianoSynth.h"


PianoSynth::PianoSynth()
{
    settings = new_fluid_settings();
    //load settings
    synth = new_fluid_synth(settings);
    driver = new_fluid_audio_driver(settings, synth);
    sequencer = new_fluid_sequencer2(0);

    //load sound font
    int fluid_res = fluid_synth_sfload(synth, ".\\piano.sf2", 1); //originally AJH_Piano.sf2(obtained from hammersound.net) -- Adrian Huang based on Warren Trachtman

}

PianoSynth::~PianoSynth()
{
    delete_fluid_sequencer(sequencer);
    delete_fluid_audio_driver(driver);
    delete_fluid_synth(synth);
}

void PianoSynth::press(int midiNumber) const
{
    fluid_synth_noteon(synth, 1, midiNumber, 127);
}

void PianoSynth::release(int midiNumber) const
{
    fluid_synth_noteoff(synth, 1, midiNumber);
}
