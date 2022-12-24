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


cap = cv2.VideoCapture(0)

if not cap.isOpened():
    raise IOError("Cannot open webcam")

while True:
    ret, frame = cap.read()
    # frame = cv2.resize(frame, None, fx=0.5, fy=0.5, interpolation=cv2.INTER_AREA)
    # cv2.imshow("rua's mind", frame)
    
    c = cv2.waitKey(1)
    if c == 27:
        break
    
cap.release()
cv2.destropAllWindows()
