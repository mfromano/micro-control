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

%%
roi_simon_new(metadata);

%%
load('/fastdata/micro-control/micro-control-data/processed-data/imgDiff_simon_motor_102518.mat')
load('/fastdata/micro-control/micro-control-data/processed-data/roi_simon_motor_102518.mat')
roi_overlay(roi_simon,imgDiff_simon);
print('figures/roi_overlay_motor.svg','-dsvg');
%%
roi_overlay('',imgDiff_simon);
print('figures/max_minus_mean_motor.svg','-dsvg');


%%

%%
x = csvread('micro-control-data/motor_control_v3_1247_102518.txt',1,0);
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
velocity_cms = sqrt((yr.^2+yl.^2-2*yl.*yr.*cos(sensorAngleRadians))/sin(sensorAngleRadians).^2)*100;

yl = (yl-yr*cos(sensorAngleRadians))/sin(sensorAngleRadians);

velocity2 = sqrt(yl.^2+yr.^2)*100;

taxis = x(:,1)/1e6;
%%
plot(taxis(1:2000),velocity2(1:2000));
print('figures/example_movement_mouse_imaging.svg','-dsvg');
%%
load('micro-control-data/processed-data/trace_simon_motor_102518.mat');
for r=1:numel(trace_simon)
   figure; plot(taxis,(trace_simon(r).trace-mean(trace_simon(r).trace))/mean(trace_simon(r).trace));
   title(sprintf('Trace # %d',r));
   pause
   close
end

