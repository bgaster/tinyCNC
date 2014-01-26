TinyCNC
=======

Simple application for controlling TinyCNC robot built from
PlotterBot. See the following link for more information.

http://plotterbot.com/tiny-cnc/

This is still in the early stages of development and currently it
simply loads a Gcode file and parses it with respect to TinyCNC.

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
