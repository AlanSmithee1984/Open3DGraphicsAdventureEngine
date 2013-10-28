# Copyright 1999-2012 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=0

inherit subversion autotools

ESVN_REPO_URI="https://ogreal.svn.sourceforge.net/svnroot/ogreal/trunk/OgreAL-Eihort"

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

ESVN_BOOTSTRAP="eautoreconf"

src_unpack() {
	subversion_src_unpack

        if has_version ">=dev-games/ogre-1.8.0"; then 
                epatch "${FILESDIR}"/ogre-1.8.0.patch
        fi

	# patch for strange sky problem
	epatch "${FILESDIR}"/skybox.patch

}

src_install() {
	emake DESTDIR="${D}" install || die "emake install failed"
}
