#!/bin/sh
#
# Создание DEB пакета
#

echo "*"
echo "* DO_PACKAGE"
echo "*"

MY_INSTALL_PATH=$(readlink -f .package)/usr/local
MY_PACKAGE_VERSION="${PACKAGE_VERSION:-$(date +"%s")}" 
MY_PACKAGE_NAME=ota-backend_${MY_PACKAGE_VERSION}_all.deb

#remove old version
grep -v "Version:" .package/DEBIAN/control > /var/tmp/tmp_file && mv /var/tmp/tmp_file .package/DEBIAN/control

# permission must be >=0555 and <=0775
echo "SETUP PERMISSION"
chmod 0755 .package/DEBIAN
chmod 0755 .package/DEBIAN/control
chmod 0555 .package/DEBIAN/postrm
chmod 0555 .package/DEBIAN/prerm
chmod 0555 .package/DEBIAN/preinst
chmod 0555 .package/DEBIAN/postinst

echo "Version: $MY_PACKAGE_VERSION" >> .package/DEBIAN/control
grep -i 'Package' .package/DEBIAN/control

# права на запуск
mkdir -p .package/home/user/ota-backend
mv ./dcoll .package/home/user/ota-backend/
mv ./config.json .package/home/user/ota-backend/

chmod +x -R .package/home/user/ota-backend/*

dpkg-deb -Zxz --build .package
# меняем название пакета
mv .package.deb $MY_PACKAGE_NAME

