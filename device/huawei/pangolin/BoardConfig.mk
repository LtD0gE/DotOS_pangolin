# From https://github.com/rcstar6696/android_device_huawei_phone/blob/aosp/BoardConfig.mk
include device/huawei/pangolin/HiSiCommon.mk
include vendor/dot/config/BoardConfigSoong.mk
TARGET_OTA_ASSERT_DEVICE := phone
TARGET_KERNEL_SOURCE := kernel/huawei/pangolin
TARGET_KERNEL_CONFIG := kernel/huawei/pangolin/arch/arm64/configs/merge_kirin710_defconfig

