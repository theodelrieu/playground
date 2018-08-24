macro(mgs_create_and_install_target _project_name)
  add_library(${_project_name} INTERFACE)

  install(TARGETS ${_project_name} EXPORT "${_project_name}-targets"
    COMPONENT ${_project_name}
    INCLUDES DESTINATION include
  )

  install(DIRECTORY include DESTINATION . COMPONENT ${_project_name})

  include(CMakePackageConfigHelpers)

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-config-version.cmake"
    # Will change to SameMajorVersion once 1.0 is released.
    COMPATIBILITY SameMinorVersion
  )

  set(ConfigPackageLocation lib/cmake/${_project_name})

  export(EXPORT "${_project_name}-targets"
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-targets.cmake"
    NAMESPACE mgs::
  )

  install(EXPORT "${_project_name}-targets"
    FILE
      "${_project_name}-targets.cmake"
    NAMESPACE
      mgs::
    DESTINATION
      ${ConfigPackageLocation}
  )

  install(
    FILES
    "cmake/${_project_name}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${_project_name}-config-version.cmake"
    DESTINATION
      ${ConfigPackageLocation}
    COMPONENT
      ${_project_name}
  )
endmacro()
