%
% Lab Group 11
% Mini Project Simulation and Control 
%
% This script experimentally defines motor parameters, runs the simulation MiniProjTuning.xls and
% plots the results
%
%% Experimental Data
close all;
data=csvread('angularVelocityDataMiniProj.csv');
x=data(:,1);
y=data(:,3);
%% Setting Tuning Variables
K=6.4;
sigma=15;
%% Simulink Motor Modeling
open_system('MiniProjTuning');
out=sim('MiniProjTuning');
figure(1)
plot(out.simout);
figure(2)
xlabel('X coordinates');
ylabel('Y coordinates');
title('step response');
plot(x,y);
%
% The graphical results of both the simulated and experimental motor step
% responses are shown above, and as can be seen, the match as closely as
% possible given the accuracy of the experimental data, accounting for the
% late start of the simulink model output

%% Simulink PI Model
open_system('MiniProjMotor');

%
%The PI values of Kp=.2275 and Ki=.05174 were obtained using a PI controler
%and the inbuilt PID controler tuning functions provided by simulink
%

