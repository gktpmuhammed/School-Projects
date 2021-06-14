[y,Fs] = audioread('faultyphone.wav');  % Creating audio signal from audio file
info = audioinfo('faultyphone.wav');    % Storing the audio info of audio file
% sound(y,Fs);  % playing the sound

ytran = fft(y); %discrete fourier transform
n = length(y);  % number of samples
fshift = (-n/2:n/2-1)*(Fs/n);   % 0-centered frequency range
yshift = fftshift(ytran);       % shift y values

%%%%%%%%%%%% Plotting the signal w.r.t zero centered frequency %%%%%%%%%%%%
subplot(3,1,1)
plot(fshift,abs(yshift)/n)
xlabel('Frequency (Hz)')
ylabel('Amplitude')
title('faultyphone.wav');
%%%%%%%%%%%% Plotting the signal w.r.t zero centered frequency %%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%% Plotting the signal w.r.t frequency %%%%%%%%%%%%%%%%%%
% The tall lines at the beginning and at the end are the places that we
% need to filter. And these values are 1736 and 187535. So in ytran wave we
% need to make zero the ytran(1736) and ytran(1875535).
subplot(3,1,2)
plot(abs(ytran))
xlabel('Frequency (Hz)')
ylabel('Amplitude')
%%%%%%%%%%%%%%%%%%%% Plotting the signal w.r.t frequency %%%%%%%%%%%%%%%%%%


ytran(1736)=0;  % We are making the power of this frequency zero to eliminate noise.
ytran(187535)=0;% We are making the power of this frequency zero to eliminate noise.
yfinal=ifft(ytran); % Then we are moving from frequency domain to time domain. 
% sound(yfinal,Fs);
audiowrite('filteredphone.wav',yfinal,Fs); % Writing filtered sound to a file

filtered_y = fft(yfinal);   % discrete fourier transform for filtered sound
new_N = length(yfinal);     % number of samples
filtered_shift = (-new_N/2:new_N/2-1)*(Fs/new_N);   % 0-centered frequency range
filtered_yshift = fftshift(filtered_y);             % shift y values

%%%%%%%% Plotting the filtered signal w.r.t zero centered frequency %%%%%%%
subplot(3,1,3)
plot(fshift,abs(filtered_yshift)/new_N)
xlabel('Frequency (Hz)')
ylabel('Amplitude')
title('After Filtering');
%%%%%%%% Plotting the filtered signal w.r.t zero centered frequency %%%%%%%

