add_executable(ArcranionTest
    "src/basic_application.cpp"
)

target_link_libraries(ArcranionTest
    PRIVATE ArcranionMain
)