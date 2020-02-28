QT -= gui

TARGET = mlert
TEMPLATE = lib
DEFINES += LIBMLERT_INVENTOR_LIBRARY

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

INCLUDEPATH += $$PWD/../../common/include $$PWD/../../common/src/foundation /opt/MagicLantern/include /usr/local/include

#DEFINES += MLE_DIGITAL_WORKPRINT MLE_NOT_DLL
DEFINES += \
    MLE_NOT_DLL \
    MLE_INTERNAL \
    MLE_DIGITAL_WORKPRINT \
    ML_MATH_DEBUG=0 \
    ML_FIXED_POINT=0 \

SOURCES += \
    $PWD/../../../common/src/foundation/3danim.cxx \
    $PWD/../../../common/src/foundation/3dchar.cxx \
    $PWD/../../../common/src/foundation/loaddwp.cxx \
    $PWD/../../../common/src/foundation/loadSet.cxx \
    $PWD/../../../common/src/foundation/MleActorClass.cxx \
    $PWD/../../../common/src/foundation/MleActor.cxx \
    $PWD/../../../common/src/foundation/MleDirector.cxx \
    $PWD/../../../common/src/foundation/MleDso.cxx \
    $PWD/../../../common/src/foundation/MleEvent.cxx \
    $PWD/../../../common/src/foundation/MleEventDispatcher.cxx \
    $PWD/../../../common/src/foundation/MleGroupClass.cxx \
    $PWD/../../../common/src/foundation/MleGroup.cxx \
    $PWD/../../../common/src/foundation/MleLoad.cxx \
    $PWD/../../../common/src/foundation/MleMediaRefClass.cxx \
    $PWD/../../../common/src/foundation/MleMediaRef.cxx \
    $PWD/../../../common/src/foundation/MleMonitor.cxx \
    $PWD/../../../common/src/foundation/MleNotifier.cxx \
    $PWD/../../../common/src/foundation/MleObject.cxx \
    $PWD/../../../common/src/foundation/MlePq.cxx \
    $PWD/../../../common/src/foundation/MlePtrArray.cxx \
    $PWD/../../../common/src/foundation/MlePtrDict.cxx \
    $PWD/../../../common/src/foundation/MleRoleClass.cxx \
    $PWD/../../../common/src/foundation/MleRole.cxx \
    $PWD/../../../common/src/foundation/MleSceneClass.cxx \
    $PWD/../../../common/src/foundation/MleScene.cxx \
    $PWD/../../../common/src/foundation/MleScheduler.cxx \
    $PWD/../../../common/src/foundation/MleSetClass.cxx \
    $PWD/../../../common/src/foundation/MleSet.cxx \
    $PWD/../../../common/src/foundation/MleStageClass.cxx \
    $PWD/../../../common/src/foundation/MleStage.cxx

HEADERS += \
    $PWD/../../../common/src/foundation/mle/3danim.h \
    $PWD/../../../common/src/foundation/mle/3dchar.h \
    $PWD/../../../common/src/foundation/mle/MleActorClass.h \
    $PWD/../../../common/src/foundation/mle/MleActorGC.h \
    $PWD/../../../common/src/foundation/mle/MleActor.h \
    $PWD/../../../common/src/foundation/mle/MleDirector.h \
    $PWD/../../../common/src/foundation/mle/MleDso.h \
    $PWD/../../../common/src/foundation/mle/MleEventDispatcher.h \
    $PWD/../../../common/src/foundation/mle/MleEvent.h \
    $PWD/../../../common/src/foundation/mle/MleGroupClass.h \
    $PWD/../../../common/src/foundation/mle/MleGroup.h \
    $PWD/../../../common/src/foundation/mle/MleLoad.h \
    $PWD/../../../common/src/foundation/mle/MleMediaRefClass.h \
    $PWD/../../../common/src/foundation/mle/MleMediaRefConverter.h \
    $PWD/../../../common/src/foundation/mle/MleMediaRef.h \
    $PWD/../../../common/src/foundation/mle/MleMonitor.h \
    $PWD/../../../common/src/foundation/mle/MleNotifier.h \
    $PWD/../../../common/src/foundation/mle/MleObject.h \
    $PWD/../../../common/src/foundation/mle/MleOpcode.h \
    $PWD/../../../common/src/foundation/mle/MlePlatformData.h \
    $PWD/../../../common/src/foundation/mle/MlePq.h \
    $PWD/../../../common/src/foundation/mle/MlePropDataset.h \
    $PWD/../../../common/src/foundation/mle/MleProperty.h \
    $PWD/../../../common/src/foundation/mle/MlePtrArray.h \
    $PWD/../../../common/src/foundation/mle/MlePtrContainer.h \
    $PWD/../../../common/src/foundation/mle/MlePtrDict.h \
    $PWD/../../../common/src/foundation/mle/MleRoleClass.h \
    $PWD/../../../common/src/foundation/mle/MleRole.h \
    $PWD/../../../common/src/foundation/mle/MleRuntime.h \
    $PWD/../../../common/src/foundation/mle/MleSceneClass.h \
    $PWD/../../../common/src/foundation/mle/MleScene.h \
    $PWD/../../../common/src/foundation/mle/MleScheduler.h \
    $PWD/../../../common/src/foundation/mle/MleSetClass.h \
    $PWD/../../../common/src/foundation/mle/MleSet.h \
    $PWD/../../../common/src/foundation/mle/MleStageClass.h \
    $PWD/../../../common/src/foundation/mle/MleStageFuncs.h \
    $PWD/../../../common/src/foundation/mle/MleStage.h \
    $PWD/../../../common/src/foundation/mle/MleTables.h

# Default rules for deployment.
unix {
    target.path = /opt/MagicLantern/lib/mle/qt
    headers.path = /opt/MagicLantern/include/mle
    headers.files = $$HEADERS
    INSTALLS += target headers
}
!isEmpty(target.path): INSTALLS += target
