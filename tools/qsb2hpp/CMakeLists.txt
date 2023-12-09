############################################################################
# QSkinny - Copyright (C) 2016 Uwe Rathmann
#           SPDX-License-Identifier: BSD-3-Clause
############################################################################

set(target qsb2hpp)
qsk_add_executable(${target} main.cpp)
target_link_libraries(${target} PRIVATE Qt::Core Qt::GuiPrivate)
set_target_properties(${target} PROPERTIES FOLDER tools)
install(TARGETS ${target})