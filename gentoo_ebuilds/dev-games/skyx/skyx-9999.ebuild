# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

inherit eutils cmake-utils git-2

EGIT_REPO_URI="https://github.com/AlanSmithee1984/SkyX.git"

MY_P="SkyX-v${PV}"
DESCRIPTION="Plugin for OGRE3D which supplies sky effects"
HOMEPAGE="http://www.paradise-studios.net/?page_id=185"
LICENSE="GNU Lesser GPL"
SLOT="0"
KEYWORDS=""

DEPEND="dev-games/ogre"

RDEPEND="${DEPEND}"

S=${WORKDIR}/${MY_P}

src_prepare() {
        cp "${FILESDIR}"/CMakeLists.txt "${S}" || die
}

src_configure() {

        local mycmakeargs=(
                -DCMAKE_INSTALL_PREFIX=/usr/
		-DSKYX_BUILD_SAMPLES=0
        )

        cmake-utils_src_configure
}

src_compile() {
        cmake-utils_src_install
}

