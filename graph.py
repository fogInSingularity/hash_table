import sys
import matplotlib.pyplot as plt
import numpy as np

number_of_nuckets = 2003
bar_width = 1.5
max_hight = 50

filename = sys.argv[1]
with open(filename, 'r') as f:
        lines_al_zero = f.readlines()

filename = sys.argv[2]
with open(filename, 'r') as f:
        lines_alw_fchr = f.readlines()

filename = sys.argv[3]
with open(filename, 'r') as f:
        lines_len = f.readlines()

filename = sys.argv[4]
with open(filename, 'r') as f:
        lines_norm = f.readlines()

filename = sys.argv[5]
with open(filename, 'r') as f:
        lines_sum_chr = f.readlines()

filename = sys.argv[6]
with open(filename, 'r') as f:
        lines_crc = f.readlines()

filename = sys.argv[7]
with open(filename, 'r') as f:
        lines_rol = f.readlines()

filename = sys.argv[8]
with open(filename, 'r') as f:
        lines_based = f.readlines()

# -----------------------------------

data_al_zero = []
for line in lines_al_zero:
        data_al_zero.append(int(line))

data_alw_fchr = []
for line in lines_alw_fchr:
        data_alw_fchr.append(int(line))

data_len = []
for line in lines_len:
        data_len.append(int(line))

data_norm = []
for line in lines_norm:
        data_norm.append(int(line))

data_sum_chr = []
for line in lines_sum_chr:
        data_sum_chr.append(int(line))

data_crc = []
for line in lines_crc:
        data_crc.append(int(line))

data_rol = []
for line in lines_rol:
        data_rol.append(int(line))

data_based = []
for line in lines_based:
        data_based.append(int(line))

fig, axs = plt.subplots(2, 4)

plt.xlabel('bucket')
plt.ylabel('number of words')

axs[0, 0].bar([i for i in range(number_of_nuckets)], data_al_zero, width=bar_width)
axs[0, 0].set_title('always zero')

axs[0, 1].bar([i for i in range(number_of_nuckets)], data_alw_fchr, width=bar_width)
axs[0, 1].set_title('always first char')

axs[0, 2].bar([i for i in range(number_of_nuckets)], data_len, width=bar_width)
axs[0, 2].set_title('length')

axs[0, 3].bar([i for i in range(number_of_nuckets)], data_norm, width=bar_width)
axs[0, 3].set_title('normalized')

axs[1, 0].bar([i for i in range(number_of_nuckets)], data_sum_chr, width=bar_width)
axs[1, 0].set_title('sum of chars')

axs[1, 1].bar([i for i in range(number_of_nuckets)], data_crc, width=bar_width, )
axs[1, 1].set_title('crc')
axs[1, 1].set_ylim(0, max_hight)

axs[1, 2].bar([i for i in range(number_of_nuckets)], data_rol, width=bar_width)
axs[1, 2].set_title('rol')

axs[1, 3].bar([i for i in range(number_of_nuckets)], data_based, width=bar_width)
axs[1, 3].set_title('mur')

plt.tight_layout()

plt.show()
