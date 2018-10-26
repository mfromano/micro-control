% first get tiff file names

[fi,dir] = uigetfile('*.tif','multiselect','on');
for f=1:numel(fi)
    fi{f} = [dir fi{f}];
end
save('motor_file_names.mat','fi');

%%
clear
load('motor_file_names.mat');
motion_correct_mike_v1(fi,'motor_102518',[0 nan],'all'); % will leave last, single frame alone

%%
metadata.suffix = 'motor_102518';
[fi,dir] = uigetfile('*.tif','multiselect','on');


for f=1:numel(fi)
    fi{f} = [dir fi{f}];
end
fi2 = order_filenames_processed(fi);
metadata.tiffs = fi2;



save('metadata_motor_102518.mat','metadata');
%%
clear
load('metadata_motor_102518.mat')