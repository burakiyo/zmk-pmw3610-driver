zephyr_library()

zephyr_library_sources_ifdef(CONFIG_PMW3610 src/pmw3610.c)
zephyr_library_sources_ifdef(CONFIG_PMW3610_CPI_BEHAVIOR src/behaviors/pmw3610_cpi_behavior.c)
zephyr_include_directories(${APPLICATION_SOURCE_DIR}/include)
