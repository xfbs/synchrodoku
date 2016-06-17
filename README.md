# Synchodoku

[![Build Status](https://travis-ci.org/xfbs/synchrodoku.svg?branch=master)](https://travis-ci.org/xfbs/synchrodoku)

This is an experiment to write
a sudoku solver in C that uses message-passing
concurrency instead of doing a naïve recursive approach
or using traditional state-sharing concurrency.

The idea behind this project is for me to play
around with message-passing using ZeroMQ, and
to gain experience working with multithreaded
programs.

## How does it work?

![diagram](diagram.png)

Simply put, there is a managing thread and one (or more)
worker threads. The managing thread keeps the workers
supplied with jobs (for example unsolved sudokus), and
the workers work on the jobs, and reply in one of two
ways:
  - they send a solution (solved sudoku) back to
    managing thread
  - they send multiple partial solutions (unsolved
    sudokus) back to managing thread

This way, simple solutions can be found quickly, but
if there is more than one solution, the others are also
found. 

## Libraries

 -  [ZeroMQ](http://zeromq.org)
 -  [mpack](https://github.com/ludocode/mpack)
 -  [jansson](http://www.digip.org/jansson/)
 -  [glib](https://developer.gnome.org/glib/2.48/)
 -  `pthreads`
