pinyin-translator
=================

Pinyin to chinese translator with dictionary.

Want to write Chinese but really can't wrap your head around the chinese characters? Not to worry! 
Pinyin-translator builds a local database of words and provides a quick tool to mechanically
convert pinyin into traditional chinese as you type. 

![Screenshot](https://raw.githubusercontent.com/ensisoft/pinyin-translator/master/screens/screenshot.png "Main application window")

The application incorporates a dictionary of over 110k words and a frequency table of over 9k entries.

![Screenshot](https://raw.githubusercontent.com/ensisoft/pinyin-translator/master/screens/dictionary.png "Dictionary window")

Running on Windows
------------------

1. unzip translator.zip
2. install the vcredist_x86.exe package to install runtime libraries.
3. run dist/pinyin-translator.exe


Running on Linux
-----------------

1. Extract the translator.tar.gz
2. run dist/translator.sh

Building from source for LInux
------------------------------

1. Install and prepare boost.build

```
$ wget http://sourceforge.net/projects/boost/files/boost/1.51.0/boost_1_51_0.zip/download boost_1_51_0.zip
$ unzip boost_1_51_0.zip
$ cd boost_1_51_0/tools/build/v2
$ ./bootstrap.sh
$ sudo ./b2 install 
$ bjam --version
  Boost.build.2011.12-svn
```

2. Download and extract Qt everywhere and build it. 
   Note that you must have XRender and fontconfig for nice looking font rendering in Qt.
   
```
$ tar -zxvvf qt-everywhere-opensource-src-4.8.2.tar.gz
$ cd qt-everywhere-opensource-src-4.8.2
$ ./configure --prefix=../qt-4.8.2 --no-qt3support --no-webkit
$ make
$ make install
```

3. Build pinyin-translator

```
$ bjam 
```

Building from source for Windows
---------------------------------

todo:

