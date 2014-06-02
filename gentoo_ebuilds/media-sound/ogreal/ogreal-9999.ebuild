# Copyright 1999-2012 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

inherit autotools git-2

EGIT_REPO_URI="https://github.com/AlanSmithee1984/OgreAL.git"



DESCRIPTION="an OpenAL wrapper for Ogre"
HOMEPAGE="http://sourceforge.net/projects/ogreal"

LICENSE="LGPL-2.1"
SLOT="0"
KEYWORDS=""
IUSE=""

DEPEND=">=dev-games/ogre-1.4
		>=media-libs/openal-0.0.8
		media-libs/libvorbis
		dev-games/ois"
RDEPEND=${DEPEND}

EGIT_BOOTSTRAP="eautoreconf"


src_install() {
	emake DESTDIR="${D}" install || die "emake install failed"
}
