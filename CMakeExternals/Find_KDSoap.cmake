# MAF
# KDSoap
# Kewei Duan 31/03/2014
  
  SET(KDSoap_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/KDSoap")
  #FIND_PACKAGE(KDSoap REQUIRED)
  #additional include directory
  INCLUDE_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Build/KDSoap/include/" "${MAF_EXTERNAL_BUILD_DIR}/Build/KDSoap/include/KDSoapClient/" "${MAF_EXTERNAL_BUILD_DIR}/Build/KDSoap/include/KDSoapServer/")
  
  INCLUDE(${QT_USE_FILE})
  ADD_DEFINITIONS(${QT_DEFINITIONS})
  
  SET(PROJECT_LIBS ${PROJECT_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} kdsoap kdsoap-server)
  SET(KDSoap_LIBRARY_DIRS "${MAF_EXTERNAL_BUILD_DIR}/Build/KDSoap/include/lib")