x = csvread('micro-control-data/motor_trial_mouse_1753.txt',1,0);
%% now compute distance travelled in sum

yl = x(:,3);
tl = x(:,4);

yr = x(:,6);
tr = x(:,7);

t = x(:,1);

yl = yl./tl;
yr = yr./tr;

sensorAngleDegrees = 75; % measured by kyle
sensorAngleRadians = (sensorAngleDegrees/360)*2*pi;

yl = (yl-yr*cos(sensorAngleRadians))/cos(pi/2-sensorAngleRadians);

velocity_cms = sqrt(yl.^2+yr.^2)*100;
%%

figure;
t = t/(10^6);
plot(t,velocity_cms);
xlabel('Time [s]');
ylabel('Speed [cm/s]');
xlim([180 260]);
title('Example session');
print(gcf,sprintf('figures/mouse_1753_speed_example.pdf'));

st.mn = mean(velocity_cms);
st.sd = std(velocity_cms);

%% now examine the timing of the digital pulses

d = 'micro-control-data/9-17-2018-Mike-1753-Virmen/Block-1';
data = TDTbin2mat(d);
fs = data.streams.Puls.fs;
data = data.streams.Puls.data;
taxis = (0:1:(length(data)-1))/fs; % taxis in seconds;
plot(taxis, data);

% find all of the digital ones
digital_on = data > 1;

% now find indices turned on;
change_inds = [0, diff(digital_on) == 1];

% now find times
chtimes = taxis(~~change_inds);
chtimes = chtimes-chtimes(1);

hist(diff(chtimes))
xlabel('Time [s]');
ylabel('Count');
title(sprintf('Mouse 1753, fs: %d, dt: %d',fs,1/fs));
print(gcf,sprintf('figures/mouse_1753_delta_digitalon.pdf'),'-dpdf');
