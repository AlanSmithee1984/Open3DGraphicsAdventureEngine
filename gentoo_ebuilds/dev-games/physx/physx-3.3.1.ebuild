# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

DESCRIPTION="NVIDIA PhysX"
HOMEPAGE="http://developer.nvidia.com/object/physx.html"
SRC_URI="http://developer.download.nvidia.com/${PN}/${PV}/${PN}-${PV}_linux_sdk_core.zip"

LICENSE="AGEIA PhysX Driver License Agreement"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE=""

DEPEND="app-arch/unzip"
RDEPEND=""

#inherit rpm


pkg_nofetch() {

        einfo "Please download '${A}' from:"
        einfo "https://developer.nvidia.com/gameworksdownload"
        einfo "and move it to '${DISTDIR}'"
}


src_unpack() {
#	PACKET=${DISTDIR}/"physx-3.3.1_linux_sdk_core.zip"

#	cp ${PACKET} ${WORKDIR}
	unpack ${A}

	cd "${WORKDIR}"
}

src_install() {
	# libphysx
	insinto /usr/lib/physx/

	LIBPATH="Lib"

        if use x86; then
                LIBPATH=${LIBPATH}/linux32
        elif use amd64; then
                LIBPATH=${LIBPATH}/linux64
        fi

	doins ${WORKDIR}/${LIBPATH}/*
}


