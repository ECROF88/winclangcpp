set -e
#clear test.exe
export CXX=clang++
pwd
cd /c/Users/D1mo/Desktop/winclangcpp/src/LR/
rm -rf build
cmake . -B build -G Ninja 
Ninja -C build
./build/Singleton_pattern
