function(avnd_make_vintage)
  cmake_parse_arguments(AVND "" "TARGET;MAIN_FILE;MAIN_CLASS;C_NAME" "" ${ARGN})
  set(AVND_FX_TARGET "${AVND_TARGET}_vintage")
  add_library(${AVND_FX_TARGET} SHARED)

  configure_file(
    include/avnd/binding/vintage/prototype.cpp.in
    "${CMAKE_BINARY_DIR}/${AVND_C_NAME}_vintage.cpp"
    @ONLY
    NEWLINE_STYLE LF
  )

  target_sources(
    ${AVND_FX_TARGET}
    PRIVATE
      "${CMAKE_BINARY_DIR}/${AVND_C_NAME}_vintage.cpp"
  )

  set_target_properties(
    ${AVND_FX_TARGET}
    PROPERTIES
      OUTPUT_NAME "${AVND_C_NAME}.vintage"
      LIBRARY_OUTPUT_DIRECTORY vintage
      RUNTIME_OUTPUT_DIRECTORY vintage
  )

  target_link_libraries(
    ${AVND_FX_TARGET}
    PRIVATE
      Avendish::Avendish_vintage
      DisableExceptions
  )

  avnd_common_setup("${AVND_TARGET}" "${AVND_FX_TARGET}")
endfunction()

add_library(Avendish_vintage INTERFACE)
target_link_libraries(Avendish_vintage INTERFACE Avendish)
add_library(Avendish::Avendish_vintage ALIAS Avendish_vintage)

target_sources(Avendish PRIVATE
  include/avnd/binding/vintage/audio_effect.hpp
  include/avnd/binding/vintage/atomic_controls.hpp
  include/avnd/binding/vintage/configure.hpp
  include/avnd/binding/vintage/dispatch.hpp
  include/avnd/binding/vintage/helpers.hpp
  include/avnd/binding/vintage/midi_processor.hpp
  include/avnd/binding/vintage/processor_setup.hpp
  include/avnd/binding/vintage/programs.hpp
  include/avnd/binding/vintage/vintage.hpp
)
