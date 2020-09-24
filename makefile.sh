#!/bin/bash
# compile
xc32-gcc -c -x c -mprocessor=32MZ2048EFM144 -O0 -I "src"  -o "build/release/out/main.o" "src/main.c" -no-legacy-libc -MMD -finline -Wfatal-errors -Winline -Wall

xc32-gcc -mprocessor=32MZ2048EFM144 -O0 -o "build/release/firmware.elf" "build/release/out/main.o" -no-legacy-libc -Wl,--script=\"scripts/app.ld\",--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="build/release/firmware.map"


xc32-bin2hex "build/release/firmware.elf"

xc32-objdump -D "build/release/firmware.elf" > "build/release/dissambly.s"

#read -n1 -r -p "Press space to open dissambly..." key

#if [ "$key" == '' ]
#then
    # Space pressed, do something
    # echo [$key] is empty when SPACE is pressed # uncomment to trace
   #vim src/app_tasks.s

    # Anything else pressed, do whatever else.
    # echo [$key] not empty
#fi

