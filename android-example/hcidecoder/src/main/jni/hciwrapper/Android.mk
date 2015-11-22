LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hciwrapper

LOCAL_SRC_FILES := hci_decode.cpp

LOCAL_SHARED_LIBRARIES := hcidecoder

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
