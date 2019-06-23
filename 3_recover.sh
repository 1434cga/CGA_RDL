#!/bin/sh

# $1 : TEMPLATE BASE DIRECTORY  (OUTPUT/stc)
# $2 : WORKING  BASE DIRECTORY  (test_recover/working)
# $3 : MERGED   BASE DIRECTORY  (test_recover/merged)

echo "$1"
echo "$2"

cdir=`pwd`

cd $1
for i in `find . -type f -print`
do
    echo $i
    echo ${cdir}
    cd ${cdir}
    echo "perl 3_recover.pl --template=${cdir}/$1/${i} --working=${cdir}/$2/${i} --merge=${cdir}/$3/${i}"
    perl 3_recover.pl --template=${cdir}/$1/${i} --working=${cdir}/$2/${i} --merge=${cdir}/$3/${i}
    #perl 3_recover.pl --template=./OUTPUT/stc/src/2_example.cpp --working=./3_working.cpp.data --merge=./c/d/a.cpp
done
cd ..
