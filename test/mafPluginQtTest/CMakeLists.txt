#
#  CMakeLists.txt
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2012 SCS-B3C. All rights reserved.
#
#  See License at: http://tiny.cc/QXJ4D
#
#

## #################################################################
## Find the external packages.
## #################################################################

## #################################################################
## Init Project
## #################################################################

mafMacroInitProject("plugin")
add_definitions(-DQT_PLUGIN)
## #################################################################
## Build rules
## #################################################################

# Create the executable.
ADD_LIBRARY(${PROJECT_NAME} SHARED ${PROJECT_SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBS})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ".mafplugin")

## #################################################################
## Installation
## #################################################################

mafMacroBuildProject("test")

