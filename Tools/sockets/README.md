Trying to make a util like synergy

mousepos_win =-----> windows, VS 2012, exe to emit mouse pos with page detect acts as setver
CppSockets/examples/readin.cpp -----> mac version to consume pos and set mousepos on mac client
readin.py --> python prototype
uses Udp port 7301

status as on 8 Oct 22
====================
 mouseposewin emits udp data with 1ms intercal, mousepos_mac/readin on mac consumes the data
looks to function well, no visibel latency on wifi , however cursor jumps here and there (debug it)

Also the CppSockets is a cross platform socket class library, it can be incorporated in AlgeSDK to add TCP/UDP support
to make future apps have tcp/udp support.

determine if there are other solutions, can we sell it
synergy is the defacto standard, how can we provide a better alternative
