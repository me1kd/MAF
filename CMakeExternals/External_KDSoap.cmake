# MAF
# KDSoap
# Kewei Duan 31/03/2014

SET(proj KDSoap)

  SET(ep_base "${MAF_EXTERNAL_BUILD_DIR}")
  SET_PROPERTY(DIRECTORY PROPERTY EP_BASE ${ep_base})
  
  SET(ep_install_dir ${ep_base}/Install)
  SET(ep_build_dir ${ep_base}/Build)
  SET(ep_source_dir ${ep_base}/Source)
  #SET(ep_parallelism_level)
  #SET(ep_build_shared_libs ON)
  #SET(ep_build_testing OFF)
                               
  IF(NOT DEFINED KDSoap_DIR)
    #MESSAGE("Adding project:${proj}")
    
    ExternalProject_Add(${proj}
      GIT_REPOSITORY "https://github.com/KDAB/KDSoap.git"
      UPDATE_COMMAND ${ep_source_dir}/${proj}/autogen.py
      CONFIGURE_COMMAND ${ep_source_dir}/${proj}/configure.sh -shared -release -prefix ${ep_build_dir}/${proj}
      BUILD_IN_SOURCE 1
      BUILD_COMMAND make
      )
    SET(KDSoap_DIR ${ep_build_dir}/${proj})
   
  
  ELSE()
    include(CMake/mafMacroEmptyExternalProject.cmake)
    mafMacroEmptyExternalProject(${proj} "${proj_DEPENDENCIES}")
  ENDIF()

#ENDIF(RESULT)

  SET(${KDSoap_enabling_variable}_INCLUDE_DIRS KDSoap_INCLUDE_DIRS)
  SET(${KDSoap_enabling_variable}_FIND_PACKAGE_CMD KDSoap)