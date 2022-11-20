@echo off  
:loop 
    3391rand.exe > data.in
    splay.exe < data.in > splay.out
    3391model.exe < data.in > std.out
    fc splay.out std.out 
if not errorlevel 1 goto loop
pause