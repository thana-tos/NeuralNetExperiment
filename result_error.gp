set terminal png
set output 'result_error.png'
set title "error change"
set xlabel "number of samples"
set ylabel "total error[log scale]"
unset key
set logscale y

plot "result_error.dat" w l lc 7 lw 2
