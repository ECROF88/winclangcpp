set -e

cmake . -B build -G Ninja
Ninja -C build
./build/test
