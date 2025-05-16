include $(TOPDIR)/rules.mk

PKG_NAME:=logtimer
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/logtimer
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Simple C++ log timer
  MAINTAINER:=Jared Bellingham <jared@robots.ox.ac.uk>
  DEPENDS:=+libstdcpp
endef

define Package/logtimer/description
  A simple C++ app that logs timestamped messages every 10 seconds.
endef

define Build/Compile
	$(TARGET_CXX) $(TARGET_CFLAGS) -std=c++17 -O2 \
		$(PKG_BUILD_DIR)/logtimer.cpp -o $(PKG_BUILD_DIR)/logtimer
endef

define Package/logtimer/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/logtimer $(1)/usr/bin/
endef

$(eval $(call BuildPackage,logtimer))
