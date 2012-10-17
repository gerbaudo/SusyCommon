#!/bin/bash

# Settings
tag=n0107
nickname=Steve
nGBPerJob=MAX
athenaTag=17.3.1.1
metFlav="Egamma10NoTau_STVF"
destSE="SLACXRD_SCRATCHDISK"

# get the samples of interest
if [[ $# = 0 ]]; then
        echo "submit all samples"
        pattern="data"
else
        pattern=$1
fi

# User blacklist of sites
blackList=`cat blacklist.txt`

# Parse samples
datasets=(`cat dataSamples.txt | grep $pattern`)
echo "${#datasets[@]} datasets"


# Loop over datasets
for inDS in ${datasets[@]}; do

	# get sample name from input dataset name
        sample=${inDS#*phys-susy.}
        sample=${sample%.PhysCont.*}
        sample=${sample/physics_/}

	# final output ds name
	#outDS="user.Steve.$sample.SusyNt.$iteration/"
        outDS="user.$nickname.${inDS%/}_$tag/"
        outDS=${outDS/NTUP_SUSY/SusyNt}
        outDS=${outDS/SKIM/}

        command="./gridScript.sh %IN --saveTau -s $sample --metFlav $metFlav"

	echo 
        echo "__________________________________________________________________________________________________________"
	echo "INPUT    $inDS"
	echo "OUTPUT   $outDS"
        echo "sample   $sample"
        echo "command  $command"
	

	# prun command
	prun --exec "$command" --useRootCore --tmpDir /tmp --inTarBall=area.tar \
             --extFile '*.so,*.root' --match "*root*" --outputs "susyNt.root" \
             --destSE=$destSE \
             --excludedSite=$blackList \
             --nGBPerJob=$nGBPerJob \
             --athenaTag=$athenaTag \
	     --inDS  $inDS \
	     --outDS $outDS

done
