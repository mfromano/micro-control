fi = fopen('practice_mark_orig.txt','w');
uart = serial('COM10', 'BaudRate', 115200);
fopen(uart);

pause(2);
pause(0.1);

movement = cell(5000,1);
pause(0.5);
for i=1:(10000+1)
    movement{i} = fscanf(uart,'%s');
    fprintf('%s\n',movement{i});
end
for i=1:(5000+1)
    fprintf(fi,'%s\n',movement{i});
end
fclose(fi);
fclose(uart);
delete(uart);