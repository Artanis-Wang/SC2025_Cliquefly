

from matplotlib import pyplot as plt


#ring
ring_size = [0.008, 0.032, 0.128, 0.512, 2, 8, 32, 128, 512]

ring_size = [0.008, 0.032, 0.128, 0.512, 2, 8, 32, 128, 512]

ring_idx = [1,2,3,4,5,6,7,8,9,10,11,12,13,14]


DF_new_ring_system_pars = [30251, 30251, 30251,  30957, 30962, 31333, 34633, 39323, 46022, 56782, 77650, 119082, 192842, 0, 0]  
                                            #1     2     3     6        11     20    40     80    160     320     640   1280   2560
DF_ur_ring_system_par =   [38638, 38638, 38638, 38638, 39817, 40963, 60694, 61538, 74314, 84981, 105518, 148500, 215479, 0, 0]
DF_ur_ring_system_def =   [59333, 59333, 59333, 59988, 60843, 61869, 66695, 69967, 76471, 88367, 113068, 161906, 239657, 0, 0]



DF_new_ring_system_random_pars = [102579, 102579, 102579, 103279, 104318, 107288, 137191, 140630, 160944, 194515, 231031, 302652, 482672, 0]  
                                                    #1      2         3     6        11     20     40       80    160      320     640     1280   2560
DF_ur_ring_system_random_par =   [121257, 121257, 121257, 121257, 122469, 125567, 154160, 156401, 176223, 189672, 217961, 274827, 442570, 0]
DF_ur_ring_system_random_def =   [143078, 143078, 143078, 143078, 144767, 147323, 156007, 160117, 169631, 181864, 209703, 266164, 439510, 0] 



DF_new_ring_part_pars = [14265, 14265, 14265, 14359, 14422, 14425, 15793, 17033, 19907, 24070, 30515, 41988, 65356, 111359, 203653]
                                        #1     2        3     6       11    20     40     80     160    320   640     1280    2560
DF_ur_ring_part_par =   [23007, 23007, 23007, 22977, 23195, 23136, 25549, 27139, 30890, 37156, 43017, 55185, 78581, 124936, 217377]
DF_ur_ring_part_def =   [28060, 28060, 28060, 28092, 28031, 28096, 29738, 30695, 32650, 35851, 42110, 54196, 77972, 124708, 217357]



DF_new_ring_part_random_pars = [27657, 27657, 27657, 27697, 27957, 28657, 30657, 32657, 36657, 33657, 45657, 69731, 96543, 140295, 280345]
                                               #1       2     3     6        11   20     40    80    160      320    640    1280   2560
DF_ur_ring_part_random_par =   [42754, 42754, 42754, 42817, 42632, 42418, 47280, 47971, 51364, 54513, 60958, 73492, 97691, 145594, 293065]
DF_ur_ring_part_random_def =   [42072, 42072, 42072, 42011, 42012, 42201, 44283, 45420, 47599, 50907, 57262, 69701, 94162, 141792, 288357]






com_size = [8, 32, 128, 512, 2000, 8000, 32000, 128000, 512000]
for i in range(20):
    if i < len(DF_new_ring_system_pars):
        DF_new_ring_system_pars[i] *=0.002
    if i < len(DF_ur_ring_system_par):
        DF_ur_ring_system_par[i] *=0.002
    if i < len(DF_ur_ring_system_def):
        DF_ur_ring_system_def[i] *=0.002
    if i < len(DF_new_ring_system_random_pars):
        DF_new_ring_system_random_pars[i] *=0.002
    if i < len(DF_ur_ring_system_random_par):
        DF_ur_ring_system_random_par[i]*=0.002
    if i < len(DF_ur_ring_system_random_def):
        DF_ur_ring_system_random_def[i] *=0.002

    if i < len(DF_new_ring_part_pars):
        DF_new_ring_part_pars[i] *=0.002
    if i < len(DF_ur_ring_part_par):
        DF_ur_ring_part_par[i]*=0.002
    if i < len(DF_ur_ring_part_def):
        DF_ur_ring_part_def[i]*=0.002
    if i < len(DF_new_ring_part_random_pars):
        DF_new_ring_part_random_pars[i] *=0.002
    if i < len(DF_ur_ring_part_random_par):
        DF_ur_ring_part_random_par[i] *=0.002
    if i < len(DF_ur_ring_part_random_def):
        DF_ur_ring_part_random_def[i] *=0.002



fig1, ax = plt.subplots(1, 2, figsize=(40, 10), gridspec_kw={'height_ratios': [1]})

x_idx = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

plt.subplot(1, 4, 1)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.xticks(fontsize=20)
plt.yticks(fontsize=24)
plt.plot(x_idx[0:8], DF_new_ring_system_pars[0:8],label = "RPAR-Linear", color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:8], DF_ur_ring_system_par[0:8],label = "PAR-Linear", color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_ring_system_def[0:8],label = "BIAS-Linear", color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:8], DF_new_ring_system_random_pars[0:8],label = "RPAR-Random",color='dodgerblue', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=5)

plt.plot(x_idx[0:8], DF_ur_ring_system_random_par[0:8],label = "PAR-Random",color='purple', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_ring_system_random_def[0:8],label = "BIAS-Random",color='grey', linestyle = 'dashed', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.title("(a) 100% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:8],['8B','16B','32B', '64B','128B', '256B' ,'512B','1KB'])
# plt.ylim(bottom)
plt.xlim(left=0)
plt.grid()
plt.yticks(fontsize=24)


plt.subplot(1, 4, 2)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.xticks(fontsize=20)
plt.yticks(fontsize=24)
plt.plot(x_idx[0:5], DF_new_ring_system_pars[8:13], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:5], DF_ur_ring_system_par[8:13],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:5], DF_ur_ring_system_def[8:13], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:5], DF_new_ring_system_random_pars[8:13],color='dodgerblue', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=5)

plt.plot(x_idx[0:5], DF_ur_ring_system_random_par[8:13],color='purple', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:5], DF_ur_ring_system_random_def[8:13],color='grey', linestyle = 'dashed', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.title("(b) 100% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:5],[ '2KB', '4KB','8KB', '16KB','32KB'])
plt.xlim(left=0)
plt.grid()
plt.yticks(fontsize=24)


plt.subplot(1, 4, 3)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.xticks(fontsize=20)
plt.yticks(fontsize=24)
plt.plot(x_idx[0:8], DF_new_ring_part_pars[0:8], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:8], DF_ur_ring_part_par[0:8],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_ring_part_def[0:8], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)

plt.plot(x_idx[0:8], DF_new_ring_part_random_pars[0:8], color='dodgerblue', marker='o', linestyle = 'dashed',markersize='10', linewidth=3,
         zorder=5)

plt.plot(x_idx[0:8], DF_ur_ring_part_random_par[0:8],color='purple', marker='o',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_ring_part_random_def[0:8], color='grey', marker='s',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=9)
plt.title("(c) 25% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:8],['8B','16B','32B', '64B','128B', '256B' ,'512B','1KB'])
# plt.ylim(bottom)
plt.xlim(left=0)
plt.grid()
plt.yticks(fontsize=24)


plt.subplot(1, 4, 4)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.xticks(fontsize=20)
plt.yticks(fontsize=24)
plt.plot(x_idx[0:7], DF_new_ring_part_pars[8:15], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:7], DF_ur_ring_part_par[8:15],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_ring_part_def[8:15], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:7], DF_new_ring_part_random_pars[8:15], color='dodgerblue', marker='o', linestyle = 'dashed',markersize='10', linewidth=3,
         zorder=5)
plt.plot(x_idx[0:7], DF_ur_ring_part_random_par[8:15],color='purple', marker='o',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_ring_part_random_def[8:15], color='grey', marker='s',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=9)

plt.title("(d) 25% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:7],[ '2KB', '4KB','8KB', '16KB','32KB','64KB','128KB'])
plt.xlim(left=0)
plt.grid()
plt.yticks(fontsize=24)


plt.subplots_adjust(left=0.12, right=0.95, top=0.95, bottom=0.15)
fig1.legend(frameon=False, loc='upper center', bbox_to_anchor=(0.5, 1.03), ncol=8, fontsize=28, columnspacing=0.6)


# fig1.savefig(
#     'df_global_ring.pdf',
#     format='pdf', dpi=500)

plt.show()