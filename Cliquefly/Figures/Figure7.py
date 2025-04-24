

from matplotlib import pyplot as plt


a2a_2d_idx = [1,2,3,4,5,6,7,8,9,10,11,12,13,14]


DF_new_a2a_2d_system_pars = [668, 668, 668, 855, 956, 1301, 1888, 2871, 5924, 16884, 37857, 93985, 217732, 496587] 
                                          #1     2     3     6    11   20     40    80    160  320    640   1280   2560
DF_ur_a2a_2d_system_par =   [797, 797, 797, 947, 1042, 1386, 2017, 2995, 5736, 14670, 36994, 77022, 172673, 432597]
DF_ur_a2a_2d_system_def =   [749, 749, 749, 874, 1041, 1600, 2696, 4269, 9014, 18973, 43080, 95834, 248980, 523167]



DF_new_a2a_2d_system_random_pars = [761, 761, 761, 947, 1142, 1734, 2785, 4952, 11060, 21215, 44513, 103660, 260563, 566479] 
                                                  #1    2     3     6    11   20     40    80    160  320    640   1280   2560
DF_ur_a2a_2d_system_random_par =   [811, 811, 811, 940, 1098, 1703, 2575, 4089, 8580, 16748, 38266, 93074, 219678, 469732]
DF_ur_a2a_2d_system_random_def =   [753, 753, 753, 905, 1134, 1920, 3721, 6349, 11161, 22732, 46046, 100973, 238746, 493254] 



DF_new_a2a_2d_part_pars = [660, 660, 660, 725, 782, 969, 1247, 1735, 3027, 6346, 14920, 39663, 89015, 227539, 557587]
                                        #1     2     3     6    11   20     40    80    160  320    640   1280   2560
DF_ur_a2a_2d_part_par =   [728, 728, 728, 793, 875, 1064, 1404, 1958, 3386, 6364, 14333, 36764, 85825, 212975, 529009]
DF_ur_a2a_2d_part_def =   [687, 687, 687, 739, 811, 1004, 1325, 1890, 3297, 6056, 15763, 40409, 95733, 239998, 593267]



DF_new_a2a_2d_part_random_pars = [658, 658, 658, 721, 813, 971, 1254, 1719, 3037, 5746, 13416, 36750, 80860, 185611, 465394]
                                               #1    2     3     6    11   20     40    80    160    320    640   1280   2560
DF_ur_a2a_2d_part_random_par =   [730, 730, 730, 797, 875, 1064, 1401, 1930, 3244, 6151, 13806, 34298, 80419, 199530, 478530]
DF_ur_a2a_2d_part_random_def =   [687, 687, 687, 751, 814, 1009, 1373, 2018, 3393, 6586, 14049, 35532, 85317, 218537, 529645]






com_size = [8, 32, 128, 512, 2000, 8000, 32000, 128000, 512000]
for i in range(20):
    if i < len(DF_new_a2a_2d_system_pars):
        DF_new_a2a_2d_system_pars[i] *=0.002
    if i < len(DF_ur_a2a_2d_system_par):
        DF_ur_a2a_2d_system_par[i] *=0.002
    if i < len(DF_ur_a2a_2d_system_def):
        DF_ur_a2a_2d_system_def[i] *=0.002
    if i < len(DF_new_a2a_2d_system_random_pars):
        DF_new_a2a_2d_system_random_pars[i] *=0.002
    if i < len(DF_ur_a2a_2d_system_random_par):
        DF_ur_a2a_2d_system_random_par[i]*=0.002
    if i < len(DF_ur_a2a_2d_system_random_def):
        DF_ur_a2a_2d_system_random_def[i] *=0.002

    if i < len(DF_new_a2a_2d_part_pars):
        DF_new_a2a_2d_part_pars[i] *=0.002
    if i < len(DF_ur_a2a_2d_part_par):
        DF_ur_a2a_2d_part_par[i]*=0.002
    if i < len(DF_ur_a2a_2d_part_def):
        DF_ur_a2a_2d_part_def[i]*=0.002
    if i < len(DF_new_a2a_2d_part_random_pars):
        DF_new_a2a_2d_part_random_pars[i] *=0.002
    if i < len(DF_ur_a2a_2d_part_random_par):
        DF_ur_a2a_2d_part_random_par[i] *=0.002
    if i < len(DF_ur_a2a_2d_part_random_def):
        DF_ur_a2a_2d_part_random_def[i] *=0.002



fig1, ax = plt.subplots(1, 2, figsize=(40, 10), gridspec_kw={'height_ratios': [1]})

x_idx = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

plt.subplot(1, 4, 1)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.xticks(fontsize=20)
plt.yticks(fontsize=24)
plt.plot(x_idx[0:8], DF_new_a2a_2d_system_pars[0:8],label = "RPAR-Linear", color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:8], DF_ur_a2a_2d_system_par[0:8],label = "PAR-Linear", color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_a2a_2d_system_def[0:8],label = "BIAS-Linear", color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:8], DF_new_a2a_2d_system_random_pars[0:8],label = "RPAR-Random", color='dodgerblue', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:8], DF_ur_a2a_2d_system_random_par[0:8],label = "PAR-Random",color='purple', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_a2a_2d_system_random_def[0:8],label = "BIAS-Random",color='grey', linestyle = 'dashed', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.title("(a) 100% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:8],['8B','16B','32B', '64B','128B', '256B' ,'512B','1KB'])
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
plt.plot(x_idx[0:6], DF_new_a2a_2d_system_pars[8:14], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:6], DF_ur_a2a_2d_system_par[8:14],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:6], DF_ur_a2a_2d_system_def[8:14], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:6], DF_new_a2a_2d_system_random_pars[8:14],color='dodgerblue', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:6], DF_ur_a2a_2d_system_random_par[8:14],color='purple', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:6], DF_ur_a2a_2d_system_random_def[8:14],color='grey', linestyle = 'dashed', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.title("(b) 100% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:6],[ '2KB', '4KB','8KB', '16KB','32KB','64KB'])
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
plt.plot(x_idx[0:8], DF_new_a2a_2d_part_pars[0:8], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:8], DF_ur_a2a_2d_part_par[0:8],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_a2a_2d_part_def[0:8], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)

plt.plot(x_idx[0:8], DF_new_a2a_2d_part_random_pars[0:8], color='dodgerblue', marker='o', linestyle = 'dashed',markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:8], DF_ur_a2a_2d_part_random_par[0:8],color='purple', marker='o',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_a2a_2d_part_random_def[0:8], color='grey', marker='s',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=9)
plt.title("(c) 25% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:8],['8B','16B','32B', '64B','128B', '256B' ,'512B','1KB'])
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
plt.plot(x_idx[0:7], DF_new_a2a_2d_part_pars[8:15], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:7], DF_ur_a2a_2d_part_par[8:15],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_a2a_2d_part_def[8:15], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:7], DF_new_a2a_2d_part_random_pars[8:15], color='dodgerblue', marker='o', linestyle = 'dashed',markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:7], DF_ur_a2a_2d_part_random_par[8:15],color='purple', marker='o',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_a2a_2d_part_random_def[8:15], color='grey', marker='s',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=9)

plt.title("(d) 25% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:7],[ '2KB', '4KB','8KB', '16KB','32KB','64KB','128KB'])
plt.xlim(left=0)
plt.grid()
plt.yticks(fontsize=24)


plt.subplots_adjust(left=0.12, right=0.95, top=0.95, bottom=0.15)
fig1.legend(frameon=False, loc='upper center', bbox_to_anchor=(0.5, 1.03), ncol=8, fontsize=25, columnspacing=0.6)


# fig1.savefig(
#     'df_global_a2a_2d.pdf',
#     format='pdf', dpi=500)

plt.show()