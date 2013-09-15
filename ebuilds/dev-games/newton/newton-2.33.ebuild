# Copyright 1999-2011 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=3

inherit eutils toolchain-funcs

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
	epatch "${FILESDIR}"/newton-2.33-ASSERT-to-ASSERTE.patch
	PACKAGE_DIR=""
	if [[ ${ARCH} == "amd64" ]]; then
		PACKAGE_DIR="linux64"
	else
		if [[ ${ARCH} == "x86" ]]; then
			PACKAGE_DIR="linux32"
		else
			die "Unhandled ARCH ${ARCH}"
		fi
	fi
	mkdir packages/${PACKAGE_DIR}
}

src_compile() {
	cd coreLibrary_200/projets/${PACKAGE_DIR}
	emake -f makefile
}

src_install() {
	insinto /usr/$(get_libdir)
	doins packages/${PACKAGE_DIR}/libNewton.so
	insinto /usr/include
	doins packages/${PACKAGE_DIR}/Newton.h
}
