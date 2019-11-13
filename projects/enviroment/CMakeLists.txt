#--------------------------------------------------------------------------
# assignment-5 project
#--------------------------------------------------------------------------

PROJECT(enviroment)
FILE(GLOB assignment-5_headers code/*.h)
FILE(GLOB assignment-5_sources code/*.cc)
FILE(GLOB res code/res/*.*)

SET(files_assignment-5 ${assignment-5_headers} ${assignment-5_sources})
SOURCE_GROUP("enviroment" FILES ${files_assignment-5})

foreach(FILE ${res})
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(FILE)

ADD_EXECUTABLE(enviroment ${files_assignment-5})
TARGET_LINK_LIBRARIES(enviroment core render)
ADD_DEPENDENCIES(enviroment core render)

