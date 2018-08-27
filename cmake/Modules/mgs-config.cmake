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

foreach(_comp ${mgs_FIND_COMPONENTS})
  if (NOT ";${_supported_components};" MATCHES _comp)
    set(mgs_FOUND False)
    set(mgs_NOT_FOUND_MESSAGE "Unsupported component: ${_comp}")
  endif()
  include("${CMAKE_CURRENT_LIST_DIR}/${_comp}/${_comp}-targets.cmake")
endforeach()
