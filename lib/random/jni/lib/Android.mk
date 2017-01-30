LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := first
LOCAL_SRC_FILES := libfirst.so
LOCAL_SHARED_LIBRARIES := first
include $(PREBUILT_SHARED_LIBRARY)

