CORSS_PREFIX ?=
SYSROOT ?=

RELEASE ?= 1
BITS ?=
MAC ?=0

$(warning $(CORSS_PREFIX))
$(warning $(SYSROOT))

CC := $(CORSS_PREFIX)g++
AR := $(CORSS_PREFIX)ar
RM := rm -rf

#原始目录
SOURCE_PATH :=./OpencvCube
SOURCE :=
SOURCE += $(wildcard $(SOURCE_PATH)/*.cpp)

#源文件
SOURCE_CODE = $(SOURCE)
#中间文件
OBJS = $(SOURCE_CODE:.cpp=.o)
OBJS_MODULE = $(SOURCE_CODE:.cpp=.o)

OBJS_cubeOut=$(OBJS_MODULE)
TARGET=cubeOut

ALL_OBJS=$(OBJS)

#动态库
LIBS ?= 
LIBS += pthread dl
LIBS += opencv_core opencv_calib3d opencv_contrib opencv_features2d opencv_flann opencv_gpu 
LIBS += opencv_highgui opencv_imgproc opencv_legacy opencv_ml
LIBS += opencv_nonfree opencv_objdetect opencv_ocl opencv_photo opencv_stitching
LIBS += opencv_superres opencv_video opencv_videostab
FRAMEWORK ?=

#头文件路径
INCLUDE_PATH ?=
INCLUDE_PATH += $(SYSROOT)/usr/include
INCLUDE_PATH += $(pwd)
INCLUDE_PATH += .
INCLUDE_PATH += ..

#动态库路径
LIBRARY_PATH ?=
#LIBRARY_PATH += $(SYSROOT)/usr/lib/ $(SYSROOT)/usr/local/lib/
#LIBRARY_PATH += $(SYSROOT)/usr/lib64/ $(SYSROOT)/usr/local/lib64/

CFLAGS ?=
LFLAGS ?=

CFLAGS += -Wall -DMAIN_TEST -DUSE_BOOL -std=c++14 -D_POSIX_C_SOURCE=199506L -D_GNU_SOURCE -g

#头文件
CFLAGS += $(foreach dir,$(INCLUDE_PATH),-I$(dir))

#库路径
LDFLAGS += $(foreach lib,$(LIBRARY_PATH),-L$(lib))

#库名
LDFLAGS += $(foreach lib,$(LIBS),-l$(lib))
LDFLAGS += $(foreach lib,$(FRAMEWORK),-framework $(lib))

#检查版本
ifeq ($(RELEASE),0)
	#debug
	CFLAGS += -g
else
	#release
	CFLAGS += -O3 -DNDEBUG
endif

#检查位宽
ifeq ($(BITS),32)
	CFLAGS += -m32
	LFLAGS += -m32
else
	ifeq ($(BITS),64)
		CFLAGS += -m64
		LFLAGS += -m64
	else
	endif
endif

$(warning $(OBJS))

#操作命令
all:clean build

$(ALL_OBJS):%.o:%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET):$(ALL_OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(OBJS_$@) $(LDFLAGS)

build:$(TARGET)
	$(RM) $(ALL_OBJS)

clean:
	echo $(SRCS)
	$(RM) $(ALL_OBJS) $(TARGET)
