# file: Collect all files that need to be compiled. 
# You can use a GLOB function as shown here, or explicitly mention the specific files
#file(GLOB FILES *.c *.h)

set(THIS_LIBRARY_NAME SrcLib)

file(GLOB FILES
  *.c
)

# add_library: With this declaration, you express the intent to build a library. 
# The first argument is the name of the library, 
# the second argument are the files that will be compiled to create your library.
add_library(${THIS_LIBRARY_NAME} ${FILES})

# target_link_libraries: If you link with other libraries, list them here
target_link_libraries(
  ${THIS_LIBRARY_NAME} 
  PRIVATE McuLib 
  PRIVATE pico_stdlib
  PRIVATE pico_stdio_semihosting
  PRIVATE pico_stdio_usb
  PRIVATE pico_stdio_uart
  PRIVATE hardware_adc
) 

# target_include_directories: Libraries need to publish their header files 
# so that you can import them in source code. This statement expresses where to find the files 
# - typically in an include directory of your projects.
target_include_directories(
  ${THIS_LIBRARY_NAME} 
  PUBLIC
  .
)
