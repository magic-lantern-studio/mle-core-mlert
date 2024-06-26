QT += widgets
#QT -= gui

TARGET = mlert
TEMPLATE = lib
DEFINES += LIBMLERT_RUNTIME_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/../../common/include \
    $$PWD/../../common/src/foundation \
    $$PWD/../../common/src/input \
    $$PWD/../../linux/include \
    /opt/MagicLantern/include \
    /usr/local/include

DEFINES += \
    MLE_NOT_DLL \
    MLE_INTERNAL \
    MLE_DIGITAL_PLAYPRINT \
    ML_MATH_DEBUG=0 \
    ML_FIXED_POINT=0 \
    MLE_QT \
    MLE_KEY_DEBUG

SOURCES += \
    $$PWD/../../common/src/foundation/3danim.cxx \
    $$PWD/../../common/src/foundation/3dchar.cxx \
    $$PWD/../../common/src/foundation/MleActor.cxx \
    $$PWD/../../common/src/foundation/MleActorGC.cxx \
    $$PWD/../../common/src/foundation/MleDirector.cxx \
    $$PWD/../../common/src/foundation/MleDso.cxx \
    $$PWD/../../common/src/foundation/MleEvent.cxx \
    $$PWD/../../common/src/foundation/MleEventDispatcher.cxx \
    $$PWD/../../common/src/foundation/MleGroup.cxx \
    $$PWD/../../common/src/foundation/MleLoad.cxx \
    $$PWD/../../common/src/foundation/MleMediaRef.cxx \
    $$PWD/../../common/src/foundation/MleMonitor.cxx \
    $$PWD/../../common/src/foundation/MleNotifier.cxx \
    $$PWD/../../common/src/foundation/MleObject.cxx \
    $$PWD/../../common/src/foundation/MlePq.cxx \
    $$PWD/../../common/src/foundation/MlePtrArray.cxx \
    $$PWD/../../common/src/foundation/MlePtrDict.cxx \
    $$PWD/../../common/src/foundation/MleRole.cxx \
    $$PWD/../../common/src/foundation/MleScene.cxx \
    $$PWD/../../common/src/foundation/MleScheduler.cxx \
    $$PWD/../../common/src/foundation/MleSet.cxx \
    $$PWD/../../common/src/foundation/MleStage.cxx

SOURCES += \
    $$PWD/../../common/src/input/MleKeyboardPolled.cxx

HEADERS += \
    $$PWD/../../common/src/foundation/mle/3danim.h \
    $$PWD/../../common/src/foundation/mle/3dchar.h \
    $$PWD/../../common/src/foundation/mle/MleActorGC.h \
    $$PWD/../../common/src/foundation/mle/MleActor.h \
    $$PWD/../../common/src/foundation/mle/MleDirector.h \
    $$PWD/../../common/src/foundation/mle/MleDso.h \
    $$PWD/../../common/src/foundation/mle/MleEventDispatcher.h \
    $$PWD/../../common/src/foundation/mle/MleEvent.h \
    $$PWD/../../common/src/foundation/mle/MleGroup.h \
    $$PWD/../../common/src/foundation/mle/MleLoad.h \
    $$PWD/../../common/src/foundation/mle/MleMediaRefConverter.h \
    $$PWD/../../common/src/foundation/mle/MleMediaRef.h \
    $$PWD/../../common/src/foundation/mle/MleMonitor.h \
    $$PWD/../../common/src/foundation/mle/MleNotifier.h \
    $$PWD/../../common/src/foundation/mle/MleObject.h \
    $$PWD/../../common/src/foundation/mle/MleOpcode.h \
    $$PWD/../../common/src/foundation/mle/MlePlatformData.h \
    $$PWD/../../common/src/foundation/mle/MlePq.h \
    $$PWD/../../common/src/foundation/mle/MlePropDataset.h \
    $$PWD/../../common/src/foundation/mle/MleProperty.h \
    $$PWD/../../common/src/foundation/mle/MlePtrArray.h \
    $$PWD/../../common/src/foundation/mle/MlePtrContainer.h \
    $$PWD/../../common/src/foundation/mle/MlePtrDict.h \
    $$PWD/../../common/src/foundation/mle/MleRole.h \
    $$PWD/../../common/src/foundation/mle/MleRuntime.h \
    $$PWD/../../common/src/foundation/mle/MleScene.h \
    $$PWD/../../common/src/foundation/mle/MleScheduler.h \
    $$PWD/../../common/src/foundation/mle/MleSet.h \
    $$PWD/../../common/src/foundation/mle/MleStageFuncs.h \
    $$PWD/../../common/src/foundation/mle/MleStage.h \
    $$PWD/../../common/src/foundation/mle/MleTables.h

HEADERS += \
    $$PWD/../../common/src/input/mle/MleKeyboardPolled.h \
    $$PWD/../../common/src/input/mle/MleKeyboardEvent.h \
    $$PWD/../../common/src/input/mle/MleMousePolled.h \
    $$PWD/../../common/src/input/mle/MleMouseEvent.h \
    $$PWD/../../common/src/input/mle/MleKeyMap.h

HEADERS += \
    $$PWD/../../linux/include/mle/mlPlatformData.h

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib/mle/runtime
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += target headers
}
#!isEmpty(target.path): INSTALLS += target
