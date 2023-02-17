add_library(book
    l3_book.cpp
)

target_include_directories(book PUBLIC
    ${PROJECT_SOURCE_DIR}
)
