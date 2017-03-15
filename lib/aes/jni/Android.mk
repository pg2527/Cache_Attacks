LOCAL_PATH := $(call my-dir)
my_LOCAL_PATH := $(LOCAL_PATH)
TARGET_ARCH_ABI := armeabi-v7a
TARGET_ARCH_PLATFORM := android-21
include /home/parul/Documents/android-openssl/jni/openssl/Android.mk
include /home/parul/Documents/Thesis/armageddon/libflush/example/Android.mk
LOCAL_PATH := $(my_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_CFLAGS += -fPIC -std=c11 -g
LOCAL_LDFLAGS +=-fPIE -pie
LOCAL_LDLIBS := -llog
#LOCAL_MODULE    := aes_encrypt	
#LOCAL_SRC_FILES := aes_encrypt.c
LOCAL_MODULE    := random2
LOCAL_SRC_FILES := spy.c
LOCAL_C_INCLUDES += /home/parul/Documents/root/openssl-1.0.1g/include/
LOCAL_C_INCLUDES += /home/parul/Documents/Thesis/armageddon/libflush/armv7/
LOCAL_SHARED_LIBRARIES := libcrypto libssl libflush
include $(BUILD_EXECUTABLE)


