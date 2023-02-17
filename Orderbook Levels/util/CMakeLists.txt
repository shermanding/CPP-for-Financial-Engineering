set(util_files
    incremental_csv_reader.cpp
)

add_library(util ${util_files})

target_include_directories(util PUBLIC
    ${PROJECT_SOURCE_DIR}
)