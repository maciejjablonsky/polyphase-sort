rmdir /q/s build
mkdir build
cd build
cmake .. -DTESTING=1
start cmake --build . -j --config Release
start cmake --build . -j --config Debug
start polyphase_sort.sln
start code ..
cd ..
