import cv2
from copy import deepcopy
import numpy as np
import sys
import datetime
import utils

def annotate_img(img):
    """
    get the processed image, line segment, control signal
    """
    line, ret = utils.get_line_seg(img)
    if line is None:
        ret = cv2.putText(ret, 'Not found!', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        print('not found!')
        return ret
    
    x1, y1, x2, y2 = line

    # draw the line segment and the center onto ret
    ret = cv2.line(ret, (x1, y1), (x2, y2), (0, 255, 0), thickness=2)
    center = ((x1+x2)//2, (y1+y2)//2)
    ret = cv2.circle(ret, center, 5, (255, 0, 0))
    print(center)
    return ret

if __name__ == "__main__":
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

    start_time = datetime.datetime.now()
    duration = 5 # second
    print(f'Started recording for {duration}s ...')
        

    while(True):
        ret, frame = video.read()
        frame = cv2.resize(frame, size)

        if ret == True:
            frame0 = deepcopy(frame)

            # annotate image
            frame2 = annotate_img(frame0)
            frame2 = cv2.resize(frame2, size) # because it shrank a bit
            frame2 = cv2.cvtColor(frame2, cv2.COLOR_GRAY2RGB)
    
            result.write(frame0)
            result2.write(frame2)
    
            # Display the frame
            # master_frame = np.concatenate([frame0, frame2], axis=1)
            # cv2.imshow('Frame',  master_frame)
    
            if (datetime.datetime.now() - start_time).seconds >= duration:
                break
    
        # Break the loop
        else:
            break
    
    # When everything done, release the video capture and video write objects
    video.release()
    result.release()
    result2.release()
        
    # Closes all the frames
    cv2.destroyAllWindows()
    
    print("The video was successfully saved")