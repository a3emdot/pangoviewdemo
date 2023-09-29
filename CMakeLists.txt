cmake_minimum_required(VERSION 3.7)
project(pangotest)

enable_testing()

## glib
find_library(GLIB2_LIBRARY glib-2.0)
get_filename_component(GLIB_LIBRARY_DIRS "${GLIB2_LIBRARY}" DIRECTORY)
find_path(GLIB_INCLUDE_DIRS "glib-object.h" PATH_SUFFIXES "glib-2.0")
find_path(GLIB_CONFIG_INCLUDE_DIRS "glibconfig.h"
    HINTS
      ${GLIB_LIBRARY_DIRS}
    PATHS
      "${CMAKE_LIBRARY_PATH}"
    PATH_SUFFIXES
      "glib-2.0/include"
      "glib-2.0"
) 
message(STATUS GLIB_INCLUDE_DIRS=${GLIB_INCLUDE_DIRS})
message(STATUS GLIB_CONFIG_INCLUDE_DIRS=${GLIB_CONFIG_INCLUDE_DIRS})
list(APPEND GLIB_INCLUDE_DIRS ${GLIB_CONFIG_INCLUDE_DIRS})
message(STATUS GLIB_INCLUDE_DIRS=${GLIB_INCLUDE_DIRS})

find_library(GOBJECT_LIBRARY "gobject-2.0")
message(STATUS GOBJECT_LIBRARY=${GOBJECT_LIBRARY})

find_library(HARFBUZZ_LIBRARY "harfbuzz")
message(STATUS HARFBUZZ_LIBRARY=${HARFBUZZ_LIBRARY})
find_path(HARFBUZZ_INCLUDE_DIR "hb.h" PATH_SUFFIXES "harfbuzz")
message(STATUS HARFBUZZ_INCLUDE_DIR=${HARFBUZZ_INCLUDE_DIR})

find_library(CAIRO_LIBRARY "cairo")
message(STATUS CAIRO_LIBRARY=${CAIRO_LIBRARY})
find_path(CAIRO_INCLUDE_DIR "cairo.h" PATH_SUFFIXES "cairo")
message(STATUS CAIRO_INCLUDE_DIR=${CAIRO_INCLUDE_DIR})

find_library(FREETYPE_LIBRARY "freetype")
message(STATUS FREETYPE_LIBRARY=${FREETYPE_LIBRARY})
find_path(FREETYPE_INCLUDE_DIR "ft2build.h" PATH_SUFFIXES "freetype" "freetype2")
message(STATUS FREETYPE_INCLUDE_DIR=${FREETYPE_INCLUDE_DIR})

find_library(FONTCONFIG_LIBRARY "fontconfig")
message(STATUS FONTCONFIG_LIBRARY=${FONTCONFIG_LIBRARY})
find_path(FONTCONFIG_INCLUDE_DIR "fontconfig/fontconfig.h")
message(STATUS FONTCONFIG_INCLUDE_DIR=${FONTCONFIG_INCLUDE_DIR})

find_library(PANGO_LIBRARY "pango-1.0")
message(STATUS PANGO_LIBRARY=${PANGO_LIBRARY})
find_path(PANGO_INCLUDE_DIR "pango/pango.h" PATH_SUFFIXES "pango-1.0")
message(STATUS PANGO_INCLUDE_DIR=${PANGO_INCLUDE_DIR})

find_library(PANGOFT2_LIBRARY "pangoft2-1.0")
message(STATUS PANGOFT2_LIBRARY=${PANGOFT2_LIBRARY})
find_path(PANGOFT2_INCLUDE_DIR "pango/pangoft2.h" PATH_SUFFIXES "pango-1.0")
message(STATUS PANGOFT2_INCLUDE_DIR=${PANGOFT2_INCLUDE_DIR})

find_library(PANGOCAIRO_LIBRARY "pangocairo-1.0")
message(STATUS PANGOCAIRO_LIBRARY=${PANGOCAIRO_LIBRARY})
find_path(PANGOCAIRO_INCLUDE_DIR "pango/pangocairo.h" PATH_SUFFIXES "pango-1.0")
message(STATUS PANGOCAIRO_INCLUDE_DIR=${PANGOCAIRO_INCLUDE_DIR})

add_executable(pangotest pangotest.cpp)

target_link_libraries(
  pangotest
  ${PANGO_LIBRARY}
  ${PANGOFT2_LIBRARY}
  ${PANGOCAIRO_LIBRARY}
  ${GLIB2_LIBRARY}
  ${HARFBUZZ_LIBRARY}
  ${GOBJECT_LIBRARY}
  ${CAIRO_LIBRARY}
  ${FREETYPE_LIBRARY}
  ${FONTCONFIG_LIBRARY}
)

target_include_directories(
  pangotest
  PRIVATE
  ${PANGO_INCLUDE_DIR}
  ${PANGOFT2_INCLUDE_DIR}
  ${PANGOCAIRO_INCLUDE_DIR}
  ${GLIB_INCLUDE_DIRS}
  ${HARFBUZZ_INCLUDE_DIR}
  ${CAIRO_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIR}
  ${FONTCONFIG_INCLUDE_DIR}
)

foreach(i RANGE 0 9 1)
  configure_file(dummy.txt ${i}/dummy.txt COPYONLY)
  message(STATUS ${pangotest_BINARY_DIR}/${i})
  
  foreach(j RANGE 0 9 1)
    add_test(
      NAME
        ${TestPrefix}pangotest-${i}-${j}
      COMMAND
        pangotest ${j} 0 ${pangotest_BINARY_DIR}/${i}
    )
  endforeach()
endforeach()

