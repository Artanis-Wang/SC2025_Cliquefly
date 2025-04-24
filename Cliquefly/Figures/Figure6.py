

from matplotlib import pyplot as plt

from hpc_finishTime_various_kb import halo2D_size

#halo2d
halo2d_size = [0.008, 0.032, 0.128, 0.512, 2, 8, 32, 128, 512]

halo2D_size = [0.008, 0.032, 0.128, 0.512, 2, 8, 32, 128, 512]

halo2d_idx = [1,2,3,4,5,6,7,8,9,10,11,12,13,14]




DF_new_halo2d_system_pars = [1567, 1567, 1567, 1625, 1698, 1746, 1993, 2141, 2357, 2844, 3537, 5349, 9790, 19150, 41956] 
DF_ur_halo2d_system_par =   [2483, 2483, 2483, 2401, 2547, 2665, 3157, 3342, 3649, 3992, 4710, 6674, 10904, 22473, 50956]
DF_ur_halo2d_system_def =   [2661, 2661, 2661, 2893, 2989, 3150, 3177, 3289, 3476, 3909, 4820, 6579, 10837, 22530, 51811]



DF_new_halo2d_system_random_pars = [2631, 2631, 2631, 2676, 2815, 2858, 3841, 4377, 4669, 5132, 5846, 7978, 13977, 28151, 66832] 
DF_ur_halo2d_system_random_par =   [2898, 2898, 2898, 2990, 3130, 3287, 4175, 4365, 4923, 5364, 6228, 7925, 12252, 26134, 60460]
DF_ur_halo2d_system_random_def =   [3827, 3827, 3827, 4020, 4086, 4356, 4614, 4599, 4832, 5295, 6229, 8186, 12797, 27230, 62746] 



DF_new_halo2d_part_pars = [1834, 1834, 1834, 1866, 2189, 2225, 2268, 2618, 2928, 3186, 3890, 5480, 9476, 20869, 46115] 
DF_ur_halo2d_part_par =   [2547, 2547, 2547, 2557, 2580, 2645, 2929, 3140, 3512, 3854, 4664, 5962, 9847, 19535, 44957]
DF_ur_halo2d_part_def =   [3012, 3012, 3012, 3044, 3055, 2986, 3146, 3217, 3424, 3857, 4584, 6199, 10542, 20779, 48287]


DF_new_halo2d_part_random_pars = [2826, 2826, 2826, 2889, 2857, 2862, 3570, 3734, 4385, 4737, 5507, 7268, 11115, 23322, 46483]
DF_ur_halo2d_part_random_par =   [4221, 4221, 4221, 4139, 4096, 4078, 4469, 4842, 5027, 5399, 6041, 7857, 11264, 23996, 48342]
DF_ur_halo2d_part_random_def =   [4254, 4254, 4254, 4297, 4311, 4326, 4411, 4473, 4751, 5098, 5720, 7418, 11340, 23545, 50363]





com_size = [8, 32, 128, 512, 2000, 8000, 32000, 128000, 512000]
for i in range(20):
    if i < len(DF_new_halo2d_system_pars):
        DF_new_halo2d_system_pars[i] *=0.002
    if i < len(DF_ur_halo2d_system_par):
        DF_ur_halo2d_system_par[i] *=0.002
    if i < len(DF_ur_halo2d_system_def):
        DF_ur_halo2d_system_def[i] *=0.002
    if i < len(DF_new_halo2d_system_random_pars):
        DF_new_halo2d_system_random_pars[i] *=0.002
    if i < len(DF_ur_halo2d_system_random_par):
        DF_ur_halo2d_system_random_par[i]*=0.002
    if i < len(DF_ur_halo2d_system_random_def):
        DF_ur_halo2d_system_random_def[i] *=0.002

    if i < len(DF_new_halo2d_part_pars):
        DF_new_halo2d_part_pars[i] *=0.002
    if i < len(DF_ur_halo2d_part_par):
        DF_ur_halo2d_part_par[i]*=0.002
    if i < len(DF_ur_halo2d_part_def):
        DF_ur_halo2d_part_def[i]*=0.002
    if i < len(DF_new_halo2d_part_random_pars):
        DF_new_halo2d_part_random_pars[i] *=0.002
    if i < len(DF_ur_halo2d_part_random_par):
        DF_ur_halo2d_part_random_par[i] *=0.002
    if i < len(DF_ur_halo2d_part_random_def):
        DF_ur_halo2d_part_random_def[i] *=0.002



fig1, ax = plt.subplots(1, 2, figsize=(40, 10), gridspec_kw={'height_ratios': [1]})

x_idx = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]

plt.subplot(1, 4, 1)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.xticks(fontsize=20)
plt.yticks(fontsize=24)
plt.plot(x_idx[0:8], DF_new_halo2d_system_pars[0:8],label = "RPAR-Linear", color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:8], DF_ur_halo2d_system_par[0:8],label = "PAR-Linear", color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_halo2d_system_def[0:8],label = "BIAS-Linear", color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:8], DF_new_halo2d_system_random_pars[0:8],label = "RPAR-Random",color='dodgerblue', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:8], DF_ur_halo2d_system_random_par[0:8],label = "PAR-Random",color='purple', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_halo2d_system_random_def[0:8],label = "BIAS-Random",color='grey', linestyle = 'dashed', marker='s', markersize='10', linewidth=3,
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
plt.plot(x_idx[0:7], DF_new_halo2d_system_pars[8:15], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)

plt.plot(x_idx[0:7], DF_ur_halo2d_system_par[8:15],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_halo2d_system_def[8:15], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:7], DF_new_halo2d_system_random_pars[8:15],color='dodgerblue', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=51)

plt.plot(x_idx[0:7], DF_ur_halo2d_system_random_par[8:15],color='purple', linestyle = 'dashed', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_halo2d_system_random_def[8:15],color='grey', linestyle = 'dashed', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.title("(b) 100% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:7],[ '2KB', '4KB','8KB', '16KB','32KB','64KB','128KB'])
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
plt.plot(x_idx[0:8], DF_new_halo2d_part_pars[0:8], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:8], DF_ur_halo2d_part_par[0:8],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_halo2d_part_def[0:8], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)

plt.plot(x_idx[0:8], DF_new_halo2d_part_random_pars[0:8], color='dodgerblue', marker='o', linestyle = 'dashed',markersize='10', linewidth=3,
         zorder=51)

plt.plot(x_idx[0:8], DF_ur_halo2d_part_random_par[0:8],color='purple', marker='o',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:8], DF_ur_halo2d_part_random_def[0:8], color='grey', marker='s',linestyle = 'dashed', markersize='10', linewidth=3,
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
plt.plot(x_idx[0:7], DF_new_halo2d_part_pars[8:15], color='dodgerblue', marker='o', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_idx[0:7], DF_ur_halo2d_part_par[8:15],color='purple', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_halo2d_part_def[8:15], color='grey', marker='s', markersize='10', linewidth=3,
         zorder=9)
plt.plot(x_idx[0:7], DF_new_halo2d_part_random_pars[8:15], color='dodgerblue', marker='o', linestyle = 'dashed',markersize='10', linewidth=3,
         zorder=51)
plt.plot(x_idx[0:7], DF_ur_halo2d_part_random_par[8:15],color='purple', marker='o',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=6)
plt.plot(x_idx[0:7], DF_ur_halo2d_part_random_def[8:15], color='grey', marker='s',linestyle = 'dashed', markersize='10', linewidth=3,
         zorder=9)

plt.title("(d) 25% Nodes", fontsize=26, x=0.5, y=-0.15)
plt.ylabel("Communication time (us)", fontsize=30)
plt.xlabel("Message size", fontsize=24, x=0.5)
plt.xticks(x_idx[0:7],[ '2KB', '4KB','8KB', '16KB','32KB','64KB','128KB'])
plt.xlim(left=0)
plt.grid()
plt.yticks(fontsize=24)



fig1.legend(frameon=False, loc='upper center', bbox_to_anchor=(0.5, 1.03), ncol=8, fontsize=28, columnspacing=0.6)

plt.subplots_adjust(left=0.12, right=0.95, top=0.95, bottom=0.15)

# fig1.savefig(
#     'df_global_halo2d.pdf',
#     format='pdf', dpi=500)

plt.show()