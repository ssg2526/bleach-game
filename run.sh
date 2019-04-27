#!/bin/bash

c++ Player.cpp Enemy.cpp Tile.cpp GameObj.cpp CollisionDetector.cpp main.cpp  -std=c++11 -lSDL2_image `sdl2-config --cflags --libs`
#rm ./a.out
./a.out
