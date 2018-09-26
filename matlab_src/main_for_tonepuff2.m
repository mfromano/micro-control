load('micro-control-data/tone_puff_table.mat','tbl');
timestamps_orig = tbl.Time;
trial_timestamps = [1; find([0; diff(tbl.ExpNo) == 1]); length(tbl.ExpNo)+1];

d = 'micro-control-data/9-17-2018-TestRomanoTone-Puff/Block-1';
data = TDTbin2mat(d);

% find times in trial for LED on, LED off, Puff on, Puff off
st.led_length = [];
st.puff_length = [];
st.intermission = [];
st.led_length = data.epocs.Eval.offset-data.epocs.Eval.onset;
st.led_length = st.led_length(1:2:end);
st.puff_length = data.epocs.Sund.offset-data.epocs.Sund.onset;
st.puff_length = st.puff_length(1:2:end);
st.intermission = data.epocs.Sund.onset-data.epocs.Eval.offset;
st.intermission = st.intermission(1:2:end);

%% get all camera frames
camera_times = data.epocs.Valu.onset(1:2:end);

%% now get theoretical timing of puff and light

%% repeat with puff

%% now plot puff and light

print(gcf,'figures/tone_and_light_timing.svg','-dsvg');