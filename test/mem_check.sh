#rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
cd ..

rm result_memcheck.txt

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file="result_memcheck.txt" build/writernote
gedit result_memcheck.txt &
