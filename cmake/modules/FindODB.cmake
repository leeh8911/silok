# Try to find the ODB compiler and libraries

find_program(ODB_COMPILER_EXECUTABLE
    NAMES odb
    HINTS ENV ODB_ROOT
    PATH_SUFFIXES bin
)

find_path(ODB_INCLUDE_DIR
    NAMES odb/version.hxx
    HINTS ENV ODB_ROOT
    PATH_SUFFIXES include
)

find_library(ODB_LIBRARY
    NAMES odb odbD odb.dll libodb odb-static
    HINTS ENV ODB_ROOT
    PATH_SUFFIXES lib
)

find_library(ODB_SQLITE_LIBRARY
    NAMES odb-sqlite odb-sqliteD odb-sqlite.dll libodb-sqlite
    HINTS ENV ODB_ROOT
    PATH_SUFFIXES lib
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ODB
    DEFAULT_MSG
    ODB_COMPILER_EXECUTABLE
    ODB_INCLUDE_DIR
    ODB_LIBRARY
    ODB_SQLITE_LIBRARY
)

mark_as_advanced(ODB_COMPILER_EXECUTABLE ODB_INCLUDE_DIR ODB_LIBRARY ODB_SQLITE_LIBRARY)
