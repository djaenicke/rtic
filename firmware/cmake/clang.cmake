set(ALL_FILES ${APP_SRC})

# Glob together a list of the include files
foreach(DIR IN ITEMS ${APP_INCLUDES})
    FILE(GLOB INC_FILES ${DIR}/*.h)
    set(ALL_FILES ${ALL_FILES} ${INC_FILES})
endforeach()

set(FORMAT_CMD "clang-format" "${ALL_FILES}" "-i" "-style=file")
