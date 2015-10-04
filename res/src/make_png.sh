#!/bin/bash

#pacman
montage -background "transparent" pacman_right_*.svg -geometry 40x40 -tile 1x6 -matte -transparent "transparent" pacman_right.png
montage -background "transparent" pacman_right_*.svg -flop -geometry 40x40 -tile 1x6 -matte -transparent "transparent" pacman_left.png
montage -background "transparent" pacman_right_*.svg -rotate 90 -flop -geometry 40x40 -tile 1x6 -matte -transparent "transparent" pacman_down.png
montage -background "transparent" pacman_right_*.svg -rotate 270 -geometry 40x40 -tile 1x6 -matte -transparent "transparent" pacman_up.png
montage -background "transparent" pacman_{left,up,right,down}.png -geometry 40x240 -tile 4x1 -matte -transparent "transparent" ../pacman.png
rm -f pacman_{left,up,right,down}.png

#ghosts
montage -background "transparent" ghost_blue_{left,up,right,down}.svg -geometry 40x40 -tile 4x1 -matte -transparent "transparent" ../ghost_blue.png
montage -background "transparent" ghost_orange_{left,up,right,down}.svg -geometry 40x40 -tile 4x1 -matte -transparent "transparent" ../ghost_orange.png
montage -background "transparent" ghost_pink_{left,up,right,down}.svg -geometry 40x40 -tile 4x1 -matte -transparent "transparent" ../ghost_pink.png
montage -background "transparent" ghost_red_{left,up,right,down}.svg -geometry 40x40 -tile 4x1 -matte -transparent "transparent" ../ghost_red.png

#walls
montage -background "transparent" wall.svg -geometry 40x40 -tile 1x1 -matte -transparent "transparent" ../wall.png

#pills
montage -background "transparent" pill_small.svg -geometry 40x40 -tile 1x1 -matte -transparent "transparent" ../pill_small.png
montage -background "transparent" pill_big.svg -geometry 40x40 -tile 1x1 -matte -transparent "transparent" ../pill_big.png
