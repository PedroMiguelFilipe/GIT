# Imports
import cv2
import signal
import sys

cap = cv2.VideoCapture(0) # Setup video capture

fourcc = cv2.VideoWriter_fourcc('a','v','c','1') # Codec definition

detector = cv2.QRCodeDetector() # QR Code detection

#fps = cap.get(5)
fps = 20

frameList = []

numberOfSavedFrames = fps * 5 # SavedFrames = FPS * Nº of Seconds

def signal_handler(signum,frame):
    cap.release()
    cv2.destroyAllWindows()
    out = cv2.VideoWriter('Desktop/videos/Descarregamento.mp4', fourcc, fps, (640, 480))
    for frame in frameList:
        out.write(frame)
    out.release()
    #print('Done')
    sys.exit(0)
    
signal.signal(signal.SIGTERM, signal_handler)

while True: # Infinite loop so that the QR Code detection is constantly scanning for data
    
    ret, img = cap.read() # Obtain frame for QR Code detection
    
    frameList.append(img)
    
    if(len(frameList) > numberOfSavedFrames):
        frameList.pop(0)
     
    data, bbox, ret = detector.detectAndDecode(img) # Method that reads QR Codes by detecting bounding box coords and decoding the hidden QR data 
    
    if(bbox is not None): # Draw blue box around QR Code and write data on top
        for i in range(len(bbox)):
            cv2.line(img, tuple(bbox[i][0]), tuple(bbox[(i+1) % len(bbox)][0]), color=(255,
                     0, 0), thickness=2)
        cv2.putText(img, data, (int(bbox[0][0][0]), int(bbox[0][0][1]) - 10), cv2.FONT_HERSHEY_SIMPLEX,
                    1, (255, 250, 120), 2)
        
        if data: # Data found is printed
            print("data found:", data)
        
    cv2.imshow("code detector", img) # Display of the live camera feed to the Desktop
    
    if(cv2.waitKey(1) == ord("q")): # 'Q' can be pressed to stop the program
        break
    