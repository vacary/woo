# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./GUI/YadeQtGUI
# Target is a library:  

LIBS += -lSerialization \
        -lM3D \
        -lConstants \
        -lQtGUIGenerator \
        -lRigidBody \
        -rdynamic 
INCLUDEPATH = ../../GUI/QtGUIGenerator \
              ../../plugins/Body/RigidBody \
              ../../yade \
              ../../toolboxes/Math/M3D \
              ../../toolboxes/Math/Constants \
              ../../toolboxes/Libraries/Serialization 
MOC_DIR = $(YADECOMPILATIONPATH) 
UI_DIR = $(YADECOMPILATIONPATH) 
OBJECTS_DIR = $(YADECOMPILATIONPATH) 
QMAKE_LIBDIR = ../../toolboxes/Libraries/Serialization/$(YADEDYNLIBPATH) \
               ../../toolboxes/Math/M3D/$(YADEDYNLIBPATH) \
               ../../toolboxes/Math/Constants/$(YADEDYNLIBPATH) \
               ../../GUI/QtGUIGenerator/$(YADEDYNLIBPATH) \
               ../../plugins/Body/RigidBody/$(YADEDYNLIBPATH) \
               $(YADEDYNLIBPATH) 
DESTDIR = $(YADEDYNLIBPATH) 
CONFIG += debug \
          warn_on \
          dll 
TEMPLATE = lib 
FORMS += YadeQtGeneratedMainWindow.ui 
HEADERS += YadeQtGUI.hpp \
           FpsTracker.hpp \
           GLViewer.hpp \
           QGLSubWindow.hpp \
           YadeQtMainWindow.hpp 
SOURCES += YadeQtGUI.cpp \
           FpsTracker.cpp \
           GLViewer.cpp \
           QGLSubWindow.cpp \
           YadeQtMainWindow.cpp 
