fi = fopen('micro-control-data/mike_tonepuff_1.txt');
x = textscan(fi,'%s','delimiter','\n');
x2 = cellfun(@(x) textscan(x,'%s','delimiter',','),x{1});
tbl = table([],[],[],[],{},{},'variablenames',{'Time','ExpTime','ExpNo','Puff','Tone','LED'});
for i=1:numel(x2)
    tbl = cat(1,tbl,table(str2double(x2{i}{1}), str2double(x2{i}{2}), str2double(x2{i}{3}), x2{i}(4), x2{i}(5),x2{i}(6),'variablenames',{'Time','ExpTime','ExpNo','Puff','Tone','LED'}));
end

vars = {'Puff','Tone','LED'};
for v=1:numel(vars)
    % convert puff
    tbl.(vars{v}) = cellfun(@(x) strrep(x,'false','0'),tbl.(vars{v}),'uniformoutput',0);
    tbl.(vars{v}) = cellfun(@(x) strrep(x,'true','1'),tbl.(vars{v}),'uniformoutput',0);
    tbl.(vars{v}) = cellfun(@(x) num2str(x),tbl.(vars{v}));
end

save('micro-control-data/tone_puff_table.mat','tbl');

%% now extract data from tdt files


d = 'micro-control-data/9-17-2018-TestRomanoTone-Puff/Block-1';
data = TDTbin2mat(d);

light_tdt = data.streams.Soun.data;
puff_tdt = data.streams.Eyes.data;
camera_tdt = data.streams.Puls.data;

taxis_tdt = 0:1:length(light_tdt);
taxis_tdt = taxis_tdt/data.streams.Soun.fs; % divide through by the sampling frequency
%% get all camera frames

camera_on_tdt = camera_tdt > 1;
camera_start = find(~~[0 diff(camera_on_tdt) == 1],1,'first'); % find first time point above 1

% truncate recordings by aligning to first pulse
light_tdt = light_tdt(camera_start:end);
puff_tdt = puff_tdt(camera_start:end);
camera_tdt = camera_tdt(camera_start:end);
taxis_tdt = taxis_tdt(camera_start:end);
taxis_tdt = taxis_tdt-taxis_tdt(1)+tbl.Time(1)/(10^6); % set first time point equal to timing of first timestamp

%% now get all camera_on time points
camera_takes_tdt = camera_tdt > 1; % with truncated time series, find all time points above 1
camera_on_tdt = [~~1, diff(camera_takes_tdt) == 1]; % find first time points 
timestamps_tdt = taxis_tdt(camera_on_tdt);
timestamps_orig = tbl.Time/(10^6);
mdl.camera = fitlm(timestamps_orig, timestamps_tdt);
figure;

jitter = rand(length(timestamps_orig),1)-.5;

plot(timestamps_tdt, timestamps_orig+jitter*5,'.k','MarkerSize',5);
hold on;
plot(timestamps_tdt,mdl.camera.predict(timestamps_tdt(:)),'-g');
hold off;
xlabel('Measured time [s]');
ylabel('Time on Teensy [s]');
title('Camera times');
print('figures/tone_and_light_camera_timing.svg','-dsvg');
%% now get timing of puff and light
% times recorded on external device
on_times.light = taxis_tdt([~~0, diff(light_tdt > 1) == 1]);
off_times.light = taxis_tdt([~~0,diff(light_tdt > 1) == -1]);

on_times_teensy.light = timestamps_orig([~~0; diff(tbl.LED) == 1]);
off_times_teensy.light = timestamps_orig([~~0; diff(tbl.LED) == -1]);
mdl.lighton = fitlm(on_times.light, on_times_teensy.light);
mdl.lightoff = fitlm(off_times.light, off_times_teensy.light);
%% repeat with puff
on_times.puff = taxis_tdt([~~0, diff(puff_tdt > 1) == 1]);
off_times.puff = taxis_tdt([diff(puff_tdt > 1) == -1]);

on_times_teensy.puff = timestamps_orig([~~0; diff(tbl.Puff) == 1]);
off_times_teensy.puff = timestamps_orig([~~0; diff(tbl.Puff) == -1]);

mdl.puffon = fitlm(on_times.puff, on_times_teensy.puff);
mdl.puffoff = fitlm(off_times.puff, off_times_teensy.puff);
%% now plot puff and light

figure;
subplot(2,2,1)
plot(on_times.puff, on_times_teensy.puff,'.k','MarkerSize',10);
hold on;
plot(on_times.puff, mdl.puffon.predict(on_times.puff(:)),'-b');
hold off
xlabel('Measured time [s]');
ylabel('Time on Teensy [s]');
title('Puff On Times');

subplot(2,2,2)
plot(off_times.puff, off_times_teensy.puff,'.k','MarkerSize',10);
hold on;
plot(off_times.puff, mdl.puffoff.predict(off_times.puff(:)),'-b');
hold off
xlabel('Measured time [s]');
ylabel('Time on Teensy [s]');
title('Puff Off Times');

subplot(2,2,3)
plot(on_times.light, on_times_teensy.light,'.k','MarkerSize',10);
hold on;
plot(on_times.light, mdl.lighton.predict(on_times.light(:)),'-b');
hold off
xlabel('Measured time [s]');
ylabel('Time on Teensy [s]');
title('Light On Times');

subplot(2,2,4)
plot(off_times.light, off_times_teensy.light,'.k','MarkerSize',10);
hold on;
plot(off_times.light, mdl.lightoff.predict(off_times.light(:)),'-b');
hold off
xlabel('Measured time [s]');
ylabel('Time on Teensy [s]');
title('Light Off Times');

print(gcf,'figures/tone_and_light_timing.svg','-dsvg');