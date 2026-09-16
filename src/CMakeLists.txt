add_library (cs4212-util
  ArgumentParsing.cpp ArgumentParsing.h
  handleGraphicsArgs.cpp handleGraphicsArgs.h
  model_obj.cpp model_obj.h
  vec3.h color.h
  Framebuffer.cpp Framebuffer.h
)

target_compile_definitions(cs4212-util PUBLIC HAS_GLM)

target_link_libraries(cs4212-util PRIVATE Boost::program_options)

target_link_libraries(cs4212-util PUBLIC glm::glm)

add_executable(mainCode
  mainCode.cpp
)

target_link_libraries(mainCode PRIVATE cs4212-util)

target_link_libraries(mainCode PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARIES})

target_link_libraries(mainCode PRIVATE PNG::PNG)

target_link_libraries(mainCode PRIVATE ZLIB::ZLIB)
