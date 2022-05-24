clear all; close all;clc;
tic;

Toe = 93600;
Crs = 132.031;
mu = 3.986004418e+14;
dn = 2.24691*10e-7;
Cuc = 6.99982e-06;
e = 0.01361771;
Cus = 7.82311e-05;
A = 5153.76^2;
Cic = 1.73226e-7;
Wo = -166.61;
Cis = 1.91852e-07;
Io = 56.1608;
Crc = 237.844;
Mo = -120.70525;
We = 7.2921150e-5;
W = -85.8694;
Wdot = -4.34832e-07;
idot = 1.14801e-08;
T = 86400+17 +2*60;
wur = 55.45241057;
dl = 37.42127420;
H = 193;
no = sqrt(mu/(A^3));
n=no+dn;

for j=1:86400

    Tk=T-Toe;
    if Tk>302400
        Tk = Tk - 604800;
    elseif Tk<-302400
        Tk = Tk + 604800;
    end

    M = Mo+n*Tk;
    E=0;
    Ek=1;

    while (abs(Ek - E) > 0.0000001)
        Ek=E;
        E = M + e * sin(E);
    end

    nu = atan2(sqrt(1-e^2)*sin(E),cos(E)-e);
    F1 = nu+W;
    du = Cus*sin(2*F1)+Cuc*cos(2*F1);
    dr = Crs*sin(2*F1)+Crc*cos(2*F1);
    di = Cis*sin(2*F1)+Cic*cos(2*F1);

    F2 = F1+du;
    r = A*(1-e*cos(E))+dr;
    i = Io+di+idot*Tk;
    poX = r*cos(F2);
    poY = r*sin(F2);

    Omega = Wo+(Wdot-We)*(Tk)-We*Toe;
    x = poX*cos(Omega)-poY*cos(i)*sin(Omega);
    y = poX*sin(Omega)+poY*cos(i)*cos(Omega);
    z = poY*sin(i);

    Resfix(j,:) = [x y z];
    phi = We*Tk;
    xc = x*cos(phi)-y*sin(phi);
    yc = x*sin(phi)+y*cos(phi);
    zc = z;
    ResECI(j,:)=[xc yc zc];
    [East, North, Up] = ecef2enu(x, y, z, wur, dl,H, wgs84Ellipsoid);

    R = sqrt(East^2 + North^2 + Up^2);
    el(j) = rad2deg(-asin(Up/R))+90;
    az(j) = atan2(East, North);
    T=T+1;
end

toc;
text = fopen('matlab_out.txt', 'w+');

for i = 1:length(Resfix(:,1))
    fprintf(text,'%6.0d %6.6f %6.6f %6.6f\n', i, Resfix(i,1), Resfix(i,2), Resfix(i,3));
end
fclose(text);

[X, Y, Z] = sphere(10);
figure;plot3(Resfix(:,1),Resfix(:,2),Resfix(:,3));
hold on;
surf(X*6.371*10^6, Y*6.371*10^6, Z*6.371*10^6);
grid on;
xlabel('X,м');
ylabel('Y,м');
zlabel('Z,м');
title('ECEF WGS84');
figure; plot3(ResECI(:,1),ResECI(:,2),ResECI(:,3));
hold on;
surf(X*6.371*10^6, Y*6.371*10^6, Z*6.371*10^6);
grid on;
xlabel('X,м');
ylabel('Y,м');
zlabel('Z,м');
title('Инерциальная СК');
s = 1;

for y = 1:length(el)
    if el(y) <= 90
        Cel(s) = el(y);
        Caz(s) = az(y);
        s = s+1;
    end
end

figure;
polar(2*pi-Caz, Cel);
camroll(90);
grid on;
title('Полученный SKYVIEW');
