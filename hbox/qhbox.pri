
include ($$PWD/cpp/qhboxcpp.pri)

SOURCES += $$files($$PWD/*.c, false)
SOURCES += $$files($$PWD/h3rdparty/*.c,false)
HEADERS += $$files($$PWD/*.h, false) 
INCLUDEPATH += $$PWD

