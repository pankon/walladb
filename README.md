# walladb
Walla, I made a real time spacial db. Enjoy

format:

file_start:
[WALLA-DB\]\[root\]\[free\]

    node:
    [depth/next_node]
        [node_info]
            [p][c * 8]
            [n][data * n]
            
    node_info:
    [pos]
        [epoch_start]
        [epoch_end]
        [average]
        [stdev]
