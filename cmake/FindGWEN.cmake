SET(GWEN_INCLUDE_DIR thirdparty/gwen/include)

FIND_LIBRARY(GWEN_LIBRARIES gwen
	HINTS
	${GWEN}
	thirdparty/gwen/build
)
