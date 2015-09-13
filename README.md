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

1. Install the dev packages

```
$ sudo apt-get install qt4-dev-tools
```

2. 
```
$ qmake
$ make
```

Building from source for Windows
---------------------------------

todo:

