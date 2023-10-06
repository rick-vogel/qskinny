############################################################################
# QSkinny - Copyright (C) 2016 Uwe Rathmann
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

set(SOURCES 
    main.cpp
)

qsk_add_example(blurring ${SOURCES})
target_link_libraries(blurring PRIVATE Qt::QuickWidgets)

