del /f/q build\*
rmdir /q/s build
mkdir build
cd build
cmake .. -DTESTING=1
rem start cmake --build . -j --config Release
rem start cmake --build . -j --config Debug
start polyphase_sort.sln
code ..
cd ..
