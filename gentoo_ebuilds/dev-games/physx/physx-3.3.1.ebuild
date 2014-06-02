# Copyright 1999-2009 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=5

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

S=${WORKDIR}


pkg_nofetch() {

        einfo "Please download '${A}' from:"
        einfo "https://developer.nvidia.com/gameworksdownload"
        einfo "and move it to '${DISTDIR}'"
}


src_unpack() {
	unpack ${A}

	cd "${WORKDIR}"
}


src_install() {

	insinto /usr/lib/${PN}/

	LIBPATH="Lib"

        if use x86; then
                LIBPATH=${LIBPATH}/linux32
        elif use amd64; then
                LIBPATH=${LIBPATH}/linux64
        fi

	doins ${LIBPATH}/* ||  die "Install failed!"



	# header
	INCLUDEPATH="${S}/Include/"
	HEADER_DEST="/usr/include/${PN}/"

	insinto  ${HEADER_DEST} 
	doins -r ${INCLUDEPATH}/*

}


