rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
cd ..

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes build/writernote
