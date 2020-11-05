rmdir /q/s build
mkdir build
cd build
cmake ..
start cmake --build . -j --config Release
start cmake --build . -j --config Debug
start polyphase_sort.sln
cd ..
