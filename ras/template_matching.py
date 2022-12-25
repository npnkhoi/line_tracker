import cv2
# import numpy
# import threading
# import time
# import matplotlib.pyplot as plt

maxsize = 180 # TODO: remember to scale down the frame
StopTemplate = cv2.imread('/home/rua/Code/line_tracker/ras/StopSign.png',0) #load grayscale
TurnLeftTemplate = cv2.imread("/home/rua/Code/line_tracker/ras/TurnLeftSign.png",0) #load grayscale
TurnRightTemplate = cv2.imread("/home/rua/Code/line_tracker/ras/TurnRightSign.png",0) #load grayscale
base_template = [StopTemplate, TurnLeftTemplate, TurnRightTemplate]
height, width = StopTemplate.shape[:2]
templates = [[], [], []]
for i, base in enumerate(base_template):
    for scale in [0.5, 0.6, 0.7, 0.8, 1.0]:
        new_size = int(maxsize * scale)
        img = cv2.resize(base, (new_size, new_size))
        templates[i].append(img)



# StopTemplate50 = StopTemplate[int(height//2):height, 0:width]
# TurnRightTemplate50 = TurnRightTemplate[int(height//2):height, 0:width]
# TurnLeftTemplate50 = TurnLeftTemplate[int(height//2):height, 0:width]

# StopTemplate30 = StopTemplate[int(height//3*2):height, 0:width]
# TurnRightTemplate30 = TurnRightTemplate[int(height//3*2):height, 0:width]
# TurnLeftTemplate30 = TurnLeftTemplate[int(height//3*2):height, 0:width]

# a 2-d matrix of images
# SignTemplates = [[StopTemplate,TurnLeftTemplate,TurnRightTemplate],
#                 [StopTemplate50,TurnLeftTemplate50,TurnRightTemplate50], 
#                 [StopTemplate30,TurnLeftTemplate30,TurnRightTemplate30]]

# allSigns = [[], [], []]

# for scint in range(70, 50-1, -10):
#     scale = scint/100.0
#     # resizing the images
#     for type_index in range (0,3):
#         for sign_index in range (0, 3):
#             allSigns[type_index].append(
#                 cv2.resize(
#                     SignTemplates[type_index][sign_index],
#                     (int(maxsize*scale), int(maxsize*scale))
#                 ))

# TemplateCategory = {0:"Stop", 1:"TurnLeft", 2:"TurnRight", 
#                     3:"Stop 50", 4:"TurnLeft 50", 5:"TurnRight 50",
#                     6:"Stop 30", 7:"TurnLeft 30", 8:"TurnRight 30"}


#status matching: 0 - no direction --> check 100%, 
#                 1 - direction + get None --> check 50%, 3 - check 50% 


TemplateThreshold = 0.6

def GetSignSingle(imgframe):
    # result = {} #result = {0: None, 1: None, 2: None}
    grayframe = cv2.cvtColor(imgframe, cv2.COLOR_BGR2GRAY)
    # curMaxLoc = (0,0)
    best, sign = 0, None
    for i, lst in enumerate(templates):
        for temp in lst:
            res = cv2.matchTemplate(grayframe,temp,cv2.TM_CCOEFF_NORMED)
            min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
            if max_val > max(best, TemplateThreshold):
                best = max_val
                sign = (i, max_loc)
    return sign, grayframe


    # for index, sign_type in enumerate(allSigns):
    #     curMaxTemplate = -1
    #     curMaxVal = 0
    #     for c, template in enumerate(sign_type):
    #         res = cv2.matchTemplate(grayframe,template,cv2.TM_CCOEFF_NORMED)
    #         min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
    #         if max_val > max(TemplateThreshold, curMaxVal):
    #             curMaxVal = max_val
    #             curMaxTemplate = c
    #             # curMaxLoc = max_loc
    #             print(f'index={index}, c={c}')
    #             break
        
    #     if curMaxTemplate == -1:
    #         result[index] = None
    #     else:
    #         result[index] = curMaxTemplate%3
    #         # return (curMaxTemplate%3, curMaxLoc, 1 - int(curMaxTemplate/3)*0.2, curMaxVal, (1 - int(curMaxTemplate/3)*0.2)*maxsize)
    # return result, grayframe

# # Capture from first Camera Source
# cap = cv2.VideoCapture('Stop.mp4')
# cap.set(cv2.CAP_PROP_FRAME_WIDTH,320);
# cap.set(cv2.CAP_PROP_FRAME_HEIGHT,240);
# count = 0

# while(True):
#     # Capture frame-by-frame
#     ret, frame = cap.read()
#     count += 1

#     if not ret:
#         break
    
#     if count % 10 != 0:
#         continue
        
#     template = -1

#     # Apply template Matching
#     (template, top_left, scale, val, size) = GetSignSingle(frame)

#     if template != -1:
#         bottom_right = (top_left[0] + int(size), top_left[1] + int(size))
#         cv2.rectangle(frame,top_left, bottom_right, 255, 2)
#         cv2.putText(frame,str(val),(20,300),cv2.FONT_HERSHEY_SIMPLEX,2,(0,255,0),2)
#         cv2.putText(frame,str(TemplateCategory[template]),(20,200),cv2.FONT_HERSHEY_SIMPLEX,2,(0,0,255),2)
#         print(int(size))
#         print(TemplateCategory[template])
#     cv2.imshow("vid",frame)
#     k = cv2.waitKey(10)
#     if k & 0xFF == ord('q'):
#         break
