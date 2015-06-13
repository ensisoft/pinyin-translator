
#!/bin/bash

clear

# allow unlimited core files to be written
ulimit -c unlimited

# set core file notation
#echo "%e_core" > /proc/sys/kernel/core_pattern

if [ "$0" == "./translator.sh" ]; then
    export LD_LIBRARY_PATH=`pwd`
    exec "./pinyin-translator" "$@"
else
    cwd=`pwd`
    path=`echo $0 | sed s/translator.sh//`
    cd "$path"
    export LD_LIBRARY_PATH=`pwd`
    exec "./pinyin-translator" "$@"
fi
