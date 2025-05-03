.RECIPEPREFIX := $() $()
CXX = g++
RM = rm -f
CXXFLAGS=-Wall -std=c++20
# CPPFLAGS = -I/usr/include/opencv4 -Wall -std=c++20
# LDLIBS = \
# 			-lopencv_gapi             -lopencv_stitching        -lopencv_alphamat               \
# 			-lopencv_aruco            -lopencv_bgsegm           -lopencv_bioinspired            \
# 			-lopencv_ccalib           -lopencv_cvv              -lopencv_dnn_objdetect          \
# 			-lopencv_dnn_superres     -lopencv_dpm              -lopencv_face                   \
# 			-lopencv_freetype         -lopencv_fuzzy            -lopencv_hdf                    \
# 			-lopencv_hfs              -lopencv_img_hash         -lopencv_intensity_transform    \
# 			-lopencv_line_descriptor  -lopencv_mcc                                              \
# 			-lopencv_quality          -lopencv_rapid            -lopencv_reg                    \
# 			-lopencv_rgbd             -lopencv_saliency         -lopencv_signal                 \
# 			-lopencv_stereo           -lopencv_structured_light                                 \
# 			-lopencv_phase_unwrapping -lopencv_superres                                         \
# 			-lopencv_optflow          -lopencv_surface_matching -lopencv_tracking               \
# 			-lopencv_highgui          -lopencv_datasets         -lopencv_text                   \
# 			-lopencv_plot             -lopencv_videostab        -lopencv_videoio                \
# 			-lopencv_viz              -lopencv_wechat_qrcode    -lopencv_xfeatures2d            \
# 			-lopencv_shape            -lopencv_ml               -lopencv_ximgproc               \
# 			-lopencv_video            -lopencv_xobjdetect       -lopencv_objdetect              \
# 			-lopencv_calib3d          -lopencv_imgcodecs        -lopencv_features2d             \
# 			-lopencv_dnn              -lopencv_flann            -lopencv_xphoto                 \
# 			-lopencv_photo            -lopencv_imgproc          -lopencv_core                   \
CPPFLAGS = -I/usr/include/ImageMagick-7  -Wall -std=c++20
LDLIBS = \
		-fopenmp                      -DMAGICKCORE_HDRI_ENABLE=1          \
		-DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_CHANNEL_MASK_DEPTH=32  \
		-lMagick++-7.Q16HDRI          -lMagickWand-7.Q16HDRI              \
		-lMagickCore-7.Q16HDRI

SRC_DIR = src
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIR)))
OBJS = $(subst .cpp,.o,$(SRCS))

all: main

main: $(OBJS)
    $(CXX) $(LDFLAGS) -o main $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
    $(RM) ./.depend
    $(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
    $(RM) $(OBJS)

distclean: clean
    $(RM) *~ .depend

include .depend
