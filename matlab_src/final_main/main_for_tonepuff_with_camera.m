% first get tiff file names

[fi,dir] = uigetfile('*.tif','multiselect','on');
for f=1:numel(fi)
    fi{f} = [dir fi{f}];
end
save('tonepuff_file_names.mat','fi');

%%
clear
load('tonepuff_file_names.mat');
motion_correct_mike_v1(fi,'tonepuff_102618_1752_',[0 nan],'all'); % will leave last, single frame alone

%%
metadata.suffix = 'tonepuff_102618_1752';
[fi,dir] = uigetfile('*.tif','multiselect','on');


for f=1:numel(fi)
    fi{f} = [dir fi{f}];
end
fi2 = order_filenames_processed(fi);
metadata.tiffs = fi2;



save('tonepuff_102618_1752.mat','metadata');
%%
clear
load('tonepuff_102618_1752.mat')

%%
roi_simon_new(metadata);


%%
load('/fastdata/micro-control/micro-control-data/processed-data/roi_simon_tonepuff_102618_1752.mat')
load('/fastdata/micro-control/micro-control-data/processed-data/imgDiff_simon_tonepuff_102618_1752.mat')
roi_overlay(roi_simon, imgDiff_simon)
print('figures/roi_overlay_tonepuff.svg','-dsvg');
%%
roi_overlay('',imgDiff_simon);
print('figures/max_minus_mean_tonepuff.svg','-dsvg');



%% maybe redo that imaging session?



