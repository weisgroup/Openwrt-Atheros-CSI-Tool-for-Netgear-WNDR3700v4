include $(TOPDIR)/rules.mk

PKG_NAME:=onlineRecvCSI
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/onlineRecvCSI
	SECTION:=CSI
	CATEGORY:=CSI
	TITLE:=CSI Online Receive Data
endef

define Package/onlineRecvCSI/description
	CSI Online Receive Data utility
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/onlineRecvCSI/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/onlineRecvCSI $(1)/bin/
endef

$(eval $(call BuildPackage,onlineRecvCSI))

