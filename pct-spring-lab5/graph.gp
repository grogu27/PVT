set terminal pdf color enhanced font 'Calibri,16' size 20cm,14cm
set output 'graph.pdf'
set key inside left top font 'Calibri,16'
set colorsequence podo
set style line 1 lc rgb 'blue' lw 1 pt 5 ps 0.5
set style line 2 lt 1 lw 2 pt 2 ps 0.5
set style line 3 lt 2 lw 2 pt 3 ps 0.5
set style line 4 lt 4 lw 2 pt 5 ps 0.5
set style line 5 lt 5 lw 2 pt 7 ps 0.5
set style line 6 lt 6 lw 2 pt 9 ps 0.5
set xlabel "Number of threads (n)" font 'Calibri,16'
set ylabel "Speedup (S)" font 'Calibri,16'
set format y "%.1f"
set format x "%.0f"
set xtics 2

# Define the range of the x-axis and y-axis
set xrange [2:8]
set yrange [0:]

# Plot the linear speedup and data from res.txt
plot x title "Linear speedup" with lines lc rgb 'blue' lt 1 lw 2,\
     'res.txt' using 1:2 title "Tasks speedup" with linespoints ls 2