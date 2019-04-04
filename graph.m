%% TITLE BLOCK
% Ahmed Fuad Ali 
% 400075937
% alia78
% 2DP4 Final Project code to display angle
% Using Matlab R2019a 
% no additional libraries used

clear;
clc;

% start listening on the required ports
s = instrfind('Port', 'COM4');
delete(s);
clear s;
s = serial('COM4');
s.BaudRate = 19200;
s.Terminator = 'CR';
fopen(s);


figure;
hold on;
title("Ahmed Fuad Ali, 400075937");
yaxis=[];
xaxis=[];
x=0;

tic;

% start graphing based on input being read
while(1)
    input = fscanf(s);
    input = str2num(input);
    x=toc; % make sure we increment our clock as well to have time axis
    yaxis = [yaxis, input];
    xaxis = [xaxis, x];
    plot(xaxis, yaxis);
    pause(0.25); % this is the same delay in our codewarrior

end
