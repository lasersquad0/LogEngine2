AUTOMAKE_OPTIONS = subdir-objects

noinst_PROGRAMS = testLogEngineApp
testLogEngineApp_SOURCES = testLine.cpp \
testLogger.cpp \
testRunner.cpp \
testUtils.cpp \
testTStream.cpp \
testIniReader.cpp


testLogEngineApp_CXXFLAGS = -std=c++20 -Wall -Wno-reorder -I../external -I../include -D_TESTMODE
testLogEngineApp_LDFLAGS = -llogengine -lcppunit -L../libs -L../src/.libs -L../external/cppunit/libs

EXTRA_DIST=debug_support.h \
testLine.h \
testLogger.h \
testProperties.h \
testThreadLog.h \
testUtils.h \
testIniReader.h \
testTStream.h \
MyTestProgressListener.h
