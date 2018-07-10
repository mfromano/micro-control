#!/usr/bin/python

from serial import Serial, SerialException


if __name__ == '__main__':
    fname = raw_input('What would you like to name your output file?')
    port = raw_input('Which serial port will you be reading from?')	
    try:
        with open(str(fname),'w') as fi:
            s = Serial(str(port),2*115200)
            while True:
                mvmt = s.readline()
                fi.write(mvmt)
    except SerialException:
        print('Please enter a valid com port')
    except ValueError:
        raise ValueError
