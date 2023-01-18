rm -rf build
mkdir build
cd build
cmake .. -G Xcode \
	-DPDFSUPPORT=OFF \
	-DCMAKE_BUILD_TYPE=Debug 
#\
	#-DCMAKE_CXX_COMPILER=/usr/bin/clang++
