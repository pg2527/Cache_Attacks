# Define Android specific variables
ANDROID_NDK_PATH = /opt/android-ndk
ANDROID_TOOLCHAIN_BIN = ${ANDROID_NDK_PATH}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin
ANDROID_SYSROOT = ${ANDROID_NDK_PATH}/platforms/${ANDROID_PLATFORM}/arch-arm

ANDROID_CC = ${ANDROID_TOOLCHAIN_BIN}/arm-linux-androideabi-gcc
ANDROID_CC_FLAGS = --sysroot=${ANDROID_SYSROOT}

ANDROID_INCLUDES = -I ${ANDROID_NDK_PATH}/platforms/${ANDROID_PLATFORM}/arch-arm/usr/include
ANDROID_CFLAGS = ${ANDROID_INCLUDES} -march=armv7-a -fPIE
ANDROID_LDFLAGS = ${ANDROID_INCLUDES} -march=armv7-a -fPIE
#LIBFLUSH_INC ?= -I /home/user/Documents/Thesis/armageddon/libflush
#LIBFLUSH_LIB ?= -L /home/user/Documents/Thesis/armageddon/libflush/build/armv7/release -lflush

CC = ${ANDROID_CC} ${ANDROID_CC_FLAGS}
CFLAGS += ${ANDROID_CFLAGS}
LDFLAGS += -pie
