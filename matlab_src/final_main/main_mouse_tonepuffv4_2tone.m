fi = fopen('micro-control-data/tonepuff5_2tone_test/practice_mike_tonepuff_2000Hz1_0.05amp1_8000Hz2_0.4amp2.txt');
x = textscan(fi,'%s','delimiter','\n');
x2 = cellfun(@(x) textscan(x,'%s','delimiter',','),x{1});
tbl = table([],[],[],[],[],[],[],'variablenames',{'Time','ExpTime','ExpNo','Puff','Tone1','Tone2','LED'});
for i=1:numel(x2)
    tbl = cat(1,tbl,table(str2double(x2{i}{1}), str2double(x2{i}{2}), str2double(x2{i}{3}), ...
        str2double(x2{i}(4)), str2double(x2{i}(5)),str2double(x2{i}(6)),str2double(x2{i}(7)),'variablenames',{'Time','ExpTime','ExpNo','Puff','Tone1','Tone2','LED'}));
end
save('micro-control-data/tone_puff_table_2tone_2000Hz_0.05amp1_8000Hz_0.4amp2.mat','tbl');

%% now extract data from tdt files
load('micro-control-data/tone_puff_table_2tone_2000Hz_0.05amp1_8000Hz_0.4amp2.mat','tbl');

d = 'micro-control-data/tonepuff5_2tone_test/practice_2tone/Block-3'
data = TDTbin2mat(d);
trial_frame_starts = find([1; diff(tbl.ExpNo) == 1]);
tone_starts1 = find([0; diff(tbl.Tone1) == 1]);
tone_starts2 = find([0; diff(tbl.Tone2) == 1]);
puff_starts = find([0; diff(tbl.Puff) == 1]);
