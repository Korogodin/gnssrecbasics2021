close all; 
clear all; 
clc;
format long
% Координаты приемника
B = deg2rad(44.09363261);
L = deg2rad(39.00130546);
H = 1.247;
% Эфемериды
SatNum = 14;
toe = 86400;
Crs = 8.812500e+00;
Dn =1.487138e-09;
M0 = deg2rad(-1.740945e+01);
Cuc = 4.526228e-07;
e = 1.435236e-03;
Cus = 9.626150e-06;
sqrtA = 5.153648e+03;
Cic = -1.173466e-07;
Omega0 = deg2rad(1.321879e+02);
Cis = -9.313226e-09;
i0 = deg2rad(5.464168e+01);
Crc = 1.896875e+02;
omega = deg2rad(1.728546e+02);
OmegaDot = deg2rad(-4.551725e-07);
iDot = deg2rad(-2.879233e-08);
Tgd = -7.450581e-09;
toc = 86400;
af2 = 0;
af1 = -4.547474e-12;
af0 = -8.354103e-05;
URA = 0;
IODE = 23;
IODC = 23;
codeL2 = 1;
L2P = 0;
WN = 149;
% Значения констант
mu = 3.986004418e14;    % гравитационная постоянная
omega_e = 7.2921151467e-5;   % скорость вращения
% Временной промежуток
begin_time = 0; % время начала 0:00 по МСК 14 февраля
end_time = 24*60*60; % время окончания 0:00 по МСК 15 февраля
% Длина временного промежутка
t_arr = begin_time:1:end_time;
% Большая полуось
A = sqrtA^2;
% Среднее движение
n0 = sqrt(mu/A^3);
n = n0+Dn;
for k = 1:86400
% Время
    t(k) = t_arr(k)-toe;
    if t(k) > 302400
        t(k) = t(k)-604800;
    
    elseif t(k) < -302400
        t(k) = t(k)+604800;
    end
% Средняя аномалия
    M(k) = M0+n*t(k);
% Решение уравнения Кеплера
E(k) = M(k);
E_old(k) = M(k)+1;
epsilon = 1e-6;
    while abs(E(k)-E_old(k)) > epsilon
        E_old(k) = E(k);
        E(k) = M(k)+e*sin(E(k));
    end
% Истинная аномалия
    nu(k) = 2*atan(sqrt((1+e)/(1-e))*tan(E(k)/2));
% Коэффициент коррекции
cos_correction(k) = cos(2*(omega+nu(k)));
sin_correction(k) = sin(2*(omega+nu(k)));
  
% Аргумент широты
u(k) = omega+nu(k)+Cuc*cos_correction(k)+Cus*sin_correction(k);
% Радиус
r(k) = A*(1-e*cos(E(k)))+Crc*cos_correction(k)+Crs*sin_correction(k);
% Наклон
i(k) = i0+iDot*t(k)+Cic*cos_correction(k)+Cis*sin_correction(k);
% Долгота восходящего угла
lambda(k) = Omega0+(OmegaDot-omega_e)*t(k)-omega_e*toe;
% Положение на орбите
x = r(k)*cos(u(k));
y = r(k)*sin(u(k));
% Координаты
X0(k) = x*cos(lambda(k))-y*cos(i(k))*sin(lambda(k));
Y0(k) = x*sin(lambda(k))+y*cos(i(k))*cos(lambda(k));
Z0(k) = y*sin(i(k));
X(k) = X0(k)*cos(lambda(k))+Y0(k)*sin(lambda(k));
Y(k) = -X0(k)*sin(lambda(k))+Y0(k)*cos(lambda(k));
Z(k) = Z0(k);

[x(k), y(k), z(k)] = ecef2enu(X0(k), Y0(k), Z0(k), B, L, H, wgs84Ellipsoid, 'radians');
if z(k) > 0
    rho(k) = sqrt(x(k)^2 + y(k)^2 + z(k)^2);
    theta1(k) = acos(z(k)/rho(k));
    if x(k) > 0
        phi(k) = -atan(y(k)/x(k)) + pi/2;
    elseif (x(k) < 0)&&(y(k) > 0)
        phi(k) = -atan(y(k)/x(k))+3*pi/2;
    elseif (x(k)<0)&&(y(k)<0)
        phi(k) = -atan(y(k)/x(k))-pi/2;
    end
 else theta1(k) = NaN;
     rho(k) = NaN;
     phi(k) = NaN;
end
end
%График
figure (1)
axes = polaraxes;
polarplot(axes,phi,rad2deg(theta1))
axes.ThetaDir = 'clockwise';
axes.ThetaZeroLocation = 'top';
rlim([0 80]);

