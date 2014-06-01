# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=3

inherit eutils cmake-utils subversion

ESVN_REPO_URI="svn://svn.code.sf.net/p/ogre-ccs/code/"
ESVN_PROJECT="ogre-ccs-code"

DESCRIPTION="Camera Control System for Ogre"
HOMEPAGE="http://ogre-ccs.sourceforge.net"
LICENSE="GNU Lesser GPL"
SLOT="0"
KEYWORDS=""

DEPEND="dev-games/ogre
	dev-games/ois"

RDEPEND="${DEPEND}"

#src_prepare() {
#	epatch "${FILESDIR}/disable_demo.patch"
#	epatch "${FILESDIR}/add_install.patch"
#}

S=${S}/trunk

src_configure() {
	local mycmakeargs=(
		-DCMAKE_INSTALL_PREFIX=/usr/
	)
	cmake-utils_src_configure
}

src_compile() {
	cmake-utils_src_install
}

