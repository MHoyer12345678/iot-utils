ACLOCAL_AMFLAGS=-I m4

lib_LIBRARIES = libCppAppUtils.a

libCppAppUtils_a_SOURCES = \
	Logger.cpp			\
	Logger.h			\
	Configuration.cpp	\
	Configuration.h		\
	BasicGPIO.cpp 		\
	GPIOInput.cpp 		\
	GPIOOutput.cpp


libCppAppUtils_a_CPPFLAGS  = \
			-fPIC					\
		    	$(GIO_LIBS_CFLAGS)			\
		    	$(GIO_UNIX_CFLAGS)			\
		    	$(GLIB_CFLAG)				\
		    	$(GOBJECT_CFLAGS)

nobase_include_HEADERS = \
	cpp-app-utils/Logger.h \
	cpp-app-utils/Configuration.h \
	generic-embedded-utils/BasicGPIO.h \
	generic-embedded-utils/GPIOInput.h \
	generic-embedded-utils/GPIOOutput.h
