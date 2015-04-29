HEADERS += \
    switcher.h \
    solfegeKeySet.h \
    solfege.h \
    resizeDialog.h \
    pianoSynth.h \
    pianoKeySet.h \
    piano.h \
    mainWindow.h \
    keySet.h \
    key.h \
    instrument.h \
    fullBlackKey.h \
    framedSolfege.h \
    fluidsynth.h \
    customColor.h \
    basicBlackKey.h \
    whiteKey.h

SOURCES += \
    whiteKey.cpp \
    switcher.cpp \
    solfegeKeySet.cpp \
    solfege.cpp \
    resizeDialog.cpp \
    pianoSynth.cpp \
    pianoKeySet.cpp \
    piano.cpp \
    mainWindow.cpp \
    main.cpp \
    keySet.cpp \
    key.cpp \
    instrument.cpp \
    fullBlackKey.cpp \
    framedSolfege.cpp \
    basicBlackKey.cpp

RESOURCES += \
    piano.qrc

RC_FILE = pianoIcon.rc

LIBS += libfluidsynth.dll










