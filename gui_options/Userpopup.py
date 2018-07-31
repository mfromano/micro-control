import Tkinter as tk
import tkMessageBox
import serial

class ExperimentalControl:
    def __init__(self,window):
        self.window = window
        self.framerate = tk.StringVar()
        self.framerate.set('Please enter the sampling rate in ms')
        self.experiment_length = tk.StringVar()
        self.experiment_length.set('Please enter the length of the experiment in mins')
        self.serial_port_arduino = tk.StringVar()
        self.serial_port_arduino.set('Please enter the serial port for the Arduino')
        self.serial_port_motion = tk.StringVar()
        self.serial_port_motion.set('Please enter the serial port to collect motion data')
        self.output_filename = tk.StringVar()
        self.output_filename.set('Please enter the output file name')
        self.add_start_button()
        frame = tk.Frame(window)

    def add_entry(self,tv):
        entry = tk.Entry(self.window, textvariable=tv,width=50)
        entry.pack(padx=50,pady=5)

    def add_start_button(self):
        b = tk.Button(self.window,text='Start Experiment', command=self.begin)
        b.pack()
    def begin(self):
        while True:
            sr1 = serial.Serial(self.serial_port_arduino.get(),baudrate=115200)
            try:
                sr1.open()
            except SerialException:
                tkMessageBox('Error','Please enter a valid serial port for Arduino!')
                continue

            sr2 = serial.Serial(self.serial_port_motion.get(),baudrate=115200)
            try:
                sr2.open()
            except SerialException:
                tkMessageBox('Error','Please enter a valid motion serial port!')
                continue

            sr1.write('{0},{1}'.format(self.experiment_length.get(), self.framerate.get()))
            nloops = double(sr1.readline())
            ncycles = double(sr1.readline())
            self.loop_while_true()


    def loop_while_true():
        with open(self.output_filename.get(),'wb') as fi:
            for i in range(nloops):
                motion_info = sr1.readline()
                time_info = sr2.readline()
                fi.write('{0}t={1}'.format(motion_info,time_info))
                print('{0}t={1}'.format(motion_info,time_info))

if __name__ == '__main__':
    t = tk.Tk()
    experiment = ExperimentalControl(t)
    experiment.add_entry(experiment.framerate)
    experiment.add_entry(experiment.experiment_length)
    experiment.add_entry(experiment.serial_port_arduino)
    experiment.add_entry(experiment.serial_port_motion)
    experiment.add_entry(experiment.output_filename)
    t.update()
    t.wait_window()
