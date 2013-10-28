# Copyright 1999-2007 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

inherit git-2

EGIT_REPO_URI="https://github.com/imperative/CommunityHydrax.git"
#EGIT_PROJECT=""

DESCRIPTION="Water simulation plugin for Ogre"
HOMEPAGE="http://www.ogre3d.org/tikiwiki/Hydrax"
LICENSE="GNU Lesser GPL"
SLOT="0"
KEYWORDS=""

DEPEND="dev-games/ogre[boost]
	"

RDEPEND="${DEPEND}"


