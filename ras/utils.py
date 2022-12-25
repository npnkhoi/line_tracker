import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt 

crop_range = 200

def calculateLen(line):
    x1, y1, x2, y2 = line
    return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)

def get_line_seg(imageOriginal):
    # input: image
    # output: line (tuple of 4), lane_image
    drawnImage = np.copy(imageOriginal)
    laneImage = np.copy(imageOriginal)

    # CANNY ALGORITHM
    lowThreshold = 50
    highThreshold = 150
    # convert to grey scale
    greyImage = cv.cvtColor(laneImage, cv.COLOR_RGB2GRAY)
    greyImage = cv.cvtColor(laneImage, cv.COLOR_RGB2GRAY)
    thresholding = cv.threshold(greyImage, 200, 255, cv.THRESH_BINARY)
    greyImage = thresholding[1]
    # detect 'possible' lane areas us
    # detect 'possible' lane areas using canny method
    laneImage = cv.Canny(greyImage, lowThreshold, highThreshold)

    height = laneImage.shape[0]
    width = laneImage.shape[1]
    mask = np.zeros(laneImage.shape[:2], dtype="uint8")
    cv.rectangle(mask, (15, height - 15), (width - 15, crop_range), 255, -1)
    laneImage = cv.bitwise_and(laneImage, laneImage, mask = mask)

    # HOUGH TRANSFORMATION
    rho = 1
    theta = np.pi/180
    threshold = 80
    minLineLength = 100
    maxLineGap = 10
    lines = cv.HoughLinesP(laneImage, rho, theta, threshold, None, minLineLength, maxLineGap)
    if lines is None:
        return None, laneImage
    for line in lines:
        x1, y1, x2, y2 = line[0]
    
    firstline = [0, 0, 0, 0]
    for eachLine in lines:
        x1, y1, x2, y2 = eachLine[0]
        if y1 == y2 or (abs(x1 - x2))/(abs(y1 - y2)) < 0.35:
            continue
        #  if abs(y1 - y2) < abs(x1 - x2):
        #     continue
        if calculateLen(firstline) < calculateLen(eachLine[0]):
            firstline = [x1, y1, x2, y2]
    return firstline, laneImage

def get_error(img, band=0.15):
    line = get_line_seg(img)
    if line is None:
        return 4
    else:
        x1, y1, x2, y2 = line
    print()
    center = ((x1 + x2)/2, (y1+y2)/2)
#     print(img.shape)
    h, w = img.shape[:2]

    x = center[0] / w - 0.5
    print(f'center={center}, w={w}, x={x}')
    plt.plot(center[0], center[1], 'go')
    plt.axvline(w*(0.5-band))
    plt.axvline(w*(0.5+band))
    sign = 1 if x >= 0 else -1
    return sign * max(abs(x) - band, 0) * 1 / (0.5 - band)

# --------------------------------------------------------------------------
import cv2 

if __name__ == "__main__":
    video_filename = 'vid1.mov'
    vid = cv2.VideoCapture(video_filename)

    if (vid.isOpened()== False):
        print('error while openning vid')
        raise Exception

    cnt = 0
    while vid.isOpened():
        ret, img = vid.read()
        if ret:
            cnt += 1
        if cnt % 5 != 0: # WARNING: may need to change this
            continue
        # print(cnt)
        error = get_error(img)
        img = cv2.putText(img, f'error={"%.2f" % error}', (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)
        cv2.imshow('cam', img)
        if cv2.waitKey(25) & 0xFF == ord('q'):
            break
        else:
            break

    vid.release()
    cv2.destroyAllWindows()
