file(GLOB srcs [A-Z|a-z]*.cpp [A-Z|a-z]*.c [A-Z|a-z]*.f)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}  ${TRUST_ROOT}/include/backward  ${TRUST_ROOT}/src/Kernel/Framework  ${TRUST_ROOT}/src/Kernel/Geometrie  ${TRUST_ROOT}/src/Kernel/Math/Matrices  ${TRUST_ROOT}/src/Kernel/Math/SolvSys  ${TRUST_ROOT}/src/Kernel/Math  ${TRUST_ROOT}/src/Kernel/Operateurs  ${TRUST_ROOT}/src/Kernel/Schemas_Temps  ${TRUST_ROOT}/src/Kernel/Utilitaires  ${TRUST_ROOT}/src/Kernel/VF/Zones  ${TRUST_ROOT}/src/ThHyd  ${TRUST_ROOT}/src/ThSol )
add_library(obj_PF OBJECT  ${srcs} )
set(listlibs ${listlibs} PF PARENT_SCOPE    )
add_custom_target(check_sources_PF   COMMAND check_sources.sh ${CMAKE_CURRENT_SOURCE_DIR} ) #COMMENT  "checking code validity PF" )
add_DEPENDENCIES(obj_PF check_sources_PF)
if(COMPIL_DYN)
  add_library(PF SHARED   $<TARGET_OBJECTS:obj_PF> )
  install(TARGETS PF  DESTINATION lib)
endif(COMPIL_DYN)
