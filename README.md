TinyCNC
=======

Simple application for controlling TinyCNC robot built from
PlotterBot. See the following link for more information.

http://plotterbot.com/tiny-cnc/

This is still in the early stages of development and currently it
simply loads a Gcode file and parses it with respect to TinyCNC.

Requirements
============

git clone https://github.com/bgaster/tinyCNC.git tinyCNC
cd tinyCNC
git submodule update --init --recursive

Building
========

See Makefile to build. Currenly I've only tested it on OS X with
clang++ 3.3.

