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

function(conan_target_to_component target output)
  string(REPLACE CONAN_PKG::mgs_ "" _out ${target})
  set(${output} ${_out} PARENT_SCOPE)
endfunction()

# TODO discard non-component deps (i.e. external libs)
function(get_component_dependencies component output)
  get_target_property(libs "mgs::${component}" INTERFACE_LINK_LIBRARIES)
  if (libs)
    set(_out)
    foreach(_conan_lib ${libs})
      conan_target_to_component(${_conan_lib} dependency_component)
      list(APPEND _out ${dependency_component})
    endforeach()
    set(${output} ${_out} PARENT_SCOPE)
  endif()
endfunction()


function(fix_interface_link_libraries component)
  get_target_property(libs "mgs::${component}" INTERFACE_LINK_LIBRARIES)

  if (libs)
    set(_new_libs)
    foreach(_conan_lib ${libs})
      conan_target_to_component(${_conan_lib} dependency_component)
      list(APPEND _new_libs "mgs::${dependency_component}")
    endforeach()
    set_target_properties("mgs::${component}" PROPERTIES INTERFACE_LINK_LIBRARIES ${_new_libs})
  endif()
endfunction()

set(mgs_FOUND True)
set(_done False)

list(LENGTH mgs_FIND_COMPONENTS _nb_components_to_find)
if (_nb_components_to_find EQUAL 0)
  set(mgs_FIND_COMPONENTS ${_supported_components})
endif()

set(_mgs_MISSING_COMPONENTS ${mgs_FIND_COMPONENTS})
set(_mgs_FOUND_COMPONENTS)

# TODO handle find_package(mgs)
while (NOT ${_done})
  list(LENGTH _mgs_MISSING_COMPONENTS _len)
  if (${_len} EQUAL 0)
    set(_done True)
  else()
    list(REMOVE_DUPLICATES _mgs_MISSING_COMPONENTS)
  endif()

  set(_current_missing_components)
  foreach(_comp ${_mgs_MISSING_COMPONENTS})
    set(_fullpath "${CMAKE_CURRENT_LIST_DIR}/${_comp}/${_comp}-targets.cmake")
    if (NOT ${_comp} IN_LIST _supported_components)
      set(mgs_FOUND False)
      set(mgs_NOT_FOUND_MESSAGE "Unsupported component: ${_comp}")
      set(_done True)
    elseif(NOT EXISTS ${_fullpath})
      set(mgs_FOUND False)
      set(mgs_NOT_FOUND_MESSAGE "Component not installed: ${_comp}")
      set(_done True)
    else()
      include(${_fullpath})

      list(APPEND _mgs_FOUND_COMPONENTS ${_comp})
      set(_deps)
      get_component_dependencies(${_comp} _deps)
      # replace every CONAN_PKG::mgs_* by mgs::* in INTERFACE_LINK_LIBRARIES
      fix_interface_link_libraries(${_comp})
      foreach(_dep ${_deps})
        if (NOT ${_dep} IN_LIST _mgs_FOUND_COMPONENTS AND NOT ${_dep} IN_LIST _mgs_MISSING_COMPONENTS)
          list(APPEND _current_missing_components ${_dep})
        endif()
      endforeach()
    endif()
  endforeach()
  set(_mgs_MISSING_COMPONENTS ${_current_missing_components})
endwhile()

if (mgs_FOUND)
  message(STATUS "Found mgs ${mgs_VERSION}, components:")
  foreach (_comp ${_mgs_FOUND_COMPONENTS})
    message(STATUS "  ${_comp}")
  endforeach()
endif()
