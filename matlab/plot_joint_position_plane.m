close all;
clear all;
clc;

%%

% genome = [2.5e+02 0.2 0.28 -0.61 0.15 2 0.9 0.97 0.84 0.071 0.62 0.26 0.24 0.53 0.15 -0.38 1.2];
% 
% T = genome(1);
% t = linspace(0, T, 1000);
% 
% theta1 = joint_function(t, genome(2), genome(3), genome(4), 0, T);
% theta2 = joint_function(t, genome(5), genome(6), genome(7), genome(8), T);

hipPitch = load('../bin/.__Test__LHipPitch3');
kneePitch = load('../bin/.__Test__LKneePitch3');
anklePitch = load('../bin/.__Test__LAnklePitch3');

t = hipPitch(:, 1);
hiptheta = hipPitch(:,2);
kneetheta = kneePitch(:,2);
% anklePitch = anklePitch(:, 2);

hiptheta_D = hipPitch(:,3);
kneetheta_D = kneePitch(:,3);
% anklePitch_D = anklePitch(:, 2);

figure;
plot(t, hiptheta);
figure;
plot(t, kneetheta);
figure;
plot(t, hiptheta_D);
figure;
plot(t, kneetheta_D);
figure;
% plot(t, anklePitch);
% figure;

hold on;
grid on;
% plot3(hiptheta, kneetheta, anklePitch);
% plot3(hiptheta_D, kneetheta_D, anklePitch_D);
plot(hiptheta, kneetheta);
plot(hiptheta_D, kneetheta_D);
