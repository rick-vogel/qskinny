############################################################################
# QSkinny - Copyright (C) The authors
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

include("QSkinnyTools")
set(SOURCES main.cpp)
qsk_svg2qvg(01.13.05h.svg 01.13.05h.qvg)
qt_add_resources(SOURCES images.qrc)
qsk_add_example(svgqvgruggededges ${SOURCES})