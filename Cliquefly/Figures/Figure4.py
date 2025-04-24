from matplotlib import pyplot as plt




fig1, ax = plt.subplots(1, 4, figsize=(30, 6), gridspec_kw={'height_ratios': [1]})
axes = plt.axes()
axes.spines['right'].set_visible(False)
axes.spines['top'].set_visible(False)
plt.subplot(1, 4, 1)
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

x_VAL_ur = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.45, 0.5, 0.55]
VAL_ur = [580.9, 583.2, 586.2, 587.0, 592.1, 598.2, 607.5, 626.4, 650.8, 750, 1300]


for i in range(100):
    if len(new_ur)>i:
        new_ur[i] *= 2
    if len(par_ur)>i:
        par_ur[i] *= 2
    if len(VAL_ur)>i:
        VAL_ur[i] *= 2

plt.plot(x_new_ur, new_ur, color='dodgerblue', marker='s', markersize='10', linewidth=3,
         zorder=5, label = "RPAR")
plt.plot(x_par_ur, par_ur, color='purple', marker='o', markersize='10', linewidth=3,
         zorder=5, label = "PAR")
plt.plot(x_VAL_ur, VAL_ur, color='grey', marker='o', markersize='10', linewidth=3,
         zorder=13, label = "BIAS")
plt.title("(a) Uniform Random", fontsize=26, x=0.5, y=-0.22)
y_ticks_label_adv = [0, 300, 600, 900, 1200, 1500, 1800]
plt.yticks(y_ticks_label_adv, fontsize=20)
plt.xticks(x_ticks_ur, fontsize=20)
plt.ylim(top=2000)
plt.ylabel("Latency (ns)", fontsize=25)
plt.ylim(bottom=0)
plt.xlim(left=0)

plt.grid()
plt.xlabel("Load", fontsize=23, x=1)

plt.subplot(1, 4, 2)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
x_ticks_perm = [0, 0.1, 0.2, 0.3, 0.4, 0.5]

x_new_adv = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.4, 0.45]
new_adv = [388.7, 392.2, 393.2, 396.8, 402.1, 436.5, 477.2, 663.2, 1521.3]

x_par_adv = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450]
par_adv = [608.1, 610.9, 613.9, 620.1, 631.0, 647.9, 683.3, 742.1, 1975.6]  #已更新

x_ugal_adv = [0.05, 0.1, 0.15]
ugal_adv = [457.6, 633.4, 1500]

x_VAL_adv = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450, 0.500, 0.550]
VAL_adv  = [587.9, 589.3, 591.2, 592.5, 596.5, 603.8, 617.9, 633.4, 669.7, 760.3, 1930.9, ]


for i in range(100):
    if len(new_adv)>i:
        new_adv[i] *= 2
    if len(par_adv)>i:
        par_adv[i] *= 2
    if len(VAL_adv)>i:
        VAL_adv[i] *= 2
    if len(ugal_adv)>i:
        ugal_adv[i] *= 2

plt.plot(x_new_adv, new_adv, color='dodgerblue', marker='s', markersize='10', linewidth=3,
         zorder=5)
plt.plot(x_par_adv, par_adv, color='purple', marker='o', markersize='10', linewidth=3,
         zorder=8)

plt.plot(x_VAL_adv, VAL_adv, color='grey', marker='o', markersize='10', linewidth=3,
         zorder=6)
plt.title("(b) Adversarial", fontsize=26, x=0.5, y=-0.22)
plt.xlabel("Load", fontsize=23, x=1)

y_ticks_label_adv = [0, 300, 600, 900, 1200, 1500, 1800]
plt.yticks(y_ticks_label_adv, fontsize=20)
plt.xticks(x_ticks_adv, fontsize=20)
plt.ylim(top=2005)
plt.ylim(bottom=0)
plt.xlim(left=0)
plt.xlim(right=0.55)
# plt.legend(fontsize=20)
plt.grid()

x_new_perm = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.425,  0.500]

new_perm = [441.3, 442.6, 443.6, 444.2, 449.8, 470.0, 512.8, 724.1, 1022.7, 1422.1]

x_par_perm = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450, 0.500, 0.550, ]
par_perm = [556.3, 566.8, 572.8, 580.5, 589.1, 602.8, 611.5, 626.0, 648.7, 750.2,  1199.5, ]


x_ugal_perm = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450, 0.500, 0.550, 0.600]
ugal_perm = [384.0, 383.0, 382.6, 382.9, 384.0, 386.2, 392.3, 406.8, 449.7, 574.5, 953.3, 1120.4]

x_VAL_perm = [0.050, 0.100, 0.150, 0.200, 0.250, 0.300, 0.350, 0.400, 0.450, 0.500, 0.550]
VAL_perm = [583.2, 585.7, 586.5, 587.8, 591.5, 598.5, 610.7, 626.8, 649.6, 790.4, 1364.5,]

for i in range(100):
    if len(new_perm)>i:
        new_perm[i] *= 2
    if len(par_perm)>i:
        par_perm[i] *= 2
    if len(VAL_perm)>i:
        VAL_perm[i] *= 2
    if len(ugal_perm)>i:
        ugal_perm[i] *= 2

plt.subplot(1, 4, 3)
plt.gca().spines['bottom'].set_linewidth(3)
plt.gca().spines['left'].set_linewidth(3)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)

x_ticks_perm = [0, 0.1, 0.2, 0.3, 0.4, 0.5]


plt.plot(x_new_perm, new_perm, color='dodgerblue', marker='s', markersize='10', linewidth=3,
         zorder=50)
plt.plot(x_par_perm, par_perm, color='purple', marker='o', markersize='10', linewidth=3,
         zorder=13)
plt.plot(x_VAL_perm, VAL_perm, color='grey', marker='o', markersize='10', linewidth=3,
         zorder=14)

plt.title("(c) Random Permutation", fontsize=26, x=0.5, y=-0.22)
y_ticks_label_adv = [0, 300, 600, 900, 1200, 1500, 1800]
plt.yticks(y_ticks_label_adv, fontsize=20)
plt.xticks(x_ticks_adv, fontsize=20)
plt.ylim(top=2005)
plt.ylim(bottom=0)
plt.xlim(left=0)
plt.xlim(right=0.55)
fig1.legend(frameon=False, loc='upper center', bbox_to_anchor=(0.37, 1.012), ncol=8, fontsize=20, columnspacing=1.2)
plt.grid()
plt.xlabel("Load", fontsize=23, x=1)


ax1 = plt.subplot(1, 4, 4)
ax2 = ax1.twinx()
import numpy as np


categories = ['PAR', 'RPAR', ' PAR', 'RPAR ', ' PAR ', ' RPAR ']
data = [
    [0.18, 0.001, 0.027, 0.803],
    [0.615, 0.375, 0.002, 0.01],

    [0.038, 0.034, 0.094, 0.83],
    [0.80, 0.090, 0.021, 0.09],

    [0.21, 0.001, 0.033, 0.79],
    [0.44, 0.31, 0.04, 0.21]
]
colors = ['green', 'dodgerblue', 'orange', 'red']


averages = [5.98, 3.26, 5.97, 3.06, 5.83, 3.90]

totals = [sum(category_data) for category_data in data]
for i, category_data in enumerate(data):
    bottom = 0  
    for j, value in enumerate(category_data):
        height = value / totals[i]
        ax1.bar(categories[i], height, bottom=bottom, color=colors[j], width=0.4)
        bottom += height  

ax2.plot(categories, averages, 'd', color='black', label='Average hops', markersize=18)


ax1.set_yticks(np.linspace(0, 1, 6))
ax1.set_yticklabels(['0%', '20%', '40%', '60%','80%', '100%'], fontsize=20)
ax1.legend(['$\leq 3 hops\% $', '4 hops%', '5 hops%', '$\geq 6 hops\% $'], frameon=False, loc='upper center', bbox_to_anchor=(0.48, 1.195), ncol=4, fontsize=20, columnspacing=0.8)
ax2.legend(frameon=False, loc='upper center', bbox_to_anchor=(0.57, 1.125), ncol=2, fontsize=20, columnspacing=1.2)
plt.title("(d) Hop count", fontsize=26, x=0.5, y=-0.22)

plt.text(-0.25,2.0,"Uniform Random    Adversarial    Random Permutation", fontsize=16, color='black')
ax2.set_ylim(2.5, 6.5)
plt.subplots_adjust(left=0.05, right=0.95, top=0.88, bottom=0.17)
ax1.set_xticklabels(categories, fontsize=16)
ax1.yaxis.grid(True, linestyle='--', linewidth=0.7)
ax1.set_axisbelow(True)
ax2.set_yticklabels([2.5,3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5],fontsize=20)
# fig1.savefig(
#     'system_level_traffic.pdf',
#     format='pdf', dpi=500)
plt.show()


