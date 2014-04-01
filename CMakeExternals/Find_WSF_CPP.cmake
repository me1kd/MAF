# MAF
# WSF_CPP
# Kewei Duan 31/04/2014
  
  SET(WSF_CPP_DIR "${MAF_EXTERNAL_BUILD_DIR}/Build/WSF_CPP")
  INCLUDE_DIRECTORIES("${MAF_EXTERNAL_BUILD_DIR}/Build/WSF_CPP/include/" "${MAF_EXTERNAL_BUILD_DIR}/Build/WSF_CPP/include/axis2-1.6.0/" "${MAF_EXTERNAL_BUILD_DIR}/Build/WSF_CPP/include/axis2-1.6.0/platforms/")
    
  SET(PROJECT_LIBS ${PROJECT_LIBS} axutil axis2_axiom axis2_parser axis2_engine wso2_wsf pthread axis2_http_sender axis2_http_receiver)
  SET(WSF_CPP_LIBRARY_DIRS "${MAF_EXTERNAL_BUILD_DIR}/Build/WSF_CPP/lib")