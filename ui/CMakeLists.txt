qt_add_qml_module(
    ${PROJECT}_UI
    URI Application
    VERSION 1.0
    # STATIC
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/
    QML_FILES main.qml
              common/AppStatusBar.qml
              common/CustomTabBar.qml
              common/Led.qml
              common/SortTableHeader.qml
              common/SplitDelimeter.qml
              common/TableHeader.qml
              common/TextDelegate.qml
              common/ToolBarButton.qml
)
