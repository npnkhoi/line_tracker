import cv2
from copy import deepcopy
import numpy as np
import sys
import datetime
import utils
from typing import Tuple
from arduino import Arduino
import template_matching

def detect_line(img, size) -> Tuple:
    """
    get the processed image, line segment, control signal

    Return: (error: float, img)
    """
    line, ret = utils.get_line_seg(img)
    ret = cv2.resize(ret, size) # because it shrank a bit
    ret = cv2.cvtColor(ret, cv2.COLOR_GRAY2RGB)

    if line is None:
        ret = cv2.putText(ret, 'Not found!', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        return 4, ret
    
    x1, y1, x2, y2 = line

    # draw the line segment and the center onto ret
    ret = cv2.line(ret, (x1, y1), (x2, y2), (0, 255, 0), thickness=4)
    center = ((x1+x2)//2, (y1+y2)//2)
    ret = cv2.circle(ret, center, 10, (0, 0, 255), -1)
    h, w = ret.shape[:2]
    error = center[0] / w - 0.5
    ret = cv2.putText(ret, f'error={round(error, 2)}', (50, 50), 
        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    
    def error2control(e: float, mid_band=0.05) -> float:
        if abs(error) <= mid_band:
            return 0
        # positive control means turn left, and vice versa
        return 1.2 if error < 0 else -1.2
    control = error2control(error)

    return control, ret

def detect_sign(img):
    dir, ret = template_matching.GetSignSingle(img)
    return dir, ret

    # draw rectangle to ret


    # print('getSignSingle done.')
    # ret = cv2.resize(ret, size)
    # ret = cv2.cvtColor(ret, cv2.COLOR_GRAY2RGB)

    # #if detects full sign --> update direction dict
    # if sign_annotating[0] is not None:
    #     directionDict[sign_annotating[0]] += 1
    #     ret = cv2.putText(ret, f'direction={directionDict[sign_annotating[0]]}', (50, 50), 
    #                     cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    #     return None, ret, directionDict
    # elif sign_annotating[1] is None and sign_annotating[2] is not None:
    #     direction = np.argmax(directionDict.values())
    #     if directionDict[direction] > 3:
    #         if direction == 0:
    #             ret = cv2.putText(ret, f'STOP', (50, 50), 
    #                     cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    #             return 4, ret, directionDict
    #         elif direction == 1:
    #             ret = cv2.putText(ret, f'TURN LEFT', (50, 50), 
    #                     cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    #             return 1.2, ret, directionDict
    #         elif direction == 2:
    #             ret = cv2.putText(ret, f'TURN RIGHT', (50, 50), 
    #                     cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    #             return -1.2, ret, directionDict

    # ret = cv2.putText(ret, f'NO SIGN', (50, 50), 
    #             cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    # return None, ret, directionDict

if __name__ == "__main__":
    control = 0
    directionDict = {0: 0, 1: 0, 2: 0}
    duration = int(sys.argv[1])

    # Create an object to read 
    # from camera
    video = cv2.VideoCapture(0)
    
    # We need to check if camera
    # is opened previously or not
    if (video.isOpened() == False): 
        print("Error reading video file")
    
    # We need to set resolutions.
    # so, convert them from float to integer.
    frame_width = 480*16//9 # int(video.get(3))
    frame_height = 480 # int(video.get(4))
    # print('screen size:', frame_width, frame_height)

    # sys.exit(0)
    
    size = (frame_width, frame_height)
    
    # Below VideoWriter object will create
    # a frame of above defined The output 
    # is stored in 'filename.avi' file.
    result = cv2.VideoWriter('raw.avi', 
                            cv2.VideoWriter_fourcc(*'MJPG'),
                            10, size)
    result2 = cv2.VideoWriter('annotated.avi', 
                            cv2.VideoWriter_fourcc(*'MJPG'),
                            10, size)
    result3 = cv2.VideoWriter('sign_detecting.avi', 
                            cv2.VideoWriter_fourcc(*'MJPG'),
                        10, size)

    start_time = datetime.datetime.now()
    print(f'Started recording for {duration}s ...')

    ard = Arduino()

    while(True):
        ret, frame = video.read()
        frame = cv2.resize(frame, size)

        if ret == True:
            frame0 = deepcopy(frame)

            # annotate image
            print('dectecting line')
            line_control, frame2 = detect_line(frame, size)
            print('dectecting line done. switch to sign')
            tic = datetime.datetime.now()
            # direction_control, frame3, directionDict = detect_sign(frame, size, directionDict)
            direction_control, frame3 = detect_sign(frame)
            # direction_control, frame3, directionDict = None, frame0, directionDict
            toc = datetime.datetime.now()
            print(f'dectecting sign done, duration = {(toc-tic)}')
            result.write(frame0)
            result2.write(frame2)
            result3.write(frame3)

            # control!
            if direction_control is not None:
                control = direction_control
            elif line_control != 4:
                control = line_control
            print('control:', control)

            ard.write(str(control))
    
            # Display the frame
            # master_frame = np.concatenate([frame0, frame2], axis=1)
            # cv2.imshow('master frame',  master_frame)

            # Break condition
            if (datetime.datetime.now() - start_time).seconds >= duration:
                ard.write('4')
                break
            if cv2.waitKey(1) & 0xFF == ord('q'): # Do we need this in headless mode?
                ard.write('4')
                break
    
        # Break the loop
        else:
            break
    
    # When everything done, release the video capture and video write objects
    video.release()
    result.release()
    result2.release()
    result3.release()
        
    # Closes all the frames
    cv2.destroyAllWindows()
    
    print("The video was successfully saved")