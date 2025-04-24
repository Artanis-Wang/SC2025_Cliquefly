from matplotlib import pyplot as plt




fig1, ax = plt.subplots(1, 1, figsize=(10, 6), gridspec_kw={'height_ratios': [1]})
axes = plt.axes()
axes.spines['right'].set_visible(False)
axes.spines['top'].set_visible(False)
plt.subplot(1, 1, 1)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)

x_ticks_ur = [0, 0.2, 0.4, 0.6, 0.8, 1.0]

x_ticks_adv = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6]

x_new_ur = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450, 0.500, 0.550, 0.600]
new_ur = [397.8, 393.0, 390.4, 388.2, 388.2, 388.1, 390.6, 392.9, 397.3, 398.8, 512.0, 1120.3]


x_par_ur = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450, 0.500, 0.550, 0.600, 0.650, 0.700, 0.750, 0.800, 0.850, 0.900, 0.950, 0.975]
par_ur = [550.0, 562.3, 569.2, 583.3, 598.5, 615.9, 628.1, 650.7, 657.7, 649.4, 629.7, 605.4, 582.1, 563.1, 548.1, 534.4, 524.1, 521.9, 651.7, 1500]



x_ugal_ur = []
ugal_ur = []
x_VAL_ur = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.45, 0.5, 0.55]
VAL_ur = [580.9, 583.2, 586.2, 587.0, 592.1, 598.2, 607.5, 626.4, 650.8, 750, 1300]


x_ticks_inter = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8]

x_new_inter = [0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85]
new_inter = [211.1, 211.6, 212.1, 212.6, 213.2, 213.6, 214.8, 215.9, 217.1, 218.8, 220.5, 223.8, 227.5, 270.9, 380.0,690, 1900]

x_par_inter = [0.05, 0.1, 0.15, 0.2, 0.25, 0.3]
par_inter = [211.7, 229.7, 285.5, 568.0, 970, 1300]

x_VAL_inter = [0.05, 0.1, 0.15, 0.2, 0.25]
VAL_inter = [211.7, 244.7, 301.5, 679.0, 1200]

for i in range(100):
    if len(new_inter)>i:
        new_inter[i] *= 2
    if len(par_inter)>i:
        par_inter[i] *= 2
    if len(VAL_inter)>i:
        VAL_inter[i] *= 2

for i in range(100):
    if len(new_ur)>i:
        new_ur[i] *= 2
    if len(par_ur)>i:
        par_ur[i] *= 2
    if len(VAL_ur)>i:
        VAL_ur[i] *= 2

plt.plot(x_new_inter, new_inter, color='dodgerblue', marker='s', markersize='10', linewidth=3,
         zorder=5, label = "RPAR")
plt.plot(x_par_inter, par_inter, color='purple', marker='o', markersize='10', linewidth=3,
         zorder=5, label = "PAR")
plt.plot(x_VAL_inter, VAL_inter, color='grey', marker='o', markersize='10', linewidth=3,
         zorder=13, label = "BIAS")
y_ticks_label_adv = [0, 300, 600, 900, 1200, 1500, 1800]
plt.yticks(y_ticks_label_adv, fontsize=20)
plt.xticks(x_ticks_inter, fontsize=20)
plt.ylim(top=1200)
plt.ylabel("Latency (ns)", fontsize=25)
plt.ylim(bottom=0)
plt.xlim(left=0)
plt.xlim(right=0.85)
plt.grid()
plt.xlabel("Congestor Partition Load", fontsize=23, x=0.5)
fig1.legend(frameon=False, loc='upper center', bbox_to_anchor=(0.5, 0.95), ncol=8, fontsize=20, columnspacing=1.2)
plt.subplots_adjust(left=0.15, right=0.85, top=0.85, bottom=0.15)

# fig1.savefig(
#     'inter_traffic.pdf',
#     format='pdf', dpi=500)
plt.show()

