clc 
clear all
close all 

% Константы из ИКД
pi = 3.1415326535898;
mu = 3.986005e+14; % Гравитационная постоянная
Omega_e_dot = 7.2921151467e-5; % Скорость вращения Земли

% Эфемериды 
C_rs = -22.9688; % Амплитуда поправочного члена синусоидальной гармоники к радиусу орбиты
delta_n = 2.55652e-7; % Среднее отклонение движения от вычисленного значения
M_0 = deg2rad(-94.6511); % Средняя аномалия
C_uc = -1.21817e-6; % Амплитуда поправочного члена косинусной гармоники к аргументу широты
eks = 0.00225987; % Эксцентриситет
C_us = 1.06804e-5; % Амплитуда поправочного члена синусоидальной гармоники к аргументу широты
rootA = 5153.62; % Корень из большой полуоси
t_oe = 93600 + 18 - 7200; % Опорное время эфемерид за вычетом 2-х часов
C_ic = -2.42144e-8; % Амплитуда поправочного члена косинусной гармоники к углу наклона
Omega_0 = deg2rad(8.79464); % Долгота узла
C_is = 1.04308e-7; % Амплитуда поправочного члена синусоидальной гармоники к углу наклона
i_0 = deg2rad(54.6911); % Наклонение
C_rc = 170.656; % Амплитуда поправочного члена косинусной гармоники к радиусу орбиты
omega = deg2rad(107.109); % Аргумент перигея
Omega_dot = deg2rad(-4.49402e-7); % Скорость прямого восхождения
IDOT = 2.53544e-8; % Скорость угла наклона

% Расчет значений элементов Кеплера
A = rootA^2; % Большая полуось
n_0 = sqrt(mu/A^3); % Расчетное среднее движение

for i = 1:86400 % Суточный интервал рассмотрения
t = 93600 + 18 + i - 7200; % Количество секунд от начала текущей недели при начале рассмотрения в 00:00:01
% Время от опорной эпохи эфемерид
t_k = t - t_oe; 

if t_k>302400
 t_k = t_k - 604800;
elseif t_k<-302400
 t_k = t_k + 604800;
end

n = n_0 + delta_n; % Скорректированное среднее движение
M_k = M_0 + n * t_k; % Средняя аномалия
% Эксцентрическая аномалия
E_0 = M_k;
E_k = 0;
k = 0;

while abs(E_0 - E_k)>1e-8 
 E_k = E_0;
 E_0 = E_0 + (M_k - E_0 + eks * sin(E_0))/(1 - eks * cos(E_0));
 k = k + 1;
end

nu_k = atan2((sqrt(1 - eks^2) * sin(E_k)/(1 - eks * cos(E_k))), ((cos(E_k) - eks)/(1 - eks * cos(E_k)))); % Истинная аномалия
Phi_k = nu_k + omega; % Аргумент широты
delta_u_k = C_us*sin(2*Phi_k) + C_uc*cos(2*Phi_k); % Аргумент поправки на широту
delta_r_k = C_rs*sin(2*Phi_k) + C_rc*cos(2*Phi_k); % Коррекция радиуса
delta_i_k = C_is*sin(2*Phi_k) + C_ic*cos(2*Phi_k); % Коррекция наклона
u_k = Phi_k + delta_u_k; % Скорректированный аргумент широты
r_k = A * (1 - eks * cos(E_k)) + delta_r_k; % Скорректированный радиус
i_k = i_0 + delta_i_k + IDOT * t_k; % Скорректированное наклонение

% Позиция в орбитальной плоскости
x_k_sh = r_k * cos(u_k);
y_k_sh = r_k * sin(u_k);

Omega_k = Omega_0 + (Omega_dot - Omega_e_dot) * t_k - Omega_e_dot * t_oe; % Скорректированная широта восходящего узла

% Расчет координат спутника
x_k = x_k_sh * cos(Omega_k) - y_k_sh * cos(i_k) * sin(Omega_k);
y_k = x_k_sh * sin(Omega_k) + y_k_sh * cos(i_k) * cos(Omega_k);
z_k = y_k_sh * sin(i_k);

x_current(1,i) = x_k;
y_current(1,i) = y_k;
z_current(1,i) = z_k;

% Инерциальная система координат
theta = Omega_e_dot * t_k;
x_k_1 = x_k * cos(theta) - y_k * sin(theta);
y_k_1 = x_k * sin(theta) + y_k * cos(theta);
z_k_1 = z_k;

x_converted(1,i) = x_k_1;
y_converted(1,i) = y_k_1;
z_converted(1,i) = z_k_1;

% Полярная система координат
rho = sqrt((x_k).^2+(y_k).^2+(z_k).^2);
phi = atan2((y_k), (x_k));
theta_1 = acos((z_k)/rho);
theta_1_i(1,i) = theta_1*180/pi;
phi_1_i(1,i) = phi;
end

% Вывод значений координат
out = fopen('../out.txt', 'w+');
for i = 1:length(x_current)
    fprintf(out, '%6.0f %10.10f %10.10f %10.10f\n', i, x_current(1,i), y_current(1,i), z_current(1,i));
end
fclose(out);

% Вывод графиков
R = 6378136; % Радиус Земли
[x_sphere, y_sphere, z_sphere] = sphere(100);
x_Earth=R*x_sphere;
y_Earth=R*y_sphere;
z_Earth=R*z_sphere;

figure (1)
subplot (1,1,1)
surf(x_Earth,y_Earth,z_Earth);
hold on;
plot3(x_current(1,:), y_current(1,:), z_current(1,:));
xlabel('x, м','FontSize',16);
ylabel('y, м','FontSize',16);
zlabel('z, м','FontSize',16);

figure (2)
subplot (1,1,1)
surf(x_Earth,y_Earth,z_Earth);
hold on;
plot3(x_converted(1,:), y_converted(1,:), z_converted(1,:));
xlabel('x, м','FontSize',16);
ylabel('y, м','FontSize',16);
zlabel('z, м','FontSize',16);

figure (3)
axes = polaraxes;
polarplot(axes,phi_1_i,theta_1_i)
axes.ThetaDir = 'clockwise';
axes.ThetaZeroLocation = 'top';
rlim([0 50]);
