rmdir /q/s build
mkdir build
cd build
cmake ..
start cmake --build . -j
start polyphase_sort.sln
cd ..
