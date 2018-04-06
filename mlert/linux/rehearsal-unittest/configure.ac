dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(libmlert-rehearsal-test, 1.0)
AC_PROG_CC
AC_PROG_CXX

AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIR([m4])

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl This allows us to specify individual liking flags for each target
AM_PROG_CC_C_O 

dnl Add variable for Google Test location
AC_SUBST([GTEST_DIR])
AC_ARG_VAR(GTEST_DIR, Google Test installation directory)

dnl Initialize Libtool
LT_INIT

AC_CONFIG_FILES(Makefile
                exampleProgram/Makefile
                libmlerttest/Makefile
                include/Makefile)
AC_OUTPUT