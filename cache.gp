reset
set style fill solid
set title 'cache-miss comparison'
set term png
set output 'cache-analysis.png'
set label "96.291 % \ncache-misses" at graph 0.1,0.4
set label "67.476 % \ncache-misses" at graph 0.6,0.4

plot [:][:]'cache.txt' using 2:xtic(1) with histogram title 'cache-misses', \
'' using ($0+0.001):($2+0.01):2 with labels title ' ', \
'' using 3:xtic(1) with histogram title 'cache-references'  , \
'' using ($0+0.3):($3+0.01):3 with labels title ' '
