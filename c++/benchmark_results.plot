# Scale font and line width (dpi) by changing the size! It will always display stretched.
set terminal svg size 600,300 enhanced fname 'arial'  fsize 10 butt solid
set output 'out.svg'

# Key means label...
set key inside bottom right
set logscale xy
set xlabel 'n'
set ylabel 'Time (ms)'
set title 'Fibo Comparison'
plot  "data.txt" using 1:2 title 'Heater' with lines, "data.txt" using 1:3 title 'ejolson' with lines, "data.txt" using 1:4 title 'ejolsonNew' with lines

