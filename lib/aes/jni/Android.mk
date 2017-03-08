LOCAL_PATH := $(call my-dir)
my_LOCAL_PATH := $(LOCAL_PATH)
TARGET_ARCH_ABI := armeabi-v7a
TARGET_ARCH_PLATFORM := android-14
include /home/parul/Documents/android-openssl/jni/openssl/Android.mk
include /home/parul/Documents/Thesis/armageddon/libflush/example/Android.mk
LOCAL_PATH := $(my_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -fPIC -std=c11 -g
LOCAL_LDFLAGS +=-fPIE -pie
LOCAL_LDLIBS := -llog
LOCAL_MODULE    := spy
LOCAL_SRC_FILES := spy.c
LOCAL_C_INCLUDES += /home/parul/openssl/include/
LOCAL_C_INCLUDES += /home/parul/Documents/Thesis/armageddon/libflush/armv7/
LOCAL_SHARED_LIBRARIES := crypto ssl flush
include $(BUILD_EXECUTABLE)

