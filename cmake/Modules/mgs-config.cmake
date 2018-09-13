set(_supported_components
  adapters
  adapters_base
  base16
  base32
  base32hex
  base64
  base64url
  binary_to_text
  codecs_base
  exceptions
  iterators
  meta
)

set(mgs_FOUND True)
foreach(_comp ${mgs_FIND_COMPONENTS})
  set(_fullpath "${CMAKE_CURRENT_LIST_DIR}/${_comp}/${_comp}-targets.cmake")
  if (NOT ${_comp} IN_LIST _supported_components)
    set(mgs_FOUND False)
    set(mgs_NOT_FOUND_MESSAGE "Unsupported component: ${_comp}")
  elseif(NOT EXISTS ${_fullpath})
    set(mgs_FOUND False)
    set(mgs_NOT_FOUND_MESSAGE "Component not installed: ${_comp}")
  else()
    include(${_fullpath})
  endif()
endforeach()

if (mgs_FOUND)
  message(STATUS "Found mgs ${mgs_VERSION}, components:\n    ${mgs_FIND_COMPONENTS}")
endif()
