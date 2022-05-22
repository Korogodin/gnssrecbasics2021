clc 
clear all
close all hidden
close all force

set(0,'defaultTextFontSize', 14)                        % Default Font Size
set(0,'defaultAxesFontSize', 14)                        % Default Font Size
set(0,'defaultAxesFontName','Times')                    % Default Font Type
set(0,'defaultTextFontName','Times')                    % Default Font Type
set(0,'defaultFigurePaperPositionMode','auto')          % Default Plot position
set(0,'DefaultFigurePaperType','<custom>')              % Default Paper Type
set(0,'DefaultFigurePaperSize',[14.5 7.7])            	% Default Paper Size
format longE

%% Константы
pi= 3.1415926535898; % Отношение длины окружности к ее диаметру
mu = 3.986004418*1e14; % Геоцентрическая гравитационная постоянная 
omega_E = 7.2921151467*1e-5; % Средняя угловая скорость Земли
c = physconst('LightSpeed'); % Скорость света
Earth_radius = physconst('EarthRadius'); % Радиус Земли

%% Эфемериды в соответствии с моделью GPS
C_rs = 7.750000e+000; % Амплитуда поправочного члена синусоидальной гармоники к радиусу орбиты
delta_n = deg2rad(2.55652e-7); % Среднее отклонение движения от вычисленного значения
M_0 = deg2rad(-7.133039); % Средняя аномалия в контрольный момент времени
C_uc = 5.867332e-007; % Амплитуда поправочного члена косинусной гармоники к аргументу широты
ecc = 1.275745e-002; % Эксцентриситет
C_us = 9.344891e-006; % Амплитуда поправочного члена синусоидальной гармоники к аргументу широты
aSqRoot = 5.153602e+003; % Поправка к большой полуоси
t_oe = 100800; % Опорное время эфемерид
C_ic = 6.891787e-008  ; % Амплитуда поправочного члена косинусной гармоники к углу наклона
Omega0 = deg2rad(135.039698); % Долгота восходящего узла
C_is = 1.844019e-007; % Амплитуда поправочного члена синусоидальной гармоники к углу наклона
i0 = deg2rad(55.591104); % Наклонение в контрольный момент времени относительно i0 = 56 град
C_rc = 2.056875e+002; % Амплитуда поправочного члена косинусной гармоники к радиусу орбиты
omega = deg2rad(40.568872); % Аргумент перигея
OmegaDot = deg2rad(-4.556637e-007); % Скорость прямого восхождения
iDot = deg2rad(-2.312390e-008); % Скорость угла наклона


%% Расчёт Кеплеровских элементов орбиты
A = aSqRoot^2; % Большая полуось
n_0 = sqrt(mu/A^3); % Расчетное среднее движение

%% Алгоритм расчёта координат
leapSeconds = 18; % Секунды координации
daySecondsCount = 86400;

for ind = 1:daySecondsCount
t = daySecondsCount + leapSeconds + ind; % Количество секунд от начала текущей недели
t_k = t - t_oe; % Время от опорной эпохи эфемерид

    if t_k > 302400
        
        t_k = t_k - 604800;
        
    elseif t_k < -302400
        
        t_k = t_k + 604800;
        
    end

n = n_0 + delta_n; % Скорректированное среднее движение
M_k = M_0 + n * t_k; % Средняя аномалия
E_0 = M_k;
E_k = 0;
count = 0;

while true
    E_k = E_0;
    E_0 = E_0 + (M_k - E_0 + ecc * sin(E_0))/(1 - ecc * cos(E_0)); 
    if abs(E_0-E_k) < 1e-8
        break
    end
    count = count + 1;
end

nu = atan2((sqrt(1 - ecc^2) * sin(E_k)/(1 - ecc * cos(E_k))), ((cos(E_k) - ecc)/(1 - ecc * cos(E_k)))); % Истинная аномалия
Phi = nu + omega; % Аргумент широты
delta_u = C_us*sin(2*Phi) + C_uc*cos(2*Phi); % Аргумент поправки на широту
delta_r = C_rs*sin(2*Phi) + C_rc*cos(2*Phi); % Коррекция радиуса
delta_i = C_is*sin(2*Phi) + C_ic*cos(2*Phi); % Коррекция наклона
u_k = Phi + delta_u; % Скорректированный аргумент широты
r = A * (1 - ecc * cos(E_k)) + delta_r; % Скорректированный радиус
i_corr = i0 + delta_i + iDot * t_k; % Скорректированное наклонение

x_shtr = r * cos(u_k);  
y_shtr = r * sin(u_k);

Omega_corr = Omega0 + (OmegaDot - omega_E) * t_k - omega_E * t_oe; % Скорректированная широта восходящего узла

x(ind) = x_shtr * cos(Omega_corr) - y_shtr * cos(i_corr) * sin(Omega_corr); % Координаты КА
y(ind) = x_shtr * sin(Omega_corr) + y_shtr * cos(i_corr) * cos(Omega_corr);
z(ind) = y_shtr * sin(i_corr);

% Инерциальная СК
Theta = omega_E * t_k;
x_ics(ind) = x(ind) * cos(Theta) - y(ind) * sin(Theta);
y_ics(ind) = x(ind) * sin(Theta) + y(ind) * cos(Theta);
z_ics(ind) = z(ind);

% Вычисление координат приёмника
lat = deg2rad(44.09363261); % Широта
lon = deg2rad(39.00130546); % Долгота
H = 1.247; % Высота

x_point = (Earth_radius + H)*cos(lat)*cos(lon);  % Расчёт координат приёмника
y_point = (Earth_radius + H)*cos(lat)*sin(lon);
z_point = (Earth_radius + H)*sin(lon);

[xRezult(ind), yRezult(ind), zRezult(ind)] = ecef2enu(x(ind),y(ind),z(ind),lat,lon,H,wgs84Ellipsoid,'radians'); % Отображение координат КА относительно передатчика

% Полярная СК
    if zRezult(ind) > 0
    rho(ind) = norm([xRezult(ind),yRezult(ind),zRezult(ind)]); 
    theta(ind) = acos(zRezult(1,ind)/rho(ind));
        if xRezult(ind) > 0
            phi(ind) = -atan(yRezult(ind)/xRezult(ind))+pi/2;
        elseif (xRezult(ind)<0)&&(yRezult(ind)>0)
            phi(ind) = -atan(yRezult(ind)/xRezult(ind))+3*pi/2;
        elseif (xRezult(ind)<0)&&(yRezult(ind)<0)
            phi(ind) = -atan(yRezult(ind)/xRezult(ind))-pi/2;
        end
    else
        rho(ind) = nan;
        theta(ind) = nan;
        phi(ind) = nan;
    end
end

%% Вывод значений координат в файл out.txt
outTxt = fopen('out.txt', 'w'); 
formatSpec = '%1.0f %10.8f %10.8f %10.8f\n';
for indCount = 1:length(x)
    fprintf(outTxt, formatSpec, indCount, x(indCount), y(indCount), z(indCount));
end
fclose(outTxt);
type out.txt

%% Расчёт для построения Земли
[x_sphere,y_sphere,z_sphere] = sphere(50);
x_Earth = Earth_radius*x_sphere; 
y_Earth = Earth_radius*y_sphere; 
z_Earth = Earth_radius*z_sphere; 

%% Построение графиков
figure(1)
subplot(1,1,1)
surf(x_Earth,y_Earth,z_Earth)
hold on
plot3(x, y, z)
plot3(x_point,y_point ,z_point,'o','Color' , 'r', 'MarkerSize' , 5, 'MarkerFaceColor','r') % Построение точки
xlabel('x, м')
ylabel('y, м')
zlabel('z, м')
daspect([1,1,1])

figure(2)
subplot(1,1,1)
surf(x_Earth,y_Earth,z_Earth)
hold on
plot3(x_ics(1,:), y_ics(1,:), z_ics(1,:))
plot3(x_point,y_point ,z_point,'o','Color' , 'r', 'MarkerSize' , 5, 'MarkerFaceColor','r') % Построение точки
xlabel('x, м')
ylabel('y, м')
zlabel('z, м')
daspect([1,1,1])

figure (3)
axes = polaraxes;
polarplot(axes,phi,rad2deg(theta))
axes.ThetaDir = 'clockwise';
axes.ThetaZeroLocation = 'top';
rlim([0 80]) % Учёт угла места
