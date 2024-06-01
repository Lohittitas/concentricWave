import os
import numpy as np
from scipy.special import jv, jn_zeros
from scipy.integrate import solve_ivp, trapezoid
import mpmath as mp
## Variables#####
N = 17
Rd = 4.282
nu = 0.00089
r = np.linspace(0, Rd, 200)
eps = 0.1
k1 = 16.4706 / Rd
ss = 0.004
a0 = 0.02599
b = 0.8
j1 = jn_zeros(1, N)
t_hat = np.arange(0.001, 0.399, 0.001)
gg = 981
gamma = 72
t0n = t_hat * np.sqrt(gg / Rd)
dt = 0.001 * np.sqrt(gg / Rd)
rr = 4
rri = 0.25
k = j1 / Rd
omega = np.sqrt(gg * k + gamma * k**3)
x0 = -0.0512 * a0 * np.exp(-0.16 * k**2) * k**2 # Modal spectrum of the initial condition for \hat{b}=0.8
yy = np.zeros(N)
eta = np.zeros(len(r))
path = "PATH_NAME"
non_t = np.sqrt(gg / Rd)
#######
def gen_data(data, temp, i=0):
    for l in range(len(r)):
        yy = k * temp[i, :] * jv(0, k * r[l])
        data[l, 1] = (trapezoid(yy, k))
    data[l + 1 :, 1] = (data[: l + 1, 1])
    data[:l+1,1] = np.flip(data[:l+1,1])
    return data
z2 = (
    lambda s, i: x0[i]
    * (
        s
        + 4 * nu * k[i] ** 2
        - (4 * k[i] ** 3 * nu / (k[i] + (k[i] ** 2 + s / nu) ** 0.5))
    )
    / (
        s
        * (
            s
            + 4 * nu * k[i] ** 2
            - (4 * k[i] ** 3 * nu / (k[i] + (k[i] ** 2 + s / nu) ** 0.5))
        )
        + omega[i] ** 2
    )
)

temp2 = np.zeros((len(t_hat), len(k)), dtype=float)
for j in range(len(t_hat)):  # len(t_hat)
    t = t_hat[j]
    for i in range(len(k)):
        temp2[j, i] = mp.invertlaplace(
            lambda s: z2(s, i),
            t 
            
        )
data = np.zeros((2 * len(r), 2))
data[:, 0] = np.concatenate((-r, r))
import os
if not os.path.exists("data"):
    os.makedirs("data")
for i in range(len(t_hat)):
    for l in range(len(r)):
        yy = k * temp2[i, :] * jv(0, k * r[l])
        data[l, 1] = trapezoid(yy, k)
    data[l + 1 :, 1] = data[: l + 1, 1]
    np.savetxt(f"data/data-{i}.csv", data, delimiter=",")
