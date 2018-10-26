[fi,dir] = uigetfile('*.tif','multiselect','on');
for f=1:numel(fi)
    fi{f} = [dir fi{f}];
end
save('tonepuff_tiff_names.mat','fi');

%%
%%
clear
load('tonepuff_tiff_names.mat');
motion_correct_mike_v2(fi,'motor_102518',[0 nan],'all'); % will leave last, single frame alone
