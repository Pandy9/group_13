#!/usr/bin/env bash

# programmet du vill mäta
CMD="./pearson ./data/1024.data test.txt"

# temporär fil där /usr/bin/time skriver
TMP=$(mktemp)

runs=0
sum_time=0
sum_cpu=0
sum_mem=0

# konverterar 0:01.23 eller 1:02:03 till sekunder
to_seconds() {
    local t="$1"
    if [[ "$t" == *:* ]]; then
        local parts=(${t//:/ })
        if (( ${#parts[@]} == 2 )); then
            # MM:SS.xx
            awk -v m="${parts[0]}" -v s="${parts[1]}" 'BEGIN{print m*60 + s}'
        else
            # HH:MM:SS
            awk -v h="${parts[0]}" -v m="${parts[1]}" -v s="${parts[2]}" 'BEGIN{print h*3600 + m*60 + s}'
        fi
    else
        # bara sekunder
        echo "$t"
    fi
}

while [[ $runs -lt 10 ]]; do
    # kör programmet, släng programmets stdout, men låt time skriva till TMP
    /usr/bin/time -f "Time: %E\nCPU: %P\nMaxMem: %M" -o "$TMP" --append $CMD > /dev/null

    # ta de SISTA 3 raderna som skrevs (Time, CPU, MaxMem)
    tail3=$(tail -n 3 "$TMP")

    time_str=$(echo "$tail3" | awk -F': ' '/^Time:/ {print $2}')
    cpu_str=$(echo "$tail3"  | awk -F': ' '/^CPU:/ {print $2}')
    mem_kb=$(echo "$tail3"   | awk -F': ' '/^MaxMem:/ {print $2}')

    # om någon är tom → då fick vi inte det vi ville, kör om
    if [[ -z "$time_str" || -z "$cpu_str" || -z "$mem_kb" ]]; then
        echo "Fick konstig output, försöker igen..."
        continue
    fi

    # CPU-värdet ser ut som "99%" → ta bort %
    cpu_val=${cpu_str%\%}
    cpu_int=${cpu_val%.*}

    # om CPU > 100 → ogiltig
    if (( cpu_int > 100 )); then
        echo "Körning ogiltig (CPU=$cpu_val%). Kör om..."
        continue
    fi

    # konvertera tiden till sekunder för medel
    time_sec=$(to_seconds "$time_str")

    # summera
    sum_time=$(awk -v a="$sum_time" -v b="$time_sec" 'BEGIN{printf "%.6f", a+b}')
    sum_cpu=$(awk -v a="$sum_cpu" -v b="$cpu_val"  'BEGIN{printf "%.6f", a+b}')
    sum_mem=$(awk -v a="$sum_mem" -v b="$mem_kb"   'BEGIN{printf "%.6f", a+b}')

    runs=$((runs + 1))

    # skriv ut direkt
    echo "$runs - $time_str (${time_sec}s)  CPU=${cpu_val}%  MEM=${mem_kb}KB"
done

# räkna medel
avg_time=$(awk -v s="$sum_time" -v n=10 'BEGIN{printf "%.6f", s/n}')
avg_cpu=$(awk -v s="$sum_cpu" -v n=10 'BEGIN{printf "%.2f", s/n}')
avg_mem=$(awk -v s="$sum_mem" -v n=10 'BEGIN{printf "%.2f", s/n}')

echo
echo "================ RESULTAT ================"
echo "Medeltid (väggklocka): ${avg_time} s"
echo "Medel CPU:             ${avg_cpu} %"
echo "Medel MaxMem:          ${avg_mem} KB"

# städa
rm -f "$TMP"
