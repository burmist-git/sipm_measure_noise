#!/bin/bash

#Or analyse single root file
inRootFileDir="../data/S13360_3075CN_UVE_sn2_INFNfastAmpl_crosstalk/root/"
rootFilesInfo="rootFilesInfo.dat"
inRootFiles="../data/S13360_3075CN_UVE_sn2_INFNfastAmpl_crosstalk/root/53.000_V_T_23.471_C.txt.root"
outHistSingleF="./hist.root"

function compile {
    make -f Makefilewfsimp clean; make -f Makefilewfsimp runwfsimp;
}

function printHelp {
    echo " --> ERROR in input arguments "
    echo " [0] -g  : single root file"
    echo " [0] -c  : compile"
    echo " [0] -d  : single root file"
    echo " [0] -m  : print meta data"
    echo " [0] -h  : print help"
}

function getRootFilesInfo {
    outfile=$inRootFileDir$rootFilesInfo
    echo "rootFilesInfo = $outfile"
    rm -rf $outfile
    echo "fileDir:     $inRootFileDir" >> $outfile
    for filename in `(ls -lrt $inRootFileDir/*.root | awk '{print $9}')`; do
	filename=`basename $filename`
	vbias=${filename:0:6}
	temperature=${filename:11:6}
	echo "filename:    $filename" >> $outfile
	echo "vbias:       $vbias" >> $outfile
	echo "temperature: $temperature" >> $outfile
	#./runwfsimp 2 $inRootFileDir/$filename
    done
}

if [ $# -eq 0 ] 
then    
    printHelp
else
    if [ "$1" = "-d" ]; then
	./runwfsimp 1 $inRootFiles $outHistSingleF
    elif [ "$1" = "-c" ]; then
	compile
    elif [ "$1" = "-g" ]; then
	getRootFilesInfo
    elif [ "$1" = "-m" ]; then
	./runwfsimp 2 $inRootFiles
    elif [ "$1" = "-h" ]; then
        printHelp
    else
        printHelp
    fi
fi

#espeak "I have done"
