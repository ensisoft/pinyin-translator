CONFIG += qt
CONFIG += c++11
CONFIG += debug_and_release

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-unused-parameter


SOURCES = dictionary.cpp \
	dlgdictionary.cpp \
	freqtable.cpp \
	main.cpp \
	mainwindow.cpp \
	qtmain_win.cpp

HEADERS = config.h \
	dictionary.h \
	dlgdictionary.h \
	dlgword.h \
	format.h \
	freqtable.h \
	mainwindow.h \
	pinyin.h \
	warnpop.h \
	warnpush.h
	
FORMS = mainwindow.ui \
	dlgword.ui \
	dlgdictionary.ui

TARGET = pinyin-translator

RESOURCES = resource.qrc


