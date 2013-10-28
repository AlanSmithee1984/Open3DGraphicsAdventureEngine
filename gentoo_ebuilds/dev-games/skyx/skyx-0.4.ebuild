# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

inherit eutils cmake-utils

MY_P="SkyX-v${PV}"
DESCRIPTION="Plugin for OGRE3D which supplies sky effects"
HOMEPAGE="http://www.paradise-studios.net/?page_id=185"
LICENSE="GNU Lesser GPL"
SRC_URI="http://modclub.rigsofrods.com/xavi/SkyX/${MY_P}.rar"
SLOT="0"
KEYWORDS="~amd64 ~x86"

DEPEND="dev-games/ogre"

RDEPEND="${DEPEND}"

S=${WORKDIR}/${MY_P}

src_prepare() {
        cp "${FILESDIR}"/CMakeLists.txt "${S}" || die
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

