#
# CMakeLists.txt - Extension System CMake file
#
# Copyright (c) 2010 Paul Giblock <pgib/at/users.sourceforge.net>
#
# This file is part of Unison - http://unison.sourceforge.net/
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program (see COPYING); if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA.
#

include(FindQt4)

set(EXTENSIONSYSTEM_SRCS
    IExtension.cpp
    OptionsParser.cpp
    ExtensionCollection.cpp
    ExtensionInfo.cpp
    ExtensionManager.cpp
)

set(EXTENSIONSYSTEM_HEADERS
    IExtension.hpp
    OptionsParser.hpp
    ExtensionCollection.hpp
    ExtensionInfo.hpp
    ExtensionManager.hpp
    extensionsystem_global.hpp
)

set(EXTENSIONSYSTEM_MOC_HEADERS
    IExtension.hpp
    ExtensionInfo_p.hpp
    ExtensionManager.hpp
)

qt4_wrap_cpp(EXTENSIONSYSTEM_MOC_SRCS ${EXTENSIONSYSTEM_MOC_HEADERS})

include_directories(
    ..
)

add_library(extensionsystem SHARED ${EXTENSIONSYSTEM_MOC_SRCS} ${EXTENSIONSYSTEM_SRCS})
    
target_link_libraries(extensionsystem
    ${QT_LIBRARIES}
    aggregation
)

install(TARGETS extensionsystem LIBRARY
        DESTINATION lib/unison)

install(FILES ${EXTENSIONSYSTEM_HEADERS}
        DESTINATION include/unison/extensionsystem)

# vim: tw=90 ts=8 sw=2 sts=2 et sta noai
