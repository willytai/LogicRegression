import sys

size = 0
with open(sys.argv[1], 'r') as f:
    for line in f:
        line = line.strip().split()
        if len(line) == 0: continue
        g_type = line[0]
        if g_type == 'AND2' or g_type == 'OR2' or g_type == 'XOR2' or g_type == 'NOR2' or g_type == 'XNOR2' or g_type == 'NAND2' or g_type == 'AND' or g_type == 'INV': size += 2

print ('circuit size:', size)
