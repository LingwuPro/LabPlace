@echo off  
:loop1 
    3369rand.exe > data.in
    AVL.exe < data.in > AVL.out
    3369model.exe < data.in > std.out
    fc AVL.out std.out 
if not errorlevel 1 goto loop2  
pause
:loop2
    RBtree.exe < data.in > RBtree.out
    fc RBtree.out std.out
if not errorlevel 1 goto loop1
pause
goto loop1
