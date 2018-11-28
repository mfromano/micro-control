%% assessing analog pin timing
d = 'micro-control-data/mike_tonetiming_112718_puffeyes_tonesoun/Block-1';
data = TDTbin2mat(d);

analog_onsets = data.epocs.Sund.onset(1:2:end);
digital_onsets = data.epocs.Eval.onset(1:2:end);
diffs = analog_onsets-digital_onsets;