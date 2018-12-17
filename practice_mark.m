

fi = fopen('practice_mark2.txt','w');
uart = serial('COM10', 'BaudRate', 115200);
fopen(uart);

pause(2);

fwrite(uart,sprintf('%s','go'));
pause(0.1);

movement = cell(5000,1);
pause(0.5);
for i=1:(5000+1)
    movement{i} = fscanf(uart,'%s');
    fprintf('%s\n',movement{i});
end
for i=1:(5000+1)
    fprintf(fi,'%s\n',movement{i});
end
fclose(fi);
fclose(uart);
delete(uart);