  SET(QtSOAP_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/QtSOAP")
  FIND_PACKAGE(QtSOAP REQUIRED)
  include(${QtSOAP_DIR}/QtSOAPConfig.cmake)
  INCLUDE(${QtSOAP_USE_FILE})