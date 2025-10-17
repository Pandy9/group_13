#!/bin/bash

echo "NOTE: this script relies on the binaries blur and blur_par to exist"

status=0
red=$(tput setaf 1)
reset=$(tput sgr0)

for thread in 1 2 4 8 16 32
do
    for image in im1 im2 im3 im4
    do
        ./blur_par 15 "data/$image.ppm" "./data_o/blur_${image}_par.ppm" $thread

        if ! cmp -s "./data_o/blur_${image}.ppm" "./data_o/blur_${image}_par.ppm"
        then
            echo "${red}Error: Incongruent output data detected when blurring image $image.ppm with $thread thread(s)${reset}"
            status=1
        fi

        rm "./data_o/blur_${image}_par.ppm"
    done
done

exit $status