cd OpenCV-2.3.1/
rm -f CMakeCache.txt
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_PYTHON_SUPPORT=ON ..
make
sudo make install

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib