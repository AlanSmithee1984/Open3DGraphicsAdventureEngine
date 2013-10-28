# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

inherit eutils cmake-utils subversion

ESVN_REPO_URI="http://svn.code.sf.net/p/ogreaddons/code/branches/ogrenewt/"


DESCRIPTION="Plugin for connecting OGRE3D with Newton"
HOMEPAGE="http://www.ogre3d.org/tikiwiki/tiki-index.php?page=OgreNewt+2"
LICENSE="GNU Lesser GPL"
SLOT="0"
KEYWORDS="~amd64 ~x86"

DEPEND="dev-games/ogre 
	dev-games/ois 
	sys-devel/libtool 
	dev-util/pkgconfig
	>=dev-games/newton-2.19"

RDEPEND="${DEPEND}"

src_prepare() {
	einfo ${S}/newton20/
	mv ${S}/newton20/* ${S}

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

