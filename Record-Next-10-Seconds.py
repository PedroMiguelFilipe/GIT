import numpy as np
import cv2
import time
import signal
import sys

cap = cv2.VideoCapture(0)

t1 = time.perf_counter()

fourcc = cv2.VideoWriter_fourcc('a','v','c','1') # Codec definition

fps = cap.get(5) # Definition of FPS

numOfSeconds = 10

numberOfSavedFrames = fps * numOfSeconds # SavedFrames = FPS * Nº of Seconds

out = cv2.VideoWriter('Desktop/videos/Carregamento.mp4', fourcc, fps, (640, 480))

i = 0

while(i < numberOfSavedFrames):
    ret, frame = cap.read() 
    out.write(frame)
    i += 1

out.release()
cap.release()
cv2.destroyAllWindows()
t2 = time.perf_counter() - t1
print(t2)



