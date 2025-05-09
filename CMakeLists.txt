# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(ohmimetro-code-resistores C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ohmimetro-code-resistores ohmimetro-code-resistores.c inc/ssd1306/ssd1306.c)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
        PICO_PRINTF_SUPPORT_FLOAT=1 
        PICO_STDIO_ENABLE_PRINTF=1
    )

pico_set_program_name(ohmimetro-code-resistores "ohmimetro-code-resistores")
pico_set_program_version(ohmimetro-code-resistores "0.1")

# Generate PIO header
pico_generate_pio_header(ohmimetro-code-resistores ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(ohmimetro-code-resistores 0)
pico_enable_stdio_usb(ohmimetro-code-resistores 1)

# Add the standard library to the build
target_link_libraries(ohmimetro-code-resistores
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(ohmimetro-code-resistores PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(ohmimetro-code-resistores 
        hardware_i2c
        hardware_adc
        hardware_pio
        hardware_clocks
        )

pico_add_extra_outputs(ohmimetro-code-resistores)

