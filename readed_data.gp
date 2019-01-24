set terminal png
set output 'readed_data.png'
set title "Readed data"
#set xrange [0.2:2.0]
#set yrange [0.4:2.5]
set xtics 0.5
set ytics 0.5
set grid

plot "readed_data.dat" every ::0:0:10000:0 using 1:2 lc 4 ps 0.5 pt 7 title "class1",\
"readed_data.dat" every ::0:1:10000:1 using 1:2 lc 6 ps 0.5 pt 7 title "class2"
