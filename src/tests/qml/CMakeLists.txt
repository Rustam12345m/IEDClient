set(QML_TEST_TARGET_NAME qml_tests)

set(QML_TEST_SOURCES
    main.cpp
)

# Qt's QUICK_TEST_MAIN directory scan only picks up tst_*.qml.
# We want Test_* — so list each file here and pass it with -input.
set(QML_TEST_FILES
    Test_Smoke.qml
    Test_DiaControlOperate.qml
    Test_DiaChangeValue.qml
    Test_SGCBPanel.qml
    Test_LN_SettingsView.qml
    Test_LD_SettingsContent.qml
    Test_HomePage.qml
    Test_DatasetPage.qml
    Test_ReportsPage.qml
    Test_IedTreePage.qml
    Test_FilesystemPage.qml
)

# Production UI resources: gives tests access to qrc:/logical_node/…,
# qrc:/common/…, qrc:/singletons/… exactly as the app sees them.
qt6_add_resources(QML_TEST_UI_QRC ${CMAKE_SOURCE_DIR}/ui/resources.qrc)

# QML mocks injected as context properties by TestSetup (main.cpp).
qt6_add_resources(QML_TEST_MOCKS_QRC ${CMAKE_CURRENT_SOURCE_DIR}/mocks.qrc)

qt_add_executable(${QML_TEST_TARGET_NAME}
    ${QML_TEST_SOURCES}
    ${QML_TEST_UI_QRC}
    ${QML_TEST_MOCKS_QRC}
)

target_link_libraries(${QML_TEST_TARGET_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::QuickTest
    Qt6::Test
)

# Build -input arg list so the test binary loads our Test_*.qml files
# explicitly instead of relying on the tst_*.qml glob.
set(QML_TEST_INPUT_ARGS)
foreach(qml_file IN LISTS QML_TEST_FILES)
    list(APPEND QML_TEST_INPUT_ARGS
        -input "${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}")
endforeach()

# CTest
add_test(NAME ${QML_TEST_TARGET_NAME}
         COMMAND ${QML_TEST_TARGET_NAME} ${QML_TEST_INPUT_ARGS})
set_tests_properties(${QML_TEST_TARGET_NAME} PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
)
