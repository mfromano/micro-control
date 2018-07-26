%{
prompt = {'USB to UART Converter Port:', 'Arduino Port:'};
definput = {'i.e. COM1','i.e. COM5');
port = inputdlg(prompt,'Data Ports', [1 15; 1 15],definput);
s = serial('COM5')
%}

global uart;
global a;
global huiw1;
global huiw2;

f = figure('Visible','off','Units','Normalized',...
    'Position', [0.1 0.1 0.4 0.4], 'Color', [0 0.7 0.7],...
    'name', 'Mouseball Setup');
huipusha = uicontrol('Style','pushbutton', 'Units', 'Normalized', ...
    'Position', [0.3 0.5 0.2 0.1],'string','Start', 'Callback', @callbackfn1,...
    'FontName', 'Wawati SC', 'FontSize', 14);
huipushb = uicontrol('Style','pushbutton', 'Units', 'Normalized', ...
    'Position', [0.5 0.5 0.2 0.1],'string','Stop', 'Callback', @callbackfn2,...
    'FontName', 'Wawati SC', 'FontSize', 14);
huiw1 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.43 0.33 0.05],'string','Enter the USB to UART Serial Port (i.e COM1)',...
    'FontName', 'Wawati SC', 'FontSize', 9);
huiw2 = uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', [0.35 0.37 0.33 0.05],'string','Enter the Arduino Serial Port (i.e COM5)',...
    'FontName', 'Wawati SC', 'FontSize', 9);
set(f, 'Visible','on');


function callbackfn1(~,~)
    global huiw1;
    global huiw2;
    global uart;
    global a;
    
    try
        uart = serial(huiw1.String, 'BaudRate',115200);
        fopen(uart);
    catch
        errordlg('There was an error connecting to the USB to UART bridge. Please check the COM port.');
    end
    try
        a = arduino(huiw2.String, 'uno'); %The board may need to change if a different board is used
    catch
        errordlg('There was an error connecting to the Arduino. Please check the COM port.');
    end
    writeDigitalPin(a,'D3',1);
    fscanf(uart)
end
function callbackfn2(~,~)
    global uart;
    global a;
    
    writeDigitalPin(a,'D3',0);
    fclose(uart);
    delete(uart);
    clear uart
end