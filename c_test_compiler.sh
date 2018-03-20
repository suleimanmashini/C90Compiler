#!/bin/bash

if [[ "$1" != "" ]] ; then
    compiler="$1"
else
    compiler="bin/c_compiler"
fi

have_compiler=0
if [[ ! -f bin/c_compiler ]] ; then
    >&2 echo "Warning : cannot find compiler at path ${compiler}. Only checking C reference against python reference."
    have_compiler=1
fi

input_dir="test_deliverable/test_cases"

working="tmp/compiler_tests"
mkdir -p ${working}

for i in ${input_dir}/*.c ; do
  if [[ ${i: -9} != "_driver.c" ]] ; then
    base=$(echo $i | sed -E -e "s|${input_dir}/([^.]+)[.]c|\1|g");
    # Compile the driver
    mips-linux-gnu-gcc -S -static -std=c90 -march=mips32 ${input_dir}/${base}_driver.c -o ${working}/${base}_driver.s

    # Compiler using the Compiler
    if [[ ${have_compiler} -eq 0 ]] ; then
        # Create the DUT python version by invoking the compiler with translation flags
        $compiler -S ${input_dir}/${base}.c -o ${working}/${base}_got.s
    fi

    #Set up the linking process of the two files:

    mips-linux-gnu-gcc -static -march=mips32 ${working}/${base}_driver.s ${working}/${base}_got.s -o ${working}/${base}_test

    #get the value thats returned

    qemu-mips ${working}/${base}_test
    GOT_C_OUT=$?

    if [[ ${have_compiler} -ne 0 ]] ; then
        echo "$base, Fail, No C compiler/translator"
    elif [[ 0 -ne $GOT_C_OUT ]] ; then
        echo "$base, Fail, Expected ${REF_C_OUT}, got ${GOT_P_OUT}"
    else
        echo "$base, Pass"
    fi
  fi
done
