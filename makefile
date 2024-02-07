CXX = g++
CXXFLAGS = -std=c++11
LIBS = -L/opt/homebrew/lib -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_core -lopencv_videoio -lopencv_videostab -lopencv_objdetect
INCLUDES = -I/opt/homebrew/include/opencv4


readImage: readImage.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

featureCompute: featureCompute.cpp features.cpp 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

# vidDisplay: vidDisplay.cpp filters.cpp faceDetect.cpp 
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f readImage
	rm -f featureCompute
