# Day 12
I solved this one using Dijkstra's algorithm.

## First half
- First I generated graph where each edge has cost 1 and edges are computed from elevations of neighbours.
- Then I ran Dijkstra's algirithm on given graph starting from 'S'
- Then I took vertex value on `E`.

## Second half
- I changed computing of graph.
  - Neighbour element can be any elevation higher, but only 1 elevation lower (reverse from the first half).
- Then I ran Dijkstra's algorithm on given graph starting from 'E'
- Then took the element with elevation of `a` and the lowest vertex value.

## After thoughs
- I could've just done the second half and then take the value of 'S'. However I'm too lazy to implement this now, because I made two separate programs.
