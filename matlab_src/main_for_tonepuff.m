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
