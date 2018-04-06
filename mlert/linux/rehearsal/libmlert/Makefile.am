#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/rehearsal

#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/local/lib
lib_LTLIBRARIES = libmlert.la

#######################################
# Build information for each library

# Sources for libmlert
libmlert_la_SOURCES = \
	$(top_srcdir)/../../common/src/foundation/3danim.cxx \
	$(top_srcdir)/../../common/src/foundation/3dchar.cxx \
	$(top_srcdir)/../../common/src/foundation/MleActor.cxx \
	$(top_srcdir)/../../common/src/foundation/MleActorClass.cxx \
	$(top_srcdir)/../../common/src/foundation/MleDirector.cxx \
	$(top_srcdir)/../../common/src/foundation/MleDso.cxx \
	$(top_srcdir)/../../common/src/foundation/MleEvent.cxx \
	$(top_srcdir)/../../common/src/foundation/MleEventDispatcher.cxx \
	$(top_srcdir)/../../common/src/foundation/MleGroup.cxx \
	$(top_srcdir)/../../common/src/foundation/MleGroupClass.cxx \
	$(top_srcdir)/../../common/src/foundation/MleLoad.cxx \
	$(top_srcdir)/../../common/src/foundation/MleMediaRef.cxx \
	$(top_srcdir)/../../common/src/foundation/MleMediaRefClass.cxx \
	$(top_srcdir)/../../common/src/foundation/MleMonitor.cxx \
	$(top_srcdir)/../../common/src/foundation/MleNotifier.cxx \
	$(top_srcdir)/../../common/src/foundation/MleObject.cxx \
	$(top_srcdir)/../../common/src/foundation/MlePq.cxx \
	$(top_srcdir)/../../common/src/foundation/MlePtrArray.cxx \
	$(top_srcdir)/../../common/src/foundation/MlePtrDict.cxx \
	$(top_srcdir)/../../common/src/foundation/MleRole.cxx \
	$(top_srcdir)/../../common/src/foundation/MleRoleClass.cxx \
	$(top_srcdir)/../../common/src/foundation/MleScene.cxx \
	$(top_srcdir)/../../common/src/foundation/MleSceneClass.cxx \
	$(top_srcdir)/../../common/src/foundation/MleScheduler.cxx \
	$(top_srcdir)/../../common/src/foundation/MleSet.cxx \
	$(top_srcdir)/../../common/src/foundation/MleSetClass.cxx \
	$(top_srcdir)/../../common/src/foundation/MleStage.cxx \
	$(top_srcdir)/../../common/src/foundation/MleStageClass.cxx \
	$(top_srcdir)/../../common/src/foundation/loadSet.cxx \
	$(top_srcdir)/../../common/src/foundation/loaddwp.cxx \
	$(top_srcdir)/../../common/src/input/MleKeyboardPolled.cxx \
	$(top_srcdir)/../../../util/common/src/mlDebug.c \
	$(top_srcdir)/../../../util/common/src/mlErrno.c \
	$(top_srcdir)/../../../util/common/src/mlLogFile.c \
	$(top_srcdir)/../../../util/common/src/mlExpandFilename.c

# Linker options libTestProgram
libmlert_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlert_la_CPPFLAGS = \
	-DMLE_NOT_DLL \
	-DMLE_INTERNAL \
	-DMLE_REHEARSAL \
	-DMLERT_REHEARSAL_EXPORTS \
	-DML_MATH_DEBUG=0 \
	-DML_FIXED_POINT=0 \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../common/src/foundation \
	-I$(top_srcdir)/../../common/src/input \
	-I$(top_srcdir)/../../common/include \
	-I$(top_srcdir)/../../rehearsal/include \
	-I$(top_srcdir)/../../../util/common/include \
	-I$(top_srcdir)/../../../util/linux/include \
	-I$(top_srcdir)/../../../math/common/include \
	-I$(top_srcdir)/../../../math/linux/include \
	-I$(top_srcdir)/../../../../DigitalWorkprint/lib/common/include \
	-I$(top_srcdir)/../../../../DigitalWorkprint/lib/linux/include \
	-I$(top_srcdir)/../../../../DigitalPlayprint/runtime/common/include \
	-I$(top_srcdir)/../../../../DigitalPlayprint/runtime/linux/include \
	-I$(top_srcdir)/../../../../ATK/common/include \
	-I$(top_srcdir)/../../../../ATK/linux/include
