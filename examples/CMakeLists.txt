set(examples
    boxes
    shapes
    dummy
    shapes2
    invaders
)

foreach(example ${examples})
    add_executable(${example} ${example}.cpp)
    target_link_libraries(${example} ${SFML_LIBRARIES} ${Boost_LIBRARIES})
endforeach()
