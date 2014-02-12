#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([srsotr], [0.0.0], [van.fisted@googlemail.com])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/srsotr.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h syslog.h unistd.h])

AC_CHECK_LIB([srsbsns], [ringbuf_init], [], [AC_MSG_ERROR([libsrsbsns not installed?])])
AC_CHECK_LIB([otr], [otrl_version], [], [AC_MSG_ERROR([libotr not installed?])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([select strchr strdup strtoul])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

echo \
"-------------------------------------------------

 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}

 Prefix: '${prefix}'.
 Compiler: '${CC} ${CFLAGS}'
 Libraries: '${LIBS}'

--------------------------------------------------"
