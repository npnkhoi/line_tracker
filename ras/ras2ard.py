import serial
import time
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.reset_input_buffer()
    # while True:
    #     tmp = ser.readline().decode('utf-8').rstrip()
    #     print(tmp)
    #     if tmp == '':
    #         break
    print('waiting ...')
    while True:
        inp = input()
        mapper = {'w': "0", 'a': '1.5', 'd': '-1.5', 's': '-4', ' ': '4'}
        inp = inp.strip('\n')
        if not inp in mapper:
            continue
        ser.write(mapper[inp].encode('utf-8'))
        # while True:
        #     line = ser.readline().decode('utf-8').rstrip()
        #     print(line)
        #     if line.strip() == '':
        #         break
        # time.sleep(1)
