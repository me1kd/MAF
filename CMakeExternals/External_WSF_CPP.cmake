# MAF
# WSF_CPP
# Kewei Duan 01/04/2014

SET(proj WSF_CPP)

  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  #SET(ep_build_shared_libs ON)
  #SET(ep_build_testing OFF)
                               
  IF(NOT DEFINED WSF_CPP_DIR)
    #MESSAGE("Adding project:${proj}")
    
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "https://bitbucket.org/me1kd/wso2-wsf-cpp-src-2.1.0.git"
      CONFIGURE_COMMAND ${ep_source_dir}/${proj}/configure --prefix=${ep_build_dir}/${proj}
      BUILD_IN_SOURCE 1
      BUILD_COMMAND make
      )
    SET(KDSoap_DIR ${ep_build_dir}/${proj})
   
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

#ENDIF(RESULT)

  SET(${WSF_CPP_enabling_variable}_INCLUDE_DIRS WSF_CPP_INCLUDE_DIRS)
  SET(${WSF_CPP_enabling_variable}_FIND_PACKAGE_CMD WSF_CPP)