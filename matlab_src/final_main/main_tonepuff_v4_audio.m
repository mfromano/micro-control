
fi = fopen('micro-control-data/tonepuff_v4_102618/mike_elapsedmicros_102618_audio4.txt');
x = textscan(fi,'%s','delimiter','\n');
x2 = cellfun(@(x) textscan(x,'%s','delimiter',','),x{1});
tbl = table([],[],[],[],{},{},'variablenames',{'Time','ExpTime','ExpNo','Puff','Tone','LED'});
for i=1:numel(x2)
    tbl = cat(1,tbl,table(str2double(x2{i}{1}), str2double(x2{i}{2}), str2double(x2{i}{3}), ...
        str2double(x2{i}(4)), str2double(x2{i}(5)),str2double(x2{i}(6)),'variablenames',{'Time','ExpTime','ExpNo','Puff','Tone','LED'}));
end
save('micro-control-data/tone_puff_table_102618.mat','tbl');

%% now extract data from tdt files

d = 'micro-control-data/tonepuff_v4_102618/mike_audio_puff4_102618/Block-3';
data = TDTbin2mat(d);
trial_frame_starts = find([1; diff(tbl.ExpNo) == 1]);

%% create figure for camera pulses
st.frames = data.epocs.Valu.onset(1:2:end);
st.frames = st.frames-st.frames(1);
theoretical = 0.05*(0:1:(length(st.frames)-1));
mdl = fitlm(theoretical, st.frames);
fprintf('%0.9f\n',mdl.Coefficients.Estimate(2));
figure;

theoretical = theoretical(1:200:end);
plot(theoretical, st.frames(1:200:end),'.k','MarkerSize',15);
hold on;
plot(theoretical(:), mdl.predict(theoretical(:)),'-g','LineWidth',2);
xlabel('Theoretical time [s]');
ylabel('Measured time [s]');
% legend('Data','Model fitlocation','northwest');
print(gcf,'figures/tone_puff_model_fit.pdf','-dpdf');

%% get puff statistics
st.puff_length = data.epocs.Eval.offset-data.epocs.Eval.onset;
st.puff_length = st.puff_length(1:2:end);
% now get puff starts
st.puff_start = data.epocs.Eval.onset(1:2:end) - st.frames(trial_frame_starts)-data.epocs.Valu.onset(1);

%% now get timing for Sound channel and pulse channel
sound_tdt = data.streams.Soun.data;
camera_tdt = data.streams.Puls.data;
% 
taxis_tdt = 0:1:(length(camera_tdt)-1);
taxis_tdt = taxis_tdt/data.streams.Puls.fs; % divide through by the sampling frequency
% 
taxis_sound_tdt = 0:1:(length(sound_tdt)-1);
taxis_sound_tdt = taxis_sound_tdt/data.streams.Soun.fs;
% %% get all camera frames
% 
camera_on_tdt = camera_tdt > 1;
camera_start = find(~~[0 diff(camera_on_tdt) == 1],1,'first'); % find first time point above 1
% 
%% truncate recordings by aligning to first pulse

taxis_tdt = taxis_tdt(camera_start:end);
sound_inds = (taxis_sound_tdt >= taxis_tdt(1)) & (taxis_sound_tdt <= taxis_tdt(end));
taxis_sound_tdt = taxis_sound_tdt(sound_inds);
sound_tdt = sound_tdt(sound_inds);

taxis_tdt = taxis_tdt-taxis_tdt(1); % set first time point equal to timing of first timestamp
taxis_sound_tdt = taxis_sound_tdt-taxis_sound_tdt(1);

%% now get timing of sound and associated statistics
% times recorded on external device
[b,a] = butter(6,[1000]/(data.streams.Soun.fs/2),'high');
bp = filtfilt(b,a,double(sound_tdt'));
amp = abs(hilbert(bp));

sound_inds = [amp >0.005]; % then repeat this for every trial, hopefully it works!
sound_on = find([0; diff(sound_inds) == 1]);
sound_off = find([diff(sound_inds) == -1]);

st.sound_start = taxis_sound_tdt(sound_on)' - st.frames(trial_frame_starts);
st.sound_length = taxis_sound_tdt(sound_off)-taxis_sound_tdt(sound_on);
%%



%%



subplot(2,2,4);
plot(1:50,st.puff_length,'.','MarkerSize',5);
ylim([0.099 0.101]);
ylabel('Puff length [s]');
xlabel('Trial');
xlim([0 51])