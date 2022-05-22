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

%% ���������
pi= 3.1415926535898; % ��������� ����� ���������� � �� ��������
mu = 3.986004418*1e14; % ��������������� �������������� ���������� 
omega_E = 7.2921151467*1e-5; % ������� ������� �������� �����
c = physconst('LightSpeed'); % �������� �����
Earth_radius = physconst('EarthRadius'); % ������ �����

%% ��������� � ������������ � ������� GPS
C_rs = 7.750000e+000; % ��������� ������������ ����� �������������� ��������� � ������� ������
delta_n = deg2rad(2.55652e-7); % ������� ���������� �������� �� ������������ ��������
M_0 = deg2rad(-7.133039); % ������� �������� � ����������� ������ �������
C_uc = 5.867332e-007; % ��������� ������������ ����� ���������� ��������� � ��������� ������
ecc = 1.275745e-002; % ��������������
C_us = 9.344891e-006; % ��������� ������������ ����� �������������� ��������� � ��������� ������
aSqRoot = 5.153602e+003; % �������� � ������� �������
t_oe = 100800; % ������� ����� ��������
C_ic = 6.891787e-008  ; % ��������� ������������ ����� ���������� ��������� � ���� �������
Omega0 = deg2rad(135.039698); % ������� ����������� ����
C_is = 1.844019e-007; % ��������� ������������ ����� �������������� ��������� � ���� �������
i0 = deg2rad(55.591104); % ���������� � ����������� ������ ������� ������������ i0 = 56 ����
C_rc = 2.056875e+002; % ��������� ������������ ����� ���������� ��������� � ������� ������
omega = deg2rad(40.568872); % �������� �������
OmegaDot = deg2rad(-4.556637e-007); % �������� ������� �����������
iDot = deg2rad(-2.312390e-008); % �������� ���� �������


%% ������ ������������ ��������� ������
A = aSqRoot^2; % ������� �������
n_0 = sqrt(mu/A^3); % ��������� ������� ��������

%% �������� ������� ���������
leapSeconds = 18; % ������� �����������
daySecondsCount = 86400;

for ind = 1:daySecondsCount
t = daySecondsCount + leapSeconds + ind; % ���������� ������ �� ������ ������� ������
t_k = t - t_oe; % ����� �� ������� ����� ��������

    if t_k > 302400
        
        t_k = t_k - 604800;
        
    elseif t_k < -302400
        
        t_k = t_k + 604800;
        
    end

n = n_0 + delta_n; % ����������������� ������� ��������
M_k = M_0 + n * t_k; % ������� ��������
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

nu = atan2((sqrt(1 - ecc^2) * sin(E_k)/(1 - ecc * cos(E_k))), ((cos(E_k) - ecc)/(1 - ecc * cos(E_k)))); % �������� ��������
Phi = nu + omega; % �������� ������
delta_u = C_us*sin(2*Phi) + C_uc*cos(2*Phi); % �������� �������� �� ������
delta_r = C_rs*sin(2*Phi) + C_rc*cos(2*Phi); % ��������� �������
delta_i = C_is*sin(2*Phi) + C_ic*cos(2*Phi); % ��������� �������
u_k = Phi + delta_u; % ����������������� �������� ������
r = A * (1 - ecc * cos(E_k)) + delta_r; % ����������������� ������
i_corr = i0 + delta_i + iDot * t_k; % ����������������� ����������

x_shtr = r * cos(u_k);  
y_shtr = r * sin(u_k);

Omega_corr = Omega0 + (OmegaDot - omega_E) * t_k - omega_E * t_oe; % ����������������� ������ ����������� ����

x(ind) = x_shtr * cos(Omega_corr) - y_shtr * cos(i_corr) * sin(Omega_corr); % ���������� ��
y(ind) = x_shtr * sin(Omega_corr) + y_shtr * cos(i_corr) * cos(Omega_corr);
z(ind) = y_shtr * sin(i_corr);

% ������������ ��
Theta = omega_E * t_k;
x_ics(ind) = x(ind) * cos(Theta) - y(ind) * sin(Theta);
y_ics(ind) = x(ind) * sin(Theta) + y(ind) * cos(Theta);
z_ics(ind) = z(ind);

% ���������� ��������� ��������
lat = deg2rad(44.09363261); % ������
lon = deg2rad(39.00130546); % �������
H = 1.247; % ������

x_point = (Earth_radius + H)*cos(lat)*cos(lon);  % ������ ��������� ��������
y_point = (Earth_radius + H)*cos(lat)*sin(lon);
z_point = (Earth_radius + H)*sin(lon);

[xRezult(ind), yRezult(ind), zRezult(ind)] = ecef2enu(x(ind),y(ind),z(ind),lat,lon,H,wgs84Ellipsoid,'radians'); % ����������� ��������� �� ������������ �����������

% �������� ��
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

%% ����� �������� ��������� � ���� out.txt
outTxt = fopen('out.txt', 'w'); 
formatSpec = '%1.0f %10.8f %10.8f %10.8f\n';
for indCount = 1:length(x)
    fprintf(outTxt, formatSpec, indCount, x(indCount), y(indCount), z(indCount));
end
fclose(outTxt);
type out.txt

%% ������ ��� ���������� �����
[x_sphere,y_sphere,z_sphere] = sphere(50);
x_Earth = Earth_radius*x_sphere; 
y_Earth = Earth_radius*y_sphere; 
z_Earth = Earth_radius*z_sphere; 

%% ���������� ��������
figure(1)
subplot(1,1,1)
surf(x_Earth,y_Earth,z_Earth)
hold on
plot3(x, y, z)
plot3(x_point,y_point ,z_point,'o','Color' , 'r', 'MarkerSize' , 5, 'MarkerFaceColor','r') % ���������� �����
xlabel('x, �')
ylabel('y, �')
zlabel('z, �')
daspect([1,1,1])

figure(2)
subplot(1,1,1)
surf(x_Earth,y_Earth,z_Earth)
hold on
plot3(x_ics(1,:), y_ics(1,:), z_ics(1,:))
plot3(x_point,y_point ,z_point,'o','Color' , 'r', 'MarkerSize' , 5, 'MarkerFaceColor','r') % ���������� �����
xlabel('x, �')
ylabel('y, �')
zlabel('z, �')
daspect([1,1,1])

figure (3)
axes = polaraxes;
polarplot(axes,phi,rad2deg(theta))
axes.ThetaDir = 'clockwise';
axes.ThetaZeroLocation = 'top';
rlim([0 80]) % ���� ���� �����
