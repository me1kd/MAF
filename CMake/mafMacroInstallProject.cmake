#
#  mafMacroInstallProject.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2011 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#

MACRO(mafMacroInstallProject executable)

set(IS_TEST 0)

string(REGEX MATCH "(^mafQA$)|(.*Test$) | " IS_TEST ${PROJECT_NAME})

set(uis_hdrs)
  
  qt5_wrap_ui( uis_hdrs ${ui_file_list} )
  
  #qt5_add_resources(UI_RESOURCES bsed.qrc)

# mafResource -> QGui for QMatrix4x4
# mafGUI -> uitools
# maf*Test -> Test
if(IS_TEST)
    qt5_use_modules(${PROJECT_NAME} Core Widgets Sql Xml Gui Concurrent UiTools Test)
else(IS_TEST)
    qt5_use_modules(${PROJECT_NAME} Core Widgets Sql Xml Gui Concurrent UiTools)
endif(IS_TEST)

SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" )

if(${executable})
  IF (APPLE)
    set(lib_ext "dylib")
    
    set(TARGET_LOC)
    set(DEST_ABSOLUTE_DIR)
    get_target_property(TARGET_LOC ${PROJECT_NAME} LOCATION)
    get_filename_component(DEST_ABSOLUTE_DIR ${TARGET_LOC} PATH)

    set(MACOS_BUNDLE_DIR "${DEST_ABSOLUTE_DIR}")
# DEBUG
    SET(SOURCE_DIR   "${LIBRARY_OUTPUT_PATH}/Debug")
    SET(TARGET_DIR   ${MACOS_BUNDLE_DIR})
    
    FILE(GLOB file_list ${SOURCE_DIR}/*.${lib_ext})    
    foreach(filelib ${file_list})
      get_filename_component(fileName ${filelib} NAME)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                          COMMAND ${CMAKE_COMMAND} -E copy ${filelib} ${TARGET_DIR}/${fileName} )
    endforeach(filelib ${file_list})
    
    set(file_list)
#RELEASE
    SET(SOURCE_DIR   "${LIBRARY_OUTPUT_PATH}/Release")
    SET(TARGET_DIR   ${MACOS_BUNDLE_DIR})
    
    FILE(GLOB file_list ${SOURCE_DIR}/*.${lib_ext})    
    foreach(filelib ${file_list})
      get_filename_component(fileName ${filelib} NAME)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                          COMMAND ${CMAKE_COMMAND} -E copy ${filelib} ${TARGET_DIR}/${fileName} )
    endforeach(filelib ${file_list})
    set(file_list)
#
    SET(SOURCE_DIR   "${LIBRARY_OUTPUT_PATH}")
    SET(TARGET_DIR   ${MACOS_BUNDLE_DIR})
    
    FILE(GLOB file_list ${SOURCE_DIR}/*.${lib_ext})    
    foreach(filelib ${file_list})
      get_filename_component(fileName ${filelib} NAME)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                          COMMAND ${CMAKE_COMMAND} -E copy ${filelib} ${TARGET_DIR}/${fileName} )
    endforeach(filelib ${file_list})

  ENDIF (APPLE)
endif(${executable})
  
ENDMACRO()
