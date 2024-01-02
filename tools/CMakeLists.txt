if(TARGET Qt::Svg)
    add_subdirectory(svg2qvg)
endif()

if(QT_VERSION VERSION_GREATER_EQUAL 6.6.0)
    add_subdirectory(qsb2hpp)
endif()