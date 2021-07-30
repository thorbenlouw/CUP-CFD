#!/bin/bash

if [ "$COMPILER" = "gnu" ]; then
	export CC=mpicc
	export CXX=mpic++
elif [ "$COMPILER" = "intel" ]; then
	export CC=mpiicc
	export CXX=mpicxx
elif [ "$COMPILER" = "clang" ]; then
    if [ $(clang++ --version | grep 'apple' | wc -l) -gt 0 ]; then
        export CC=mpicc
        export CXX=mpicxx
    else
        export CC="mpicc -cc=clang"
	    export CXX="mpicxx -cxx=clang++"
    fi;
else
	## Default to GNU, everything has that, right?
	COMPILER=gnu
	export CC=mpicc
	export CXX=mpic++
fi

set -e
set -u

## 3rd-party libraries:
HDF5_ROOT="$HDF5_INSTALL_PATH"
METIS_ROOT="$METIS_INSTALL_PATH"
PARMETIS_ROOT="$PARMETIS_INSTALL_PATH"
PETSC_ROOT="$PETSC_INSTALL_PATH"
SQLITE_ROOT="$SQLITE_INSTALL_PATH"

## Link to treetimer library:
TREETIMER_ROOT="$TREETIMER_PATH"

## Optional: clean previous build files:
# rm -rf build

mkdir -p build

# Compile!
# RTYPE=Release
RTYPE=Debug
cd build
cmake .. -DHDF5_ROOT="$HDF5_ROOT" \
         -DCMAKE_BUILD_TYPE="$RTYPE" -DCOMPILER="$COMPILER" \
         -DMETIS_ROOT="$METIS_ROOT" \
         -DPARMETIS_ROOT="$PARMETIS_ROOT" \
		 -DSQLITE_ROOT="$SQLITE_ROOT" \
		 -DPETSC_ROOT="$PETSC_ROOT" \
		 -DTREETIMER_ROOT="$TREETIMER_ROOT" \
		 -DUSE_UNIT_TESTS=OFF

# make VERBOSE=1 -j`nproc`
make -j`nproc`

