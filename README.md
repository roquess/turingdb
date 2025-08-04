# TuringDB

## Build

Install dependencies by running the dependencies.sh script once:
```bash
./dependencies.sh
```

Build TuringDB:
```bash
mkdir -p build && cd build

cmake ..
make -j8
make install
```

Source setup.sh to get turingdb tools in the PATH:
```bash
source setup.sh
```
