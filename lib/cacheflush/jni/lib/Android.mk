LOCAL_PATH :=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := test
LOCAL_SRC_FILES := libtest.so
#LOCAL_SHARED_LIBRARIES :=test
include $(PREBUILT_SHARED_LIBRARY)
