SUMMARY = "rick OEM commands"
DESCRIPTION = "rick OEM commands"
HOMEPAGE = "https://github.com/rick0118/rick-ipmi-oem.git"

PR = "r1"
PV = "0.1+git${SRCPV}"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"


SRC_URI = "git://github.com/rick0118/rick-ipmi-oem;branch=master;protocol=https"



SRCREV = "e1ba8133ba9db8aecca8e0fe2964c4a7da277e19"


DEPENDS += " \
    boost \
    phosphor-ipmi-host \
    phosphor-logging \
    systemd \
    sdbusplus \
    phosphor-dbus-interfaces \
    libgpiod \
    autoconf-archive-native \
    "

S = "${WORKDIR}/git"
#inherit meson
inherit cmake obmc-phosphor-ipmiprovider-symlink
inherit pkgconfig


LIBRARY_NAMES = "librickoemcmds.so"
HOSTIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"
NETIPMI_PROVIDER_LIBRARY += "${LIBRARY_NAMES}"

FILES:${PN}:append = " ${libdir}/ipmid-providers/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/host-ipmid/lib*${SOLIBS}"
FILES:${PN}:append = " ${libdir}/net-ipmid/lib*${SOLIBS}"
FILES:${PN}-dev:append = " ${libdir}/ipmid-providers/lib*${SOLIBSDEV} ${libdir}/ipmid-providers/*.la"
#FILES:${PN}-dev:append = " ${libdir}/ipmid-providers/lib*${SOLIBSDEV}"
do_install:append(){
   install -d ${D}${includedir}/rick-ipmi-oem
   install -m 0644 -D ${S}/include/*.hpp ${D}${includedir}/rick-ipmi-oem
}