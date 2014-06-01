# Copyright 1999-2014 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

inherit eutils mercurial cmake-utils

DESCRIPTION="PhysX Wrapper for Ogre3D"
SRC_URI=""
EHG_REPO_URI="https://bitbucket.org/Emperor2k3/ogrephysx/"
#EHG_PROJECT="${PN%-hg}"
HOMEPAGE="http://www.ogre3d.org/forums/viewtopic.php?f=5&t=54763"
KEYWORDS=""
SLOT="0"
LICENSE="MIT"
IUSE=""

DEPEND="dev-games/ogre
	dev-games/PhysX"
RDEPEND="${DEPEND}"

src_configure() {

        local mycmakeargs=(
                -DCMAKE_INSTALL_PREFIX=/usr/
        )

        cmake-utils_src_configure
}

src_compile() {
        cmake-utils_src_install
}

