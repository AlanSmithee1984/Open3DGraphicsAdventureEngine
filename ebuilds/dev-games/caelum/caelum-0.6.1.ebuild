# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=3

inherit eutils cmake-utils 

DESCRIPTION="Atmospheric effects OGRE plugin"
HOMEPAGE="http://www.ogre3d.org/tikiwiki/Caelum"
SRC_URI="http://caelum.googlecode.com/files/${PN}-${PV}.tar.gz"
LICENSE="GNU Lesser GPL"
SLOT="0"
KEYWORDS="~amd64 ~x86"

DEPEND="dev-games/ogre 
	dev-games/ois 
	sys-devel/libtool 
	dev-util/pkgconfig"

RDEPEND="${DEPEND}"

src_unpack() {
	unpack ${A}
	cd ${S}
	if has_version ">=dev-games/ogre-1.8.0"; then 
		epatch "${FILESDIR}"/ogre-1.8.0.patch
        fi
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

