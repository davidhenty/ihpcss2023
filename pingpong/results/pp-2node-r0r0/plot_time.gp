set logscale x
set logscale y
set term png
set output "node2-c0c0-time.png"
set xlabel "Message Size (Bytes)"
set ylabel "Time (seconds)"
plot \
"standard_time.plot" using 1:2 \
title "Standard send" with linespoint, \
"sync_time.plot" using 1:2 \
title "Synchronous send" with linespoint
