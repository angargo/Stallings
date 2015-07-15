TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    graph.cpp \
    subgroup.cpp \
    folding.cpp \
    whitehead.cpp

HEADERS += \
    subgroup.hpp \
    graph.hpp \
    folding.hpp \
    ../whitehead.hpp \
    whitehead.hpp

OTHER_FILES += \
    ../assets/test.in

QMAKE_CXXFLAGS += -std=c++11
