# File generated by kdevelop qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./Container/InteractionContainer/InteractionHashMap
# Target is a library:  

HEADERS += InteractionHashMap.hpp \
           InteractionHashMapIterator.hpp 
SOURCES += InteractionHashMap.cpp \
           InteractionHashMapIterator.cpp 
LIBS += -rdynamic
QMAKE_CXXFLAGS_RELEASE += -lpthread \
-pthread
QMAKE_CXXFLAGS_DEBUG += -lpthread \
-pthread
DESTDIR = ../../../../../bin
CONFIG += debug \
          thread \
warn_on \
dll
TEMPLATE = lib
INCLUDEPATH += /usr/local/include

