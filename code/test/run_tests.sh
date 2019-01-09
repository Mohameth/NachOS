#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[1;33m'
NC='\033[0m' # No Color

#folder location
BUILDFolder='../build'
INFOLDER='./testInput'
OUTFOLDER='./testOutput'
IN='_in'
OUTEMP='tmp_out_'
OUT='_out'

nachosCmd="./nachos-userprog -x"
TESTFILES=( );

folderfilenames=`ls $BUILDFolder/test_*`
totSuccess=0
totTestFiles=0

displayFinalRes (){
   echo "==RESULT: $totSuccess/$totTestFiles =="
}

runTest () {
   testFileName=$1
   inFile="$INFOLDER/$testFileName$IN"
   outFileTmp="./$OUTEMP$testFileName"
   ExpectedoutFile="$OUTFOLDER/$testFileName$OUT"
   #outFileTmp=$ExpectedoutFile #set to addOutputfile
   hasInput=1

   if [ ! -f $inFile ]; then
      echo -e -n "${ORANGE}No input file- ${NC}"
      hasInput=0
   fi

   if [ ! -f $ExpectedoutFile ]; then
      echo -e "${RED}Missing expected output file:${NC}"
      echo $ExpectedoutFile
      exit #remove to add ouputfile
   fi


   if [ $hasInput -eq 1 ]; then
      $($BUILDFolder/$nachosCmd $BUILDFolder/$f < $inFile > $outFileTmp)
   else
      $($BUILDFolder/$nachosCmd $BUILDFolder/$f > $outFileTmp)
   fi

   var=$(diff $outFileTmp $ExpectedoutFile)
   if [ -z "$var" ]; then
      echo -e "${GREEN}SUCCESS${NC}"
      ((totSuccess++))
   else
      echo -e "${RED}FAIL${NC}"
   fi


}



for eachfile in $folderfilenames
do
   file="${eachfile%.*}"
   file="${file:9}"
   if [ "$file" != "" ]; then
      ((totTestFiles++))
      TESTFILES=("${TESTFILES[*]}" "$file")
   fi
done

echo -e "RUNNING ALL TESTS\n"


echo -e 'File \t\t\t      |   Status'
echo '------------------------------+---------'
for f in ${TESTFILES[*]}
do
echo -n -e "Running: $f: \t"
runTest $f
done

displayFinalRes
$(rm $OUTEMP*)