TinyCNC
=======

Simple application for controlling TinyCNC robot built from
PlotterBot. See the following link for more information.

http://plotterbot.com/tiny-cnc/

This is still in the early stages of development but it does work. 

First compile and upload the Arduino sketch bot/bot.ino. Then, run the 
tinyCNC application and try loading the following G-code (from a text file):

G92 X0.00 Y0.00 Z0.00
M300 S30

G1 X0.00 Y100.00 F600
G1 X100.00 Y100.00 F600
G1 X100.00 Y0.00 F600
G1 X0.00 Y0.00 F600

M300 S50

Then hit the buttom "Renderer tinyCNC" to get the bot to render it!

Requirements
============

The simple GUI uses OS X's Cocoa and so unless you want to do some simple
porting working you are going to need OS X.

git clone https://github.com/bgaster/tinyCNC.git tinyCNC
cd tinyCNC
git submodule update --init --recursive

Building
========

I've moved to using XCode just so I can useful the interface builder and so on. So
you are going to need to open tinyCNC/tinyCNC.xcodeproj.


For the Arduino sketch I use Arduino-Makefile to build, which can be found
here:

  https://github.com/sudar/Arduino-Makefile

The C++ code used in the sketch requires C++11 and so you will need to either
modify the Arduino-Makefile to enable C++11 compilaiton or modify the code 
to be compatable with C++98. Both options are straightforward. 
