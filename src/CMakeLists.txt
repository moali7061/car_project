add_executable(${ProjectName}
    main.c
    infrared.c
    ultrasonic_sensor.c
    motorTest.c
    hc05.c
    pwm_led.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4
    hardware_spi
    hardware_i2c
    pico_binary_info
    hardware_pwm
    hardware_uart 
    )


# Enable USB and UART for debugging
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

pico_add_extra_outputs(${ProjectName})