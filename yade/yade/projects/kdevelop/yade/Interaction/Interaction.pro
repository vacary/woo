# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./yade/Interaction
# Target is a library:  

LIBS += -rdynamic 
INCLUDEPATH = ../../yade/yade \
              ../../yade/Geometry \
              ../../yade/MultiMethods \
              ../../yade/Factory \
              ../../toolboxes/Libraries/Math \
              ../../toolboxes/Libraries/Serialization 
MOC_DIR = $(YADECOMPILATIONPATH) 
UI_DIR = $(YADECOMPILATIONPATH) 
OBJECTS_DIR = $(YADECOMPILATIONPATH) 
QMAKE_LIBDIR = $(YADEDYNLIBPATH) 
DESTDIR = $(YADEDYNLIBPATH) 
CONFIG += release \
          warn_on \
          staticlib 
TEMPLATE = lib 
HEADERS += BroadCollider.hpp \
           Contact.hpp \
           Interaction.hpp \
           InteractionGeometry.hpp \
           NarrowCollider.hpp \
           CollisionFunctor.hpp \
           CollisionMultiMethodsManager.hpp 
SOURCES += BroadCollider.cpp \
           Contact.cpp \
           Interaction.cpp \
           InteractionGeometry.cpp \
           NarrowCollider.cpp \
           CollisionMultiMethodsManager.cpp 
