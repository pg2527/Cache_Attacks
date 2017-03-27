LOCAL_PATH := $(call my-dir)
#my_LOCAL_PATH := $(LOCAL_PATH)
#include $(LOCAL_PATH)/lib/Android.mk
#LOCAL_PATH := $(my_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie
LOCAL_MODULE    := first
LOCAL_SRC_FILES := first.c
#LOCAL_SHARED_LIBRARIES := first
#LOCAL_LDFLAGS := -Wl,-rpath,/home/usr/Documents/Thesis/lib/random/lib -Wl,-rpath,. -L/home/usr/Documents/Thesis/lib/random/lib -lfirst
include $(BUILD_SHARED_LIBRARY)
