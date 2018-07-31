%{
prompt = {'USB to UART Converter Port:', 'Arduino Port:'};
definput = {'i.e. COM1','i.e. COM5');
port = inputdlg(prompt,'Data Ports', [1 15; 1 15],definput);
s = serial('COM5')
%}

global huiw1;
global huiw2;
global huiw3;
global huiw4;
global huiw5;


f = figure('Visible','off','Units','Normalized',...
    'Position', [0.1 0.1 0.4 0.4], 'Color', [0 0.7 0.7],...
    'name', 'Mouseball Setup');
huipusha = uicontrol('Style','pushbutton', 'Units', 'Normalized', ...
    'Position', [0.3 0.5 0.4 0.1],'string','Start', 'Callback', @callbackfn1,...
    'FontName', 'Wawati SC', 'FontSize', 14);

huiw1 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.43 0.33 0.05],'string','Enter the USB to UART Serial Port (i.e COM1)',...
    'FontName', 'Wawati SC', 'FontSize', 9);
huiw2 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.37 0.33 0.05],'string','Enter the Arduino Serial Port (i.e COM5)',...
    'FontName', 'Wawati SC', 'FontSize', 9);
huiw3 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.31 0.33 0.05],'string','Motor output file name',...
    'FontName', 'Wawati SC', 'FontSize', 9);
huiw4 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.25 0.33 0.05],'string','Length of session? [min]',...
    'FontName', 'Wawati SC', 'FontSize', 9);
huiw5 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.19 0.33 0.05],'string','Sampling interval? [ms]',...
    'FontName','Wawati SC', 'FontSize',9);
set(f, 'Visible','on');


function callbackfn1(~,~)
    global huiw1;
    global huiw2;
    global huiw3;
    global huiw4;
    global huiw5;
    global uart;
    global a;
    global fi;

    fi = fopen(huiw3.String,'w');
    try
        uart = serial(huiw1.String, 'BaudRate', 115200);
        fopen(uart);
    catch
        errordlg('There was an error connecting to the USB to UART bridge. Please check the COM port.');
    end
    try
        a = serial(huiw2.String, 'BaudRate', 115200); %The board may need to change if a different board is used
        fopen(a);
    catch
        errordlg('There was an error connecting to the Arduino. Please check the COM port.');
    end
    pause(2);
    fwrite(a,sprintf('%s,%s',huiw4.String, huiw5.String));
    pause(0.1);
    nreps = fscanf(a,'%s\n');
    repcycles = fscanf(a,'%s\n');
    fprintf('nreps: %s, repcycles: %s\n',nreps,repcycles);
    fprintf('Beginning acquisition\n');
    for i=1:str2double(nreps)
        rd = fscanf(uart);
        fprintf(fi,'%s',rd);
        fprintf('%s\n',rd);
        rd = fscanf(a,'%s\n');
        fprintf(fi,'t=%s\n',rd);
    end
    fclose(a);
    delete(a);
    fclose(fi);
    fclose(uart);
    delete(uart);
    clear uart a
end
