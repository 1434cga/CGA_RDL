#!/bin/sh

# $1 : TEMPLATE BASE DIRECTORY  (OUTPUT/stc)
# $2 : WORKING  BASE DIRECTORY  (test_recover/working)
# $3 : MERGED   BASE DIRECTORY  (test_recover/merged)

echo "help : $0 [template base dir] [working(modified) base dir] [merged(output) dir]"

if [ -z "$1" ]; then
    exit 1
fi

if [ -z "$2" ]; then
    exit 1
fi

if [ -z "$3" ]; then
    exit 1
fi

FILE="/home/vivek/lighttpd.tar.gz"
basename "$FILE"
f="$(basename -- $FILE)"
echo "result : $f"


file="/home/vivek/.gpass/passwd.enc"
basename $file 
echo "result : ${file##*/}"

file="/home/vivek/.gpass/passwd.enc"
basename $file .enc
## without using basename ##
t="${t%.enc}"
echo "$t"

_self="${0##*/}"
echo "$_self is called"

FILE="/home/vivek/lighttpd.tar.gz"
echo "${FILE#*.}"     # print tar.gz
echo "${FILE##*.}"    # print gz
ext="${FILE#*.}"      # store output in a shell variable 
echo "$FILE has $ext" # display it

echo ${FILE} | grep -o '[^/]*$'
echo "this ${FILE}" | grep -o '^[^/]*$'
pwd | awk -F / '{print $NF}'

DIR=$(dirname "${FILE}")
echo "directory ${DIR}"


EXE_DIR=$(dirname $0)
echo "Execution directory ${EXE_DIR}"
if [ -z "${EXE_DIR}" ]; then
    EXE_DIR="."
fi

tput setaf 3
echo "TEMPLATE BASE Dir : $1"
echo "WORKING  BASE Dir : $2"
echo "MERGED RESULT Dir : $3"
tput sgr0

# tput value
## 0    black     COLOR_BLACK     0,0,0
## 1    red       COLOR_RED       1,0,0
## 2    green     COLOR_GREEN     0,1,0
## 3    yellow    COLOR_YELLOW    1,1,0
## 4    blue      COLOR_BLUE      0,0,1
## 5    magenta   COLOR_MAGENTA   1,0,1
## 6    cyan      COLOR_CYAN      0,1,1
## 7    white     COLOR_WHITE     1,1,1
## sgr0 Reset text format to the terminal's default

#exit 0

cdir=`pwd`

echo "==== cp -r ${cdir}/$2   ${cdir}/$3" 
cp -r "${cdir}/$2"   "${cdir}/$3" 

cd $1
for i in `find . -type f -print`
do
    echo ${cdir}
    cd ${cdir}
    tput setaf 3
    echo "#### filename : $i ####"
    tput setaf 4
    if [ -e "${cdir}/$2/${i}" ]
    then
        echo "perl ${EXE_DIR}/3_recover.pl --template=${cdir}/$1/${i} --working=${cdir}/$2/${i} --merge=${cdir}/$3/${i}"
        tput sgr0
        perl ${EXE_DIR}/3_recover.pl --template=${cdir}/$1/${i} --working=${cdir}/$2/${i} --merge=${cdir}/$3/${i}
        #perl 3_recover.pl --template=./OUTPUT/stc/src/2_example.cpp --working=./3_working.cpp.data --merge=./c/d/a.cpp
    else
        echo "cp -f ${cdir}/$1/${i}   ${cdir}/$3/${i}" 
        cp -f "${cdir}/$1/${i}" "${cdir}/$3/${i}" 
    fi
done
cd ..
