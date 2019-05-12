#!/bin/bash

c++ Player.cpp Enemy.cpp Tile.cpp Bullet.cpp GameObj.cpp CollisionDetector.cpp main.cpp  -std=c++11 -lSDL2_image -lSDL2_mixer `sdl2-config --cflags --libs`
#rm ./a.out
./a.out
