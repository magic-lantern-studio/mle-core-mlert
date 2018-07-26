#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmlerttest.la

#######################################
# Build information for each library

# Sources for libmlerttest
libmlerttest_la_SOURCES = libmlerttest.cxx TestActor.cxx testMleActor.cxx rtestubs.cxx

# Linker options libTestProgram
libmlerttest_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlerttest_la_CPPFLAGS = \
	-DMLE_DIGITAL_PLAYPRINT \
	-DMLE_NOT_DLL \
	-DMLE_INTERNAL \
	-DML_MATH_DEBUG=0 \
	-DML_FIXED_POINT=0 \
	-I$(top_srcdir)/include \
	-I$(MLE_ROOT)/include \
	-I$(GTEST_DIR)/include
