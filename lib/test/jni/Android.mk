LOCAL_PATH :=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE :=test
LOCAL_SRC_FILES :=mylib2.c
include $(BUILD_SHARED_LIBRARY)
