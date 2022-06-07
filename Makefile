# Executable
EXE := PicTool.exe

# Flag
FLG := -Wall -O3 -finput-charset=UTF-8 -fexec-charset=UTF-8
FLG += -ID:/Code/msys2/mingw64/bin/../include/opencv4

# Library
LIB := -LD:/Code/msys2/mingw64/bin/../lib -lopencv_gapi -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_barcode -lopencv_bgsegm -lopencv_ccalib -lopencv_cvv -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_ovis -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core

# Object
OBJ := main.o
OBJ += fx/BCurve.o fx/ColorKey.o fx/GCurve.o fx/LCurve.o fx/RCurve.o fx/SCurve.o
OBJ += parade/HSIChannel.o parade/LHistogram.o parade/RGBChannel.o parade/RGBHistogram.o parade/RGBParade.o parade/YUVParade.o

# Default build
.cpp.o:
	@echo "Building:" $*.o
	@g++ $(FLG) -c -o $*.o $< $(LIB)
	@echo "Done"

$(EXE): $(OBJ)
	@echo "Building:" $(EXE)
	@g++ $(FLG) -o $@ $(OBJ) $(LIB)
	@echo "Done"

clean:
	rm $(EXE) $(OBJ)
