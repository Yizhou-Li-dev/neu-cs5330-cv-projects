## Report

1. In this project, we implemented 4 programs. The first three acomplished the tasks as required. The last program implemented a new calibration method by using charuco board. 
2.  Required images:
   1. calibration image with chessboard corners highlighted:
      ![1](/Users/yangtianyu/Library/Developer/Xcode/DerivedData/project4-ejgpnouwaunxbcankignujhunuee/Build/Products/Debug/caliberation_images/1.jpg)
   2.  the error estimate:![Screenshot 2023-03-20 at 18.22.24](/Users/yangtianyu/Desktop/Screenshot 2023-03-20 at 18.22.24.png)
   3. 3-D axes screenshot:![Screenshot 2023-03-20 at 18.26.58](/Users/yangtianyu/Library/Application Support/typora-user-images/Screenshot 2023-03-20 at 18.26.58.png)
   4. *phantom battle axe* screenshot:![Screenshot 2023-03-20 at 18.36.39](/Users/yangtianyu/Desktop/Screenshot 2023-03-20 at 18.36.39.png)
   5. About task 7 details, please refer to video https://youtu.be/TeACMVQ1rtg. About Harris corners' function in AR, we can use the detected Harris corners of chessboard as reference points to track the movement of objects in the image.
3. instead of using chessboard, we use charuco board to caliberate the camera, which can still work with minor block of the board.![board](/Users/yangtianyu/Desktop/board.jpeg)
   Here is a example image about the caliberation on the charuco board:![Screenshot 2023-03-20 at 18.52.29](/Users/yangtianyu/Desktop/Screenshot 2023-03-20 at 18.52.29.png)
4. In this project, we learned to implement AR for the first time. Specificly in finding corner,  caliberation, and coordination system conversion, and making 3D object and preject it into video. 
5. Acknowledgement:
   https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a
   https://docs.opencv.org/4.x/dd/d1a/group__imgproc__feature.html#ga354e0d7c86d0d9da75de9b9701a9a87e
   https://docs.opencv.org/3.4/d9/d0c/group__calib3d.html#ga6a10b0bb120c4907e5eabbcd22319022

