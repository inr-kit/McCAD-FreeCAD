
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${COIN3D_INCLUDE_DIRS}
    ${OCC_INCLUDE_DIR}
    ${QT_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${XercesC_INCLUDE_DIRS}
)


set(McCadGui_LIBS
    McCad
    FreeCADGui
)

set(McCadGui_MOC_HDRS
    MatDlg.h
    WorkingSpaceDlg.h
    ConfigDlg.h    
    McCadSettingDlg.h
    McCadConvertorDlg.h
    McCadLostParticle.h
    McCadExporter.h
)

fc_wrap_cpp(McCadGui_MOC_SRCS ${McCadGui_MOC_HDRS})
SOURCE_GROUP("Moc" FILES ${McCadGui_MOC_HDRS})

qt4_add_resources(McCad_QRC_SRCS Resources/McCad.qrc)

SET(McCadGui_SRCS
    ${McCad_QRC_SRCS}
    AppMcCadGui.cpp
    Command.cpp
    McCadUtils.cpp
    McCadUtils.h
    MatDlg.h
    MatDlg.cpp
    WorkingSpaceDlg.h
    WorkingSpaceDlg.cpp
    ConfigDlg.h
    ConfigDlg.cpp
    PreCompiled.cpp
    PreCompiled.h
    Workbench.cpp
    Workbench.h  
    McCadSettingDlg.cpp
    McCadSettingDlg.h
    McCadComboDelegate.h
    McCadComboDelegate.cpp
    McCadConvertorDlg.h
    McCadConvertorDlg.cpp
    McCadLostParticle.h
    McCadLostParticle.cpp
    McCadExporter.h
    McCadExporter.cpp
)

add_library(McCadGui SHARED ${McCadGui_SRCS})
target_link_libraries(McCadGui ${McCadGui_LIBS})

fc_target_copy_resource(McCadGui 
    ${CMAKE_SOURCE_DIR}/src/Mod/McCad
    ${CMAKE_BINARY_DIR}/Mod/McCad
    InitGui.py)

SET_BIN_DIR(McCadGui McCadGui /Mod/McCad)
SET_PYTHON_PREFIX_SUFFIX(McCadGui)
    
install(TARGETS McCadGui DESTINATION ${CMAKE_INSTALL_LIBDIR})
