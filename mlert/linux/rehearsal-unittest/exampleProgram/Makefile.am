#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#bin_PROGRAMS=exampleProgram

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=exampleProgram

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

ACLOCAL_AMFLAGS=-I ../m4

# Sources for the a.out 
exampleProgram_SOURCES = exampleProgram.cxx

# Libraries for a.out
exampleProgram_LDADD = $(top_srcdir)/libmlerttest/libmlerttest.la \
	$(top_srcdir)/../rehearsal/libmlert/libmlert.la \
	/opt/MagicLantern/lib/libDWP.a \
	/opt/MagicLantern/lib/libmlutil.a \
	/opt/MagicLantern/lib/libmlmath.a \
	$(GTEST_DIR)/googletest/libgtest.a -lX11 -ldl

# Linker options for a.out
exampleProgram_LDFLAGS = \
    -rpath `cd $(top_srcdir);pwd`/libmlerttest/.libs -pthread

# Compiler options for a.out
exampleProgram_CPPFLAGS = \
	-DML_MATH_DEBUG=0 \
	-DML_FIXED_POINT=0 \
	-DMLE_REHEARSAL \
	-I$(top_srcdir)/include \
	-I/opt/MagicLantern/include \
	-I$(GTEST_DIR)/googletest/include