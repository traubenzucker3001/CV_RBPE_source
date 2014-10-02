#
# Try to find CVK library and include path.
# Once done this will define
#
# CVK_FOUND
# CVK_INCLUDE_PATH
#

FIND_PATH( CVK_INCLUDE_PATH CVK_Framework.h
	${DEPENDENCIES_PATH}/cvk/src/libraries/CVK_Framework
)

SET(CVK_FOUND "NO")
IF (CVK_INCLUDE_PATH)
	SET(CVK_FOUND "YES")
    message("EXTERNAL LIBRARY 'CVK' FOUND")
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'CVK' NOT FOUND")

ENDIF (CVK_INCLUDE_PATH)