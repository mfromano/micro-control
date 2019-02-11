%{
prompt = {'USB to UART Converter Port:', 'Arduino Port:'};
definput = {'i.e. COM1','i.e. COM5');
port = inputdlg(prompt,'Data Ports', [1 15; 1 15],definput);
s = serial('COM5')
%}

global huiw1;
global huiw3;
global huiw4;
global huiw5;
global movement;
global fi;

fi = 0;
movement = cell(0);
global f;

f = figure('Visible','off','Units','Normalized',...
    'Position', [0.1 0.1 0.4 0.4], 'Color', [0 0.7 0.7],...
    'name', 'Mouseball Setup');
huipusha = uicontrol('Style','pushbutton', 'Units', 'Normalized', ...
    'Position', [0.2 0.5 0.3 0.1],'string','Start', 'Callback', @callbackfn1,...
    'FontName', 'Wawati SC', 'FontSize', 14);

text_input = @(position, text) uicontrol('Style','edit', 'Units', 'Normalized', ...
    'Position', position,'string',text,...
    'FontName', 'Wawati SC', 'FontSize', 9);

huiw1 = text_input([0.17 0.43 0.33 0.05],'Enter the Teensy Serial Port (i.e COM1)');
huiw3 = text_input([0.17 0.31 0.33 0.05],'Experiment output file name');
huiw4 = text_input([0.17 0.25 0.33 0.05],'Number of trials?');
huiw5 = text_input([0.17 0.19 0.33 0.05],'Length of trials? [us]');
huiw13 = text_input([0.17 0.13 0.33 0.05],'Puff Start [us]');
huiw14 = text_input([0.17 0.07 0.33 0.05],'Puff Length [us]');

huiw9 = text_input([0.5 0.43 0.33 0.05],'Tone 1 FQ?');
huiw7 = text_input([0.5 0.31 0.33 0.05],'Tone 1 Start [us]?');
huiw8 = text_input([0.5 0.25 0.33 0.05],'Tone 1 Length [us]?');

huiw12 = text_input([0.5 0.19 0.33 0.05],'Tone 2 FQ?');
huiw10 = text_input([0.5 0.13 0.33 0.05],'Tone 2 Start [us]?');
huiw11 = text_input([0.5 0.07 0.33 0.05],'Tone 2 Length [us]?');

huiw6 = uicontrol('Style','pushbutton', 'Units', 'Normalized', ...
    'Position', [0.5 0.5 0.3 0.1],'string','Stop','CallBack',@callbackfn2,...
    'FontName','Wawati SC', 'FontSize',9);
set(huiw6,'enable','off');
set(f, 'Visible','on');


function callbackfn1(~,~)
    global huiw1;
    global huiw3;
    global huiw4;
    global huiw5;
    global huiw6;
    global huiw7;
    global huiw8;
    global huiw9;
    global huiw10;
    global huiw11;
    global huiw12;
    global huiw13;
    global huiw14;
    
    
    
    global uart;
    global fi;
    global movement;
    
    fi = fopen(huiw3.String,'w');
    uart = serial(huiw1.String, 'BaudRate', 115200);
    fopen(uart);

    pause(2);

    fwrite(uart,sprintf('%s,%s,%s,%s,%s,%s,%s,%s,%s,%s',...
        huiw4.String, huiw5.String, ... % add in trials/trial length
        huiw13.String, huiw14.String,... % add in puff/puff length;
        huiw7.String, huiw8.String, huiw9.String,... % tone for fq1
        huiw10.String, huiw11.String, huiw12.String... % tone for fq2
    ));
    
    pause(0.1);
    x = fscanf(uart,'%s\n');
    fprintf(x);
    movement = cell(0);
    fprintf('Beginning acquisition\n');
    set(huiw6,'enable','on');
    while true
        movement{end+1} = fscanf(uart,'%s');
        if strcmp(movement{end},'END')
            break;
        end
        fprintf('%s\n',movement{end});
    end
    for i=1:numel(movement)
        fprintf(fi,'%s\n',movement{i});
    end
    fclose(fi);
    set(huiw6,'enable','off');
    fclose(uart);
    delete(uart);
    movement = cell(0);
    clear uart
end

function callbackfn2(~,~)
global uart;

fwrite(uart,"STOP");

end

