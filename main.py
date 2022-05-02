# Made on Earth by Murathon
import math
import numpy as np
import matplotlib.pyplot as plt
import pymap3d


class TakeCoordinate():

    def __init__(self, t):
        self.time = t
        self.XYZ = np.zeros((1, 4))
        self.XYZv = np.zeros((1, 4))
        self.WGS_XYZ = np.zeros((1, 4))
        self.WGS_XYZv = np.zeros((1, 4))

    def coordinates(self):
        for i in range(86400):
            print(i)
            self.XYZv[0, 0] = i
            self.WGS_XYZv[0, 0] = i
            t_k = self.time[i] - t_oe

            if t_k > 302400:
                t_k -= 604800
            if t_k < -302400:
                t_k += 604800

            n_0 = math.sqrt(mu / A ** 3)
            n = n_0 + Dn

            E_old = M_0 + n * t_k

            E_old = E_old
            E_new = E_old + e_n * math.sin(E_old)
            i = 1
            while abs(E_new - E_old) > 1e-8:
                E_old = E_new
                E_new = E_old + e_n * math.sin(E_old)
                i += 1
                if (i > 10):
                    break
            E_k = E_new

            cosNu_k = (math.cos(E_k) - e_n) / (1 - e_n * math.cos(E_k))
            sinNu_k = (math.sqrt(1 - e_n ** 2) * math.sin(E_k)) / (1 - e_n * math.cos(E_k))

            Nu_k = math.atan2(sinNu_k, cosNu_k)
            Phi_k = Nu_k + omega

            delta_u_k = Cus * math.sin(2 * Phi_k) + Cuc * math.cos(2 * Phi_k)
            delta_r_k = Crs * math.sin(2 * Phi_k) + Crc * math.cos(2 * Phi_k)
            delta_i_k = Cis * math.sin(2 * Phi_k) + Cic * math.cos(2 * Phi_k)

            u_k = Phi_k + delta_u_k
            r_k = A * (1 - e_n * math.cos(E_k)) + delta_r_k
            i_k = i_0 + iDot * t_k + delta_i_k

            x_k_shtrih = r_k * math.cos(u_k)
            y_k_shtrih = r_k * math.sin(u_k)

            Omega_k = Omega_0 + (Omega_dot - Omega_e_Dot) * t_k - Omega_e_Dot * t_oe

            x_k = x_k_shtrih * math.cos(Omega_k) - y_k_shtrih * math.cos(i_k) * math.sin(Omega_k)
            y_k = x_k_shtrih * math.sin(Omega_k) + y_k_shtrih * math.cos(i_k) * math.cos(Omega_k)
            z_k = y_k_shtrih * math.sin(i_k)
            self.WGS_XYZv[0, 1] = x_k
            self.WGS_XYZv[0, 2] = y_k
            self.WGS_XYZv[0, 3] = z_k
            self.WGS_XYZ = np.vstack((self.WGS_XYZ, self.WGS_XYZv))

            X = x_k * math.cos(Omega_k) + y_k * math.sin(Omega_k)
            Y = -x_k * math.sin(Omega_k) + y_k * math.cos(Omega_k)
            Z = z_k

            self.XYZv[0, 1] = X
            self.XYZv[0, 2] = Y
            self.XYZv[0, 3] = Z
            self.XYZ = np.vstack((self.XYZ, self.XYZv))
        np.savetxt("XYZ.txt", self.XYZ)
        np.savetxt("WGS_XYZ.txt", self.WGS_XYZ)


if __name__ == '__main__':

    gps_pi = 3.1415926535898
    toRad = gps_pi / 180
    sec2rad = gps_pi / (3600 * 180)
    min2rad = gps_pi / (60 * 180)
    mu = 3.986005e14
    Omega_e_Dot = 7.2921151467e-5
    Omega_ref_Dot = -2.6e-9

    # LNAV    Ephemeris(slot=221473810) =
    Crs = -14.7812
    Dn = 1.36379e-09 * toRad
    M_0 = -142.99 * toRad
    Cuc = -8.08388e-07
    e_n = 0.00169565
    Cus = 1.0509e-05
    A = 5153.65 ** 2
    t_oe = 93600
    Cic = -1.49012e-08
    Omega_0 = 11.8269 * toRad
    Cis = 4.09782e-08
    i_0 = 55.0915 * toRad
    Crc = 178.406
    omega = -175.597 * toRad
    Omega_dot = -4.4271e-07 * toRad
    iDot = 2.93244e-08 * toRad
    Tgd = 0
    toc = 93600
    af2 = 0
    af1 = 2.27374e-12
    af0 = -0.00018969
    WN = 149
    IODC = 588
    URA = 0
    Health = 0
    IODE2 = 76
    IODE3 = 76
    codeL2 = 1
    L2P = 1
    begin_time = 86382
    end_time = 172782
    step = 1
    # Создадим массив времени
    t = np.arange(begin_time, end_time, step, int)
    # Найдем координаты на интервале времени
    moment1 = TakeCoordinate(t)
    #moment1.coordinates()
    moment1.XYZ = np.loadtxt("XYZ.txt")
    moment1.WGS_XYZ = np.loadtxt("WGS_XYZ.txt")
    # print(moment1.XYZ)
    # Переведем в радианы координата приемника
    N_gr = 44
    N_min = 9
    N_sec = 36.3261
    N = N_gr * toRad + N_min * min2rad + N_sec * sec2rad

    E_gr = 39
    E_min = 00
    E_sec = 13.0546
    E = E_gr * toRad + E_min * min2rad + E_sec * sec2rad

    H = 1.247

    # найдем x
    SKP = np.zeros((1, 3))
    SKPF = np.zeros((1, 3))
    # for i in range(86400):
    #     Vrem = pymap3d.ecef2enu(moment1.WGS_XYZ[i, 1], moment1.WGS_XYZ[i, 2], moment1.WGS_XYZ[i, 3], N, E, H, deg = False)
    #     SKP[0, 0] = Vrem[0]
    #     SKP[0, 1] = Vrem[1]
    #     SKP[0, 2] = Vrem[2]
    #     SKPF = np.vstack((SKPF, SKP))
    #     print(i)
    # np.savetxt("SKPF.txt", SKPF)
    SKPF = np.loadtxt("SKPF.txt")
    RFT = np.zeros((1, 3))
    RFTF = np.zeros((1, 3))

    for i in range(86400):
        print(i)
        if SKPF[i, 2] > 0:
            RFT[0, 0] = math.sqrt(SKPF[i, 0] ** 2 + SKPF[i, 1] ** 2 + SKPF[i, 2] ** 2)
            RFT[0, 1] = math.acos(SKPF[i, 2] / RFT[0, 0])
            if SKPF[i, 0] > 0:
                RFT[0, 2] = -math.atan(SKPF[i, 1] / SKPF[i, 0]) + gps_pi / 2

            elif ((SKPF[i, 0] < 0) and (SKPF[i, 1] > 0)):
                RFT[0, 2] = -math.atan(SKPF[i, 1] / SKPF[i, 0]) + 3 * gps_pi / 2

            elif ((SKPF[i, 0] < 0) and (SKPF[i, 1] < 0)):
                RFT[0, 2] = -math.atan(SKPF[i, 1] / SKPF[i, 0]) - gps_pi / 2
        else:
            RFT[0, 1] = np.nan
            RFT[0, 0] = np.nan
            RFT[0, 2] = np.nan
        RFTF = np.vstack((RFTF, RFT))

    plt.subplot(111, polar=True)  # Полярная система координат


    plt.plot(RFTF[0:86400, 2], RFTF[0:86400, 1]/toRad, lw=2)

    plt.show()
    #Найдем xyz приемника

    X_RCV = 6400000 * math.cos(N)*math.cos(E)

    Y_RCV = 6400000 * math.cos(N)*math.sin(E)

    Z_RCV = 6400000*math.sin(N)


    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    m = 0
    for i in range(85):
        m += 1000
        ax.scatter(moment1.WGS_XYZ[m, 1], moment1.WGS_XYZ[m, 2], moment1.WGS_XYZ[m, 3], s=5)
    print(X_RCV,Y_RCV,Z_RCV)
    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)

    x = 6400000 * np.outer(np.cos(u), np.sin(v))
    y = 6400000 * np.outer(np.sin(u), np.sin(v))
    z = 6400000 * np.outer(np.ones(np.size(u)), np.cos(v))

    ax.plot_surface(x, y, z, rstride=4, cstride=4, color='b')
    ax.set_xlim3d(-3e7, 3e7)
    ax.set_ylim3d(-3e7, 3e7)
    ax.set_zlim3d(-3e7, 3e7)
    plt.show()

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    m = 0
    for i in range(85):
        m += 1000
        ax.scatter(moment1.XYZ[m, 1], moment1.XYZ[m, 2], moment1.XYZ[m, 3], s=5)

    u = np.linspace(0, 2 * np.pi, 100)
    v = np.linspace(0, np.pi, 100)

    x = 6400000 * np.outer(np.cos(u), np.sin(v))
    y = 6400000 * np.outer(np.sin(u), np.sin(v))
    z = 6400000 * np.outer(np.ones(np.size(u)), np.cos(v))

    ax.plot_surface(x, y, z, rstride=4, cstride=4, color='b')
    ax.set_xlim3d(-3e7, 3e7)
    ax.set_ylim3d(-3e7, 3e7)
    ax.set_zlim3d(-3e7, 3e7)
    plt.show()

