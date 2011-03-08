#
#  mafMacroFoundationLibraries.cmake
#  maf
#
#  Created by Daniele Giunchi - Paolo Quadrani on 10/09/10.
#  Copyright 2009 B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#
MACRO(mafCopyFoundationLibraries vardir)
#copy all the libraries inside Install/bin

SET(result)
SET(erro)
FILE(GLOB file_list ${vardir}/*.*)

foreach(file ${file_list})
get_filename_component(fileName ${file} NAME)

set(result)
set(erro)

EXECUTE_PROCESS(
   COMMAND ${CMAKE_COMMAND} -E copy
   ${file}
   ${CMAKE_BINARY_DIR}/Install/bin/${CMAKE_CFG_INTDIR}/${fileName}
   RESULT_VARIABLE result
   ERROR_VARIABLE erro)
endforeach(file ${filelist})
ENDMACRO()

MACRO(mafMacroFoundationLibraries)

include(FindVXL)
include(FindQtSOAP)
include(FindQXMLRPC)
include(FindQScxml)

set(MAF_FOUNDATION_LIBRARIES ${QXMLRPC_LIBRARY} ${VXL_LIBRARY} ${QTSOAP_LIBRARY} ${QSCXML_LIBRARY})
mafCopyFoundationLibraries("${QXMLRPC_DIR}lib")
mafCopyFoundationLibraries("${QtSOAP_DIR}lib")
mafCopyFoundationLibraries("${QSCXML_DIR}lib")
ENDMACRO()

