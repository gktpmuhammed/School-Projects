%%%%%%%%%%%%%%%%%% This part creates original signal %%%%%%%%%%%%%%%%%%%%%%
fs = 100;
t = -10:1/fs:10-1/fs;
x = sawtooth(2*pi*0.1*t+pi);
%%%%%%%%%%%%%%%%%% This part creates original signal %%%%%%%%%%%%%%%%%%%%%%


h3 = sin(0*t);  % 0 signal for sum of first 3 harmonics
h5 = sin(0*t);  % 0 signal for sum of first 5 harmonics
h15 = sin(0*t); % 0 signal for sum of first 15 harmonics

% This loop computes the first 15 harmonics
for c = 1:15
    % Since the fourier cooefficents are different for even and odd terms
    % If it is a odd term harmonic then it's coefficient ak = (1/(pi*j*k)
    if mod(c,2) == 1
        h = (1/(pi*j*c)*exp(j*2*pi*c*0.1*t)) + (-1/(pi*j*c)*exp(j*2*pi*c*(-0.1)*t));
    % If it is a even term harmonic then it's coefficient ak = (-1/(pi*j*k)
    else
        h = (-1/(pi*j*c)*exp(j*2*pi*c*0.1*t)) + (1/(pi*j*c)*exp(j*2*pi*c*(-0.1)*t));
    end
    
    % This if statement sums the first 3 harmonics
    if c < 4
        h3 = h3 + h;     
    % These two if statements add 4th and 5th harmonics to h3. 
    % h5 is the sum of first 5 harmonics
    elseif c == 4
        h5 = h3 + h;
    elseif c == 5
        h5 = h5 + h;
    % These last two if statements sums the remaning harmonics until 15th
    % h15 is the sum of first 15 harmonics
    elseif c == 6
        h15 = h5 + h;
    else
        h15 = h15 + h;
    end
    
end


subplot(1,1,1);
plot(t,x,t,h3,t,h5,t,h15); % Plots the waves to the figure
legend("original","sum of first 3 harmonics","sum of first 5 harmonics","sum of first 15 harmonics"); % shows which color represents which wave
xlabel('time');
ylabel('amplitude');
title('Harmonic Summation');



