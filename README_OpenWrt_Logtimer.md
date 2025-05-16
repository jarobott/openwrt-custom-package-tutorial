# OpenWrt C++ Package Build Guide (example for GL.iNet MT3000)

This guide walks you through building and deploying a simple C++ app using the OpenWrt SDK. Example is for the **GL.iNet MT3000 (Slate AX)** router (MediaTek Filogic target) but the method will work for other routers running OpenWrt.

---

## 1. Install Prerequisites on local machine

```bash
$ sudo apt update
$ sudo apt install build-essential clang flex bison g++ gawk \
    gcc-multilib g++-multilib gettext git libncurses5-dev libssl-dev \
    python3-setuptools rsync swig unzip zlib1g-dev file wget
```

---

## 2. Download and Extract OpenWrt SDK

- Download the SDK for your router:  
  https://downloads.openwrt.org/releases/24.10.1/targets/mediatek/filogic/

- Verify the release and target (GL.iNet MT3000 = `mediatek/filogic`).

```bash
$ tar -xf openwrt-sdk-*.tar.xz
$ cd openwrt-sdk-*/
```

---

## 3. Set Up Package Directory

```bash
$ mkdir -p package/logtimer/src
```

---

## 4. Write the C++ Program

Create `package/logtimer/src/logtimer.cpp`:

```cpp
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>

int main() {
    std::ofstream log("/tmp/logtimer.log", std::ios::app);
    int counter = 0;
    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        log << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
            << " Count: " << counter++ << std::endl;
        log.flush();
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    return 0;
}
```

---

## 5. Create the OpenWrt Makefile

Create `package/logtimer/Makefile`:

```make
include $(TOPDIR)/rules.mk

PKG_NAME:=logtimer
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/logtimer
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Simple C++ log timer
  MAINTAINER:=Your Name <you@example.com>
  DEPENDS += +libstdcpp
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
```

---

## 6. Empty Source Makefile (Optional)

Create `package/logtimer/src/Makefile`:

```make
# intentionally left empty
```

---

## 7. Register the Package

```bash
$ make menuconfig
```

- Navigate to **Utilities → logtimer**
- Press `M` to build it as a module
- Save and exit

---

## 8. Build the Package

```bash
$ make package/logtimer/compile V=s
```

- Output `.ipk` file will appear in:

  ```
  bin/packages/aarch64_cortex-a53/base/logtimer_1.0-1_aarch64_cortex-a53.ipk
  ```

---

## 9. Transfer the Package to Your Router

Use legacy SCP mode to bypass SFTP:

```bash
$ scp -O bin/packages/aarch64_cortex-a53/base/logtimer_1.0-1_aarch64_cortex-a53.ipk root@192.168.1.1:/tmp/
```

---

## 10. SSH Into Your Router

```bash
$ ssh root@192.168.1.1
```

---

## 11. Install the `.ipk` Package

```bash
$ opkg install /tmp/logtimer_1.0-1_aarch64_cortex-a53.ipk
```

---

## 12. Run the Program in Background

```bash
$ logtimer &
```

Logs will be written to `/tmp/logtimer.log`.

---

## 13. Run at Boot

1. Create `/etc/init.d/logtimer`:

```sh
#!/bin/sh /etc/rc.common

START=99
STOP=10

start() {
    echo "Starting logtimer"
    /usr/bin/logtimer &
}

stop() {
    echo "Stopping logtimer"
    killall logtimer
}
```

2. Make it executable and enable at boot:

```bash
$ chmod +x /etc/init.d/logtimer
$ /etc/init.d/logtimer enable
$ /etc/init.d/logtimer start
```
