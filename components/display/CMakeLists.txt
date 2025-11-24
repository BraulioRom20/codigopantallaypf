idf_component_register(
    SRCS "windows.cpp"
    INCLUDE_DIRS "."
    REQUIRES Decoder driver arduino TFT_eSPI
)