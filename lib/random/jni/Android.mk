LOCAL_PATH := $(call my-dir)
my_LOCAL_PATH := $(LOCAL_PATH)
include $(LOCAL_PATH)/lib/Android.mk
LOCAL_PATH := $(my_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -fPIC
LOCAL_LDFLAGS +=-fPIE -pie
LOCAL_MODULE    := random
LOCAL_SRC_FILES := random.c
LOCAL_SHARED_LIBRARIES := first
include $(BUILD_EXECUTABLE)

