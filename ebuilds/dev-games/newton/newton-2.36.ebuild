# Copyright 1999-2011 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=3

inherit eutils toolchain-funcs cmake-utils

MY_P="${PN}-dynamics-${PV}"

DESCRIPTION="an integrated solution for real time simulation of physics environments"
HOMEPAGE="http://code.google.com/p/newton-dynamics/"
SRC_URI="http://newton-dynamics.googlecode.com/files/${MY_P}.rar"

LICENSE="ZLIB"
SLOT="0"
KEYWORDS="~amd64 ~x86"
IUSE=""

RDEPEND=""
DEPEND="${DEPEND}
	|| ( app-arch/unrar app-arch/unrar-gpl )"

S="${WORKDIR}"/${MY_P}

src_prepare() {
	cp ${FILESDIR}/CMakeLists.txt ${S}
	cp ${FILESDIR}/*.cmake ${S}
}

src_configure() {
	local mycmakeargs=(
                -DCMAKE_INSTALL_PREFIX=/usr/
        )

        cmake-utils_src_configure
}

src_compile() {
        cmake-utils_src_install
}


