cmake_minimum_required(VERSION 3.16)
project(pash C)

#
# options
#
option(PASH_STATIC_PIC "Build static library with -fPIC" OFF)

#
# flags
#
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#
# build packcc
#
add_executable(packcc packcc.c)

#
# generate parser sources
#
set(PARSER_GEN_C ${CMAKE_CURRENT_BINARY_DIR}/parser.gen.c)
set(PARSER_GEN_H ${CMAKE_CURRENT_BINARY_DIR}/parser.gen.h)

add_custom_command(
	OUTPUT ${PARSER_GEN_C} ${PARSER_GEN_H}
	COMMAND
		packcc
		-o ${CMAKE_CURRENT_BINARY_DIR}/parser.gen
		${CMAKE_CURRENT_SOURCE_DIR}/parser.peg
	DEPENDS packcc parser.peg defs.peg pash_internal.h ast_internal.h
	COMMENT "Generating parser with packcc"
)

set_source_files_properties(
	${PARSER_GEN_C}
	PROPERTIES GENERATED TRUE
)

#
# core libraries
#
set(
	PASH_CORE_SOURCES
	ast.c
	pash.c
	${PARSER_GEN_C}
)

#
# static library
#
add_library(pash_static STATIC ${PASH_CORE_SOURCES})

set_target_properties(
	pash_static
	PROPERTIES
	POSITION_INDEPENDENT_CODE ${PASH_STATIC_PIC}
)

target_include_directories(
	pash_static
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR} # parser.gen.h
)

#
# shared library
#
add_library(pash_shared SHARED ${PASH_CORE_SOURCES})

set_target_properties(
	pash_shared
	PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
	pash_shared
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(pash_static PROPERTIES OUTPUT_NAME pash)
set_target_properties(pash_shared PROPERTIES OUTPUT_NAME pash)

#
# final executable
#
set(
	PRINTER_SOURCES
	printer.c
	printer-pretty.c
	printer-json.c
)

add_executable(
	pash
	main.c
	${PRINTER_SOURCES}
)

target_link_libraries(pash PRIVATE pash_static)

target_include_directories(
	pash
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
)

#
# tests
#
add_custom_target(
	tests
	COMMAND
		${CMAKE_CURRENT_SOURCE_DIR}/tests/run.sh
	DEPENDS
		pash
)
