set terminal png
set output 'result.png'
set title "Result"
set yrange [-0.0001:1.0001]
set xlabel "number of samples"
set ylabel "output value"
set key
set grid y
set ytics 0.1

plot "result.dat" using 5 w p lc 4 ps 0.5 pt 7 title "class1",\
"result.dat" using 7 w p lc 3 ps 0.5 pt 7 title "class2"
