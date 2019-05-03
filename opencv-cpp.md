1. Install OpenCV using apt/opkg
	<!-- here -->

-----------------------------------------------------------------------------------------
2. Compile the .cpp with:
	g++ face_detect-opencv.cpp -o app `pkg-config --cflags --libs opencv`

-----------------------------------------------------------------------------------------
3. Run the software with:
	./app --cascade="/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml" --nested-cascade="haarcascade_eye_tree_eyeglasses.xml" --scale=1.3
	Toradex ./app