# File generated by kdevelop qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./Engine/EngineUnit/Box2PolyhedralSweptSphere
# Target is a library:  

LIBS += -lPolyhedralSweptSphere \
        -lInteractingGeometryMetaEngine \
        -lBox \
        -rdynamic 
INCLUDEPATH += /usr/local/include/ \
               ../../../DataClass/InteractingGeometry/PolyhedralSweptSphere 
QMAKE_LIBDIR = ../../../../../bin \
               /usr/local/lib/yade/yade-package-common/ \
               /usr/local/lib/yade/yade-libs/ 
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
HEADERS += Box2PolyhedralSweptSphere.hpp 
SOURCES += Box2PolyhedralSweptSphere.cpp 
