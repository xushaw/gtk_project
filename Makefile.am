bin_PROGRAMS = elgtkdrive

AM_CFLAGS = -lsqlite3 @GTK_CFLAGS@ \
			-Wall
			-g

elgtkdrive_LDADD = @GTK_LIBS@ -lsqlite3

#elgtkdrive_LDADD = -Lc:/devel/target/bdc92a580d9d9d9a1c5bb39f11f932a3/lib -Lc:/devel/target/2acfb7540ed51ac5051a9a32e319bc96/lib -Lc:/devel/target/e7ec2c0581824f9466774878c965ec39/lib -Lc:/devel/target/cc06e3789673edb8027d57d473fab0ae/lib -Lc:/devel/target/0ca5583385db1135a53cf9e275815b19/lib -lgtk-win32-2.0 -lgdk-win32-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lpangowin32-1.0 -lgdi32 -lpangocairo-1.0 -lpango-1.0 -lcairo -lgobject-2.0 -lgmodule-2.0 -lglib-2.0 -lintl -lsqlite3dll

elgtkdrive_SOURCES = \
				main.c \
				tab1.c \
				tab2.c \
				gui.c

elgtkdrive_gtk_headers = \
				tab1.h \
				tab2.h \
				gui.h 

