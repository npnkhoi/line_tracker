"""

Outline:
    - init (videocapture, ...)
    - loop:
        - get frame (img)
        - get error from img
        - turn error into control signal
        - send control signal to Arduino
        - stream the image and error to the web
"""

import cv2
from utils import get_error
import serial
import time




if __name__ == '__main__':
    # set up camera
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        raise IOError("Cannot open webcam")

    # set up serial for communication with Arduino
    ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    ser.reset_input_buffer()

    # autopilot!
    cnt = 0
    while True:
        ret, frame = cap.read()
        cnt += 1

        if cnt % 10 != 0:
            continue

        error = get_error(frame)
        c = str(round(error, 4))
        print('control signal:', c)
        ser.write(c.encode('utf-8'))

        cv2.imwrite(f'bug/bug_{cnt}_{str(c)}.jpg', frame)
    
    cap.release()
    cv2.destropAllWindows()
