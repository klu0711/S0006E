#--------------------------------------------------------------------------
# projects project
#--------------------------------------------------------------------------

PROJECT(projects)
FILE(GLOB projects_headers code/*.h)
FILE(GLOB projects_sources code/*.cc)

SET(files_projects ${projects_headers} ${projects_sources})
SOURCE_GROUP("projects" FILES ${files_projects})

ADD_EXECUTABLE(projects ${files_projects})
TARGET_LINK_LIBRARIES(projects core render)
ADD_DEPENDENCIES(projects core render)