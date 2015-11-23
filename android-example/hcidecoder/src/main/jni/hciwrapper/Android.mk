LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hciwrapper

LOCAL_CFLAGS := -std=gnu++11
LOCAL_CPPFLAGS += -fexceptions

INCLUDE_SRC_DIR       := ../../../../../../src
INCLUDE_EXTERNAL_DIR  := ../../../../../../external_libs/include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(INCLUDE_SRC_DIR)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/${INCLUDE_EXTERNAL_DIR}

LOCAL_SRC_FILES  := hci_decode.cpp

LOCAL_SHARED_LIBRARIES := hcidecoder btsnoopdecoder

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
