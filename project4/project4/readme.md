1. Group members: Yizhou Li, Tianyu Yang

2. videos about the project: https://youtu.be/TeACMVQ1rtg

3. Operating system: MacOS ventura 13.2.1, IDE: Xcode 14.2

4. There are 4 main function in this project.
    * cam_caliberation: Related to task 1-3. While running this part, it will open a camera steam and automatically detect and extract chessboard corners. When press '2', it will execute task 2 if found the chessboard. When press '3' and user has selected 5 or more frames, it will execute task 3. Press 'q' to exit the program.
    * cam_ar: Related to task 4-6. While running this part, it will also open a camera steam with the infomation provided by cam_caliberation. it will automatically execute task 4 and print the rotation and translation data in the terminal. When press '5', it will draw a xyz axis on the chessboard. When press '6', it will draw a *phantom battle axe* on the board. Press 'q' to exit the program.
    * cam_features: Related to task 7. This part shows the Harris corners with red point in the video stream. Press 'q' to exit the program.
    * cam_charuco_caliberation: Related to **extension**. The program needs some terminal params:
      ```"camera_calib_charuco.yaml" -w=5 -h=7 -sl=0.04 -ml=0.02 -d=10```
    
      Press 'c' to add current frame. 'ESC' to finish and calibrate.
    
5. We didn't use travel days in this project.

