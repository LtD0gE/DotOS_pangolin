LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SDK_VERSION := current
LOCAL_MODULE_TAGS := optional
LOCAL_PACKAGE_NAME := openkirin-overlay-burnin
LOCAL_MODULE_PATH := $(TARGET_OUT)/overlay
LOCAL_IS_RUNTIME_RESOURCE_OVERLAY := true
include $(BUILD_PACKAGE)
