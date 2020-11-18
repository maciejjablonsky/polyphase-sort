del /f/q ..\build-polyphase-sort\*
rmdir /q/s ..\build-polyphase-sort
mkdir ..\build-polyphase-sort
cd ..\build-polyphase-sort
cmake ..\polyphase-sort -DTESTING=1
rem start cmake --build . -j --config Release
rem start cmake --build . -j --config Debug
start polyphase_sort.sln
cd ..\polyphase-sort
code .
