#!/bin/bash
set -e

# Restrict number of jobs to 4 on macOS to avoid freeze
if [[ "$(uname)" == "Darwin" ]]; then
    NUM_JOBS=4
else
    NUM_JOBS=$(nproc 2>/dev/null || grep -c ^processor /proc/cpuinfo)
fi

SOURCE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DEPENDENCIES_DIR=$SOURCE_DIR/external/dependencies
BUILD_DIR=$DEPENDENCIES_DIR/build

mkdir -p $DEPENDENCIES_DIR
mkdir -p $BUILD_DIR

# Detect package manager (apt-get, dnf or zypper) on Linux only
if [[ "$(uname)" != "Darwin" ]]; then
    if command -v apt-get &> /dev/null; then
        PKG_MANAGER="apt-get"
        PKG_INSTALL="install -qqy"
    elif command -v dnf &> /dev/null; then
        PKG_MANAGER="dnf"
        PKG_INSTALL="install -y"
    elif command -v zypper &> /dev/null; then
        PKG_MANAGER="zypper"
        PKG_INSTALL="--non-interactive install -y"

    else
        echo "Neither apt-get, dnf nor zypper found. Please install dependencies manually."
        exit 1
    fi
    
    # Update package cache
    echo "Updating $PKG_MANAGER cache..."
    if [[ "$PKG_MANAGER" == "zypper" ]]; then
        sudo $PKG_MANAGER --non-interactive refresh
    else
        sudo $PKG_MANAGER update
    fi
fi

# Install curl, openssl, and zlib
if [[ "$(uname)" == "Darwin" ]]; then
    # macOS - use Homebrew
    if ! command -v brew &> /dev/null; then
        echo "Homebrew not found. Please install Homebrew first."
        exit 1
    fi

    if ! brew list curl &> /dev/null; then
        echo "Installing curl via Homebrew..."
        brew install curl
    else
        echo "curl is already installed"
    fi

    if ! brew list openssl@3 &> /dev/null; then
        echo "Installing openssl@3 via Homebrew..."
        brew install openssl@3
    else
        echo "openssl@3 is already installed"
    fi

    if ! brew list zlib &> /dev/null; then
        echo "Installing zlib via Homebrew..."
        brew install zlib
    else
        echo "zlib is already installed"
    fi
else
    # Linux - use detected package manager
    echo "Installing curl, openssl, and zlib via $PKG_MANAGER..."
    if [[ "$PKG_MANAGER" == "zypper" ]]; then
        sudo $PKG_MANAGER $PKG_INSTALL curl libcurl-devel zlib-devel libopenssl-devel
    else
        sudo $PKG_MANAGER $PKG_INSTALL curl libcurl4-openssl-dev zlib1g-dev libssl-dev
    fi
fi

# Install bison and flex
if [[ "$(uname)" == "Darwin" ]]; then
    # macOS - use Homebrew
    if ! brew list bison &> /dev/null; then
        echo "Installing bison via Homebrew..."
        brew install bison
    else
        echo "bison is already installed"
    fi

    if ! brew list flex &> /dev/null; then
        echo "Installing flex via Homebrew..."
        brew install flex
    else
        echo "flex is already installed"
    fi
else
    # Linux - use detected package manager
    echo "Installing bison and flex via $PKG_MANAGER..."
    if [[ "$PKG_MANAGER" == "zypper" ]]; then
        sudo zypper $PKG_INSTALL bison flex
    else
        sudo $PKG_MANAGER $PKG_INSTALL bison flex libfl-dev
    fi
fi

# Install BLAS
if [[ "$(uname)" == "Darwin" ]]; then
    # macOS - use Homebrew
    if ! brew list openblas &> /dev/null; then
        echo "Installing openblas via Homebrew..."
        brew install openblas
    else
        echo "openblas is already installed"
    fi

    # Install libomp for OpenMP support (needed by FAISS)
    if ! brew list libomp &> /dev/null; then
        echo "Installing libomp via Homebrew..."
        brew install libomp
    else
        echo "libomp is already installed"
    fi
else
    # Linux - use detected package manager
    echo "Installing BLAS via $PKG_MANAGER..."
    if [[ "$PKG_MANAGER" == "zypper" ]]; then
        sudo $PKG_MANAGER $PKG_INSTALL openblas-devel
    else
        sudo $PKG_MANAGER $PKG_INSTALL libopenblas-dev
    fi
fi

# Check for required build tools (cmake + C++ compiler)
if [[ "$(uname)" != "Darwin" ]] && [[ "$PKG_MANAGER" == "zypper" ]]; then
    if ! command -v cmake &> /dev/null || (! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null); then
        echo "Installing build tools (cmake + gcc-c++) via zypper..."
        sudo $PKG_MANAGER $PKG_INSTALL cmake gcc-c++
    fi
else
    if ! command -v cmake &> /dev/null; then
        echo "cmake not found. Please install cmake before running this script."
        exit 1
    fi

    if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
        echo "No C++ compiler found (g++ or clang++). Please install one before running this script."
        exit 1
    fi
fi

# Skip building if cache was hit (set by CI)
if [[ "$SKIP_BUILD_IF_CACHED" == "true" ]]; then
    echo "Dependencies cache hit, skipping build"
    exit 0
fi

# Build aws-sdk-cpp
mkdir -p $BUILD_DIR/aws-sdk-cpp
cd $BUILD_DIR/aws-sdk-cpp
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$DEPENDENCIES_DIR -DBUILD_ONLY="s3;s3-crt;ec2" -DENABLE_TESTING=OFF -DBUILD_SHARED_LIBS=OFF $SOURCE_DIR/external/aws-sdk-cpp
cmake --build $BUILD_DIR/aws-sdk-cpp -j $NUM_JOBS
cmake --install $BUILD_DIR/aws-sdk-cpp

# Build faiss
mkdir -p $BUILD_DIR/faiss
cd $BUILD_DIR/faiss

FAISS_CMAKE_ARGS=(
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=$DEPENDENCIES_DIR
    -DBUILD_TESTING=OFF
    -DBUILD_SHARED_LIBS=OFF
    -DFAISS_ENABLE_GPU=OFF
    -DFAISS_ENABLE_CUVS=OFF
    -DFAISS_ENABLE_MKL=OFF
    -DFAISS_ENABLE_PYTHON=OFF
    -DFAISS_ENABLE_EXTRAS=OFF
)

# On macOS, we need to explicitly tell CMake where to find OpenMP from Homebrew's libomp
if [[ "$(uname)" == "Darwin" ]]; then
    LIBOMP_PREFIX=$(brew --prefix libomp)
    FAISS_CMAKE_ARGS+=(
        "-DOpenMP_CXX_FLAGS=-Xpreprocessor -fopenmp -I${LIBOMP_PREFIX}/include"
        -DOpenMP_CXX_LIB_NAMES=omp
        "-DOpenMP_omp_LIBRARY=${LIBOMP_PREFIX}/lib/libomp.dylib"
    )
fi

cmake "${FAISS_CMAKE_ARGS[@]}" $SOURCE_DIR/external/faiss-1.13.1
cmake --build $BUILD_DIR/faiss -j $NUM_JOBS
cmake --install $BUILD_DIR/faiss
