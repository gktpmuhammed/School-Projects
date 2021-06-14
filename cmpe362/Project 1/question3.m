[x,Fs] = audioread('enginesound.m4a');  % Creating audio signal from audio file
info = audioinfo('enginesound.m4a');    % Storing the audio info of audio file
% sound(x,Fs);   % playing the sound
t = 0:seconds(1/Fs):seconds(info.Duration); % finding the duration of the sound
t = t(1:end-1);

%%%%%%%%%%%%%%%%%%%%%%% Plots original sound signal%%%%%%%%%%%%%%%%%%%%%%%%
subplot(3,1,1);
plot(t,x);
xlabel('Time')
ylabel('Amplitude')
title('Original Sound Signal');
%%%%%%%%%%%%%%%%%%%%%%% Plots original sound signal%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%% Filtering the original sound %%%%%%%%%%%%%%%%%%%%%%
% Creating the filter coefficient vectors. Since it is a 50 point avarager
% we need [1/50 1/50 1/50 1/50 .....   1/50] vector with size 50
a = 1;
c = 1/50;
b = c(ones(1, 50));
avg_x = filter(b,a,x);  % This function will filter the sound.
% sound(avg_x,Fs);
%%%%%%%%%%%%%%%%%%%%%%% Filtering the original sound %%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%% Plots filtered sound and spectrogram%%%%%%%%%%%%%%%
subplot(3,1,2);
plot(t,avg_x);
xlabel('Time')
ylabel('Amplitude')
title('Filtered Sound Signal');

subplot(3,1,3);
spectrogram(avg_x);
title('Spectrogram of Filtered Sound Signal');
%%%%%%%%%%%%%%%%%%%%%%% Plots filtered sound and spectrogram%%%%%%%%%%%%%%%

% y = fft(avg_x);
% n = length(avg_x);                         
% fshift = (-n/2:n/2-1)*(Fs/n);
% yshift = fftshift(y);
% 
% subplot(2,2,4)
% plot(fshift,abs(yshift)/n)
% xlabel('Frequency (Hz)')
% ylabel('Amplitude')

f0 = 0.0091 * Fs /2;    % Fundamental frequency obtained from spectrogram
N_stroke = 4;           % Given in question
N_cylinder = 4;         % given in question

N_rpm = (1/2) * (N_stroke/N_cylinder) * 60 * f0;    % formula to calculate RPM
