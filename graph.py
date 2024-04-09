import sys
import matplotlib.pyplot as plt
import numpy as np

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
        lines_sum_chr = f.readlines()

filename = sys.argv[5]
with open(filename, 'r') as f:
        lines_norm = f.readlines()

filename = sys.argv[6]
with open(filename, 'r') as f:
        lines_ror = f.readlines()

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

data_sum_chr = []
for line in lines_sum_chr:
        data_sum_chr.append(int(line))

data_norm = []
for line in lines_norm:
        data_norm.append(int(line))

data_ror = []
for line in lines_ror:
        data_ror.append(int(line))

data_rol = []
for line in lines_rol:
        data_rol.append(int(line))

data_based = []
for line in lines_based:
        data_based.append(int(line))

fig, axs = plt.subplots(2, 4)

plt.xlabel('bucket')
plt.ylabel('number of words')

bucket_size = 1500

axs[0, 0].stairs(data_al_zero)
axs[0, 0].set_title('always zero')

axs[0, 1].stairs(data_alw_fchr)
axs[0, 1].set_title('always first char')

axs[0, 2].stairs(data_len)
axs[0, 2].set_title('length')

axs[0, 3].stairs(data_sum_chr)
axs[0, 3].set_title('sum of chars')

axs[1, 0].stairs(data_norm)
axs[1, 0].set_title('normalized')

axs[1, 1].stairs(data_ror)
axs[1, 1].set_title('ror')

axs[1, 2].stairs(data_rol)
axs[1, 2].set_title('rol')

axs[1, 3].stairs(data_based)
axs[1, 3].set_title('based')

plt.tight_layout()

plt.show()
