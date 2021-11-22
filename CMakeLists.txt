cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")


project(rmkernel C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wall -specs=\"nosys.specs\"")

set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_C_FLAGS_DEBUG "-Og -g -gdwarf-3 -gstrict-dwarf")

set(OS_SOURCES
    src/os.c
    src/os_mem.c
    src/os_msg.c
    src/os_util.c
)


set(OS_HEADERS
   inc/os.h
   inc/os_defs.h
   inc/os_mem.h
   inc/os_msg.h
   inc/os_util.h
)

add_library(${PROJECT_NAME} STATIC ${OS_SOURCES} ${OS_HEADERS})

target_include_directories(${PROJECT_NAME}
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)


