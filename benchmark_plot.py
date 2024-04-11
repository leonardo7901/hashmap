import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.gridspec import GridSpec

b_O2 = pd.read_csv("benchmark_O2.csv")
b_O3 = pd.read_csv("benchmark_O3.csv")
b_Ofast = pd.read_csv("benchmark_Ofast.csv") 


fig = plt.figure(figsize=(8, 8))
gs = GridSpec(3, 1, height_ratios=[2, 1, 2])

ax0 = plt.subplot(gs[0])
ax1 = plt.subplot(gs[1])
ax2 = plt.subplot(gs[2])


ax0.plot(b_O2.tempoCInserimento, b_O2.chiavi, 'ro-', label='InsC')
ax0.plot(b_O2.tempoCppInserimento, b_O2.chiavi, 'bo-', label='InsC++')
ax0.plot(b_O2.tempoCRicerca, b_O2.chiavi, 'go-', label='SearchC')
ax0.plot(b_O2.tempoCppRicerca, b_O2.chiavi, 'mo-', label='SearchC++')
ax0.plot(b_O2.tempoCDelete, b_O2.chiavi, 'ko-', label='DeleteC')
ax0.plot(b_O2.tempoCppDelete, b_O2.chiavi, 'yo-', label='DeleteC++')
ax0.set_yscale('log')
ax0.set_xscale('log')
ax0.set_title('Benchmark 02')
ax0.grid()
ax0.legend(shadow=True, loc='best')

ax1.plot(b_O3.tempoCInserimento, b_O3.chiavi, 'ro-', label='InsC')
ax1.plot(b_O3.tempoCppInserimento, b_O3.chiavi, 'bo-', label='InsC++')
ax1.plot(b_O3.tempoCRicerca, b_O3.chiavi, 'go-', label='SearchC')
ax1.plot(b_O3.tempoCppRicerca, b_O3.chiavi, 'mo-', label='SearchC++')
ax1.plot(b_O3.tempoCDelete, b_O3.chiavi, 'ko-', label='DeleteC')
ax1.plot(b_O3.tempoCppDelete, b_O3.chiavi, 'yo-', label='DeleteC++')
ax1.set_yscale('log')
ax1.set_xscale('log')
ax1.set_title('Benchmark 03')
ax1.grid()
ax1.set_ylabel('Chiavi')
#ax1.legend(shadow=True)

ax2.plot(b_Ofast.tempoCInserimento, b_Ofast.chiavi, 'ro-', label='InsC')
ax2.plot(b_Ofast.tempoCppInserimento, b_Ofast.chiavi, 'bo-', label='InsC++')
ax2.plot(b_Ofast.tempoCRicerca, b_Ofast.chiavi, 'go-', label='SearchC')
ax2.plot(b_Ofast.tempoCppRicerca, b_Ofast.chiavi, 'mo-', label='SearchC++')
ax2.plot(b_Ofast.tempoCDelete, b_Ofast.chiavi, 'ko-', label='DeleteC')
ax2.plot(b_Ofast.tempoCppDelete, b_Ofast.chiavi, 'yo-', label='DeleteC++')
ax2.set_yscale('log')
ax2.set_xscale('log')
ax2.set_title('Benchmark Ofast')
ax2.grid()
ax2.set_xlabel('Tempo')
#ax2.legend(shadow=True)

plt.tight_layout()

plt.savefig('benchmark.png')
# plt.show()  