# Aura Avionics
A fly-by-wire system for RC airplanes of all shapes and sizes


Aura Avionics is a program which displays air data about RC airplanes on a digital instrument panel. Attitude, altitude, vertical speed, everything.
Not only does Aura manage displaying data, it also manages communication between the airplane and the software through a proprietary messaging protocol.
Not every plane is as advanced as every other, so the airplane reports its instrumentation capabilities during the pairing process. This allows Aura to adapt its display to each airplane's instrumentation capabilities and optimize the information displayed. Aura also supports a camera feed to be displayed behind the telemetry.


Aura is written in C++ and uses GLUT to display the graphics on screen.
