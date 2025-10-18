#!/bin/bash

status=0
if tput setaf 1 &> /dev/null; then
    red=$(tput setaf 1)
    yellow=$(tput setaf 3)
    green=$(tput setaf 2)
    reset=$(tput sgr0)
else
    # If tput fails, fall back to no color
    red=""
    yellow=""
    green=""
    reset=""
fi

counter=0
errors_found=0
warnings_found=0

# Run pearson to generate sequential output
./pearson "data/128.data" "./data_o/128_seq.data"
./pearson "data/256.data" "./data_o/256_seq.data"
./pearson "data/512.data" "./data_o/512_seq.data"
./pearson "data/1024.data" "./data_o/1024_seq.data"

for thread in 2 4 8 16 32
do
    for size in 128 256 512 1024
    do
        # Run the parallel version
        ./pearson_par "data/$size.data" "./data_o/${size}_par.data" $thread

        # Run the verify script and capture the return code
        ./verify "./data_o/${size}_seq.data" "./data_o/${size}_par.data"
        ret=$?

        # Check the return code and print corresponding message
        if [ $ret -eq 2 ]; then
            echo "${red}ERROR: Significant mismatch found in size ${size} with ${thread} thread(s).${reset}"
            errors_found=1
        elif [ $ret -eq 1 ]; then
            echo "${yellow}WARNING: Minor differences found in size ${size} with ${thread} thread(s).${reset}"
            warnings_found=1
        elif [ $ret -eq 0 ]; then
            echo "${green}Success: Files match for size ${size} with ${thread} thread(s).${reset}"
        else
            echo "${red}ERROR: An unexpected error occurred while processing size ${size} with ${thread} thread(s).${reset}"
            errors_found=1
        fi

        # Clean up
        rm "./data_o/${size}_par.data"
    done
done

# Final output based on results
if [ $errors_found -eq 1 ]; then
    echo "${red}Errors found during the tests.${reset}"
    status=1
elif [ $warnings_found -eq 1 ]; then
    echo "${yellow}Warnings were found, but no major errors.${reset}"
else
    echo "${green}Success: All tests passed successfully.${reset}"
fi

exit $status