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


ax0.plot(b_O2.tempoC, b_O2.chiavi, 'ro-', label='C_O2')
ax0.plot(b_O2.tempoCpp, b_O2.chiavi, 'bo-', label='Cpp_O2')
ax0.set_yscale('log')
ax0.set_xscale('log')
ax0.set_title('Benchmark 02')
ax0.grid()
ax0.legend(shadow=True)

ax1.plot(b_O3.tempoC, b_O3.chiavi, 'ro-', label='C_O3')
ax1.plot(b_O3.tempoCpp, b_O3.chiavi, 'bo-', label='Cpp_O3')
ax1.set_yscale('log')
ax1.set_xscale('log')
ax1.set_title('Benchmark 03')
ax1.grid()
ax1.set_ylabel('Chiavi')
ax1.legend(shadow=True)

ax2.plot(b_Ofast.tempoC, b_Ofast.chiavi, 'ro-', label='C_Ofast')
ax2.plot(b_Ofast.tempoCpp, b_Ofast.chiavi, 'bo-', label='Cpp_Ofast')
ax2.set_yscale('log')
ax2.set_xscale('log')
ax2.set_title('Benchmark Ofast')
ax2.grid()
ax2.set_xlabel('Tempo')
ax2.legend(shadow=True)

plt.tight_layout()

plt.savefig('benchmark.png')
# plt.show()  
