LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.kirin.rc
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := etc/init.kirin.rc
LOCAL_MODULE_PATH  := $(TARGET_OUT_ETC)/init
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := init.kirin.environ.rc
LOCAL_POST_INSTALL_CMD := \
    mkdir -p $(PRODUCT_OUT)/system/etc/init && \
    sed -e 's?%BOOTCLASSPATH%?$(PRODUCT_BOOTCLASSPATH)?g' $(LOCAL_PATH)/etc/init.kirin.environ.rc > $(PRODUCT_OUT)/system/etc/init/init.kirin.environ.rc && \
    sed -i -e 's?%SYSTEMSERVERCLASSPATH%?$(PRODUCT_SYSTEM_SERVER_CLASSPATH)?g' $(PRODUCT_OUT)/system/etc/init/init.kirin.environ.rc
include $(BUILD_PHONY_PACKAGE)
