# File generated by kdevelop qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./DataClass/GeometricalModel/LineSegment
# Target is a library:  

HEADERS += LineSegment.hpp 
SOURCES += LineSegment.cpp 
LIBS += -lyade-lib-multimethods \
        -rdynamic 
QMAKE_LIBDIR = /usr/local/lib/yade/yade-libs/ 
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

