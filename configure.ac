AC_INIT([elgtkdrive], [0.4], [peter.harlow@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])


# use autoreconf --install after change

AC_PROG_CC


AM_PATH_GTK_2_0([2.4.0])
PKG_CHECK_MODULES(GTK, gdk-2.0 gtk+-2.0, sqlite3, exit)
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)


AC_CONFIG_FILES([
Makefile
])
AC_OUTPUT
