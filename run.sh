set -e
#clear test.exe
rm -rf build
cmake . -B build -G Ninja 
Ninja -C build
./build/test



