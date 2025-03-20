#######################################
# The directory for object files and libraries of object code
libdir = $(exec_prefix)/lib/mle/runtime

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
	$(top_srcdir)/../../common/src/foundation/MleActorGC.cxx \
	$(top_srcdir)/../../common/src/foundation/MleDirector.cxx \
	$(top_srcdir)/../../common/src/foundation/MleDso.cxx \
	$(top_srcdir)/../../common/src/foundation/MleEvent.cxx \
	$(top_srcdir)/../../common/src/foundation/MleEventDispatcher.cxx \
	$(top_srcdir)/../../common/src/foundation/MleGroup.cxx \
	$(top_srcdir)/../../common/src/foundation/MleLoad.cxx \
	$(top_srcdir)/../../common/src/foundation/MleMediaRef.cxx \
	$(top_srcdir)/../../common/src/foundation/MleMonitor.cxx \
	$(top_srcdir)/../../common/src/foundation/MleNotifier.cxx \
	$(top_srcdir)/../../common/src/foundation/MleObject.cxx \
	$(top_srcdir)/../../common/src/foundation/MlePq.cxx \
	$(top_srcdir)/../../common/src/foundation/MlePtrArray.cxx \
	$(top_srcdir)/../../common/src/foundation/MlePtrDict.cxx \
	$(top_srcdir)/../../common/src/foundation/MleRole.cxx \
	$(top_srcdir)/../../common/src/foundation/MleScene.cxx \
	$(top_srcdir)/../../common/src/foundation/MleScheduler.cxx \
	$(top_srcdir)/../../common/src/foundation/MleSet.cxx \
	$(top_srcdir)/../../common/src/foundation/MleStage.cxx \
	$(top_srcdir)/../../common/src/input/MleKeyboardPolled.cxx

# Linker options libTestProgram
libmlert_la_LDFLAGS = -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlert_la_CPPFLAGS = \
	-DMLE_NOT_UTIL_DLL \
	-DMLE_NOT_MATH_DLL \
	-DMLE_NOT_DWP_DLL \
	-DMLE_NOT_RUNTIME_DLL \
	-DMLE_INTERNAL \
	-DMLE_DIGITAL_PLAYPRINT \
	-DML_MATH_DEBUG=0 \
	-DML_FIXED_POINT=0 \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../common/src/foundation \
	-I$(top_srcdir)/../../common/src/input \
	-I$(top_srcdir)/../../common/include \
	-I$(top_srcdir)/../../linux/include \
	-I$(MLE_ROOT)/include \
	-O0
