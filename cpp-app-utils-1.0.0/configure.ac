dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(cpp application utils, 1.1)

AM_SILENT_RULES([yes])

LT_INIT
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])
AC_PROG_CXX

PKG_CHECK_MODULES([GIO],      [gio-2.0                    >= 2.30.0])
PKG_CHECK_MODULES([GIO_UNIX], [gio-unix-2.0               >= 2.30.0])
PKG_CHECK_MODULES([GLIB],     [glib-2.0                   >= 2.30.0])
PKG_CHECK_MODULES([GOBJECT],  [gobject-2.0                >= 2.30.0])

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
