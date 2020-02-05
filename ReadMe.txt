
Instructionms to set up solution:

TO do this we need to set library paths for openframeworks in two files
1) .sln
2) .vcproject

steps
1) open .sln file with norepad++
2) change the path for "openFrameworksLib"(line 8) to the one where you have installed openframeworks on your machine.
3) now open .vcproject file
4) scroll down and on lines(53, 57, 61 and 65) change the path to the one where you have installed openframeworks on your machine.
5) done. Now open .sln file in visual studio 17

Instruction to run various alogithms:

>> Go to "ofApp.cpp" and inside setup() function (line 30)
>> change the value of sAlog to whichever alorithm you want to run and hit run to play the app