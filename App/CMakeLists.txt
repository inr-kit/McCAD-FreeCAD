include_directories(
    ${Boost_INCLUDE_DIRS}
    ${OCC_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIR}
    ${XercesC_INCLUDE_DIRS}
)
link_directories(${OCC_LIBRARY_DIR})

set(McCad_LIBS
    Part
    ${OCC_LIBRARIES}
    ${OCC_DEBUG_LIBRARIES}
    FreeCADApp
)

SET(McCadPython_Scripts
    Python_SRC/CreateBox.py
    Python_SRC/SelSurf.py
    Python_SRC/LostParticle.py    
    Python_SRC/LostParticlePanel.ui
    Python_SRC/MergeFiles.py
    Python_SRC/MergeFilesPanel.ui
    Python_SRC/CheckSurf.py
    Python_SRC/CheckSurfPanel.ui
    Python_SRC/Visibility.py
    Python_SRC/Paths.py
)


SET(McCad_SRCS
    AppMcCad.cpp
    PreCompiled.cpp
    PreCompiled.h
    CreateBox.h
    CreateBox.cpp    
    Material/Material.cpp
    Material/Material.h
    Material/MaterialManager.cpp
    Material/MaterialManager.h
    McCadData.h
    McCadData.cpp
    McCadDataManager.h
    McCadDataManager.cpp
    McCadDocObserver.h
    McCadDocObserver.cpp
    McCadConversion.h
    McCadConversion.cpp
    McCadMessager_Signal.h
    McCadMessager_Signal.cpp
    Convertor/IGeomFace.cxx
    Convertor/IGeomFace.hxx
    Convertor/IMcCadWriter.cxx
    Convertor/IMcCadWriter.hxx
    Convertor/McCadExtFace.cxx
    Convertor/McCadExtFace.hxx
    Convertor/McCadExtAstFace.hxx
    Convertor/McCadExtAstFace.cxx
    Convertor/McCadExtBndFace.cxx
    Convertor/McCadExtBndFace.hxx
    Convertor/McCadAddAstSurface.cxx
    Convertor/McCadAddAstSurface.hxx
    Convertor/McCadConvexSolid.cxx
    Convertor/McCadConvexSolid.hxx
    Convertor/McCadGeomCone.cxx
    Convertor/McCadGeomCone.hxx
    Convertor/McCadGeomCylinder.cxx
    Convertor/McCadGeomCylinder.hxx
    Convertor/McCadGeomData.cxx
    Convertor/McCadGeomData.hxx
    Convertor/McCadGeomPlane.cxx
    Convertor/McCadGeomPlane.hxx
    Convertor/McCadGeomRevolution.cxx
    Convertor/McCadGeomRevolution.hxx
    Convertor/McCadGeomSphere.cxx
    Convertor/McCadGeomSphere.hxx
    Convertor/McCadGeomTorus.cxx
    Convertor/McCadGeomTorus.hxx
    Convertor/McCadMcnpWriter.cxx
    Convertor/McCadMcnpWriter.hxx
    Convertor/McCadSolid.cxx
    Convertor/McCadSolid.hxx
    Convertor/McCadStitchSurfaces.cxx
    Convertor/McCadStitchSurfaces.hxx
    Convertor/McCadTransfCard.cxx
    Convertor/McCadTransfCard.hxx
    Convertor/McCadTripoliCell.cxx
    Convertor/McCadTripoliCell.hxx
    Convertor/McCadTripoliWriter.cxx
    Convertor/McCadTripoliWriter.hxx
    Convertor/McCadVoidCell.cxx
    Convertor/McCadVoidCell.hxx
    Convertor/McCadVoidCellManager.cxx
    Convertor/McCadVoidCellManager.hxx
    Convertor/McCadVoidCollision.cxx
    Convertor/McCadVoidCollision.hxx
    Convertor/McCadVoidGenerator.cxx
    Convertor/McCadVoidGenerator.hxx
    Tools/McCadMathTool.hxx
    Tools/McCadMathTool.cxx
    Tools/McCadConvertConfig.hxx
    Tools/McCadConvertConfig.cxx
    Tools/McCadEvaluator.hxx
    Tools/McCadEvaluator.cxx
    Tools/McCadInputModelData.hxx
    Tools/McCadInputModelData.cxx
    Tools/McCadGeneTool.hxx
    Tools/McCadGeneTool.cxx
    Tools/McCadGeomTool.hxx
    Tools/McCadGeomTool.cxx
    McCadSurfaceCard.h
    McCadSurfaceCard.cpp
    ${McCadPython_Scripts}
)

add_library(McCad SHARED ${McCad_SRCS})
target_link_libraries(McCad ${McCad_LIBS})

fc_target_copy_resource(McCad
   ${CMAKE_SOURCE_DIR}/src/Mod/McCad
   ${CMAKE_BINARY_DIR}/Mod/McCad
   Init.py)

ADD_CUSTOM_TARGET(McCadPython ALL SOURCES ${McCadPython_Scripts})
#fc_copy_sources(McCadPython "${CMAKE_BINARY_DIR}/Mod/McCad" ${McCadPython_Scripts})

add_custom_command(TARGET McCadPython POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/src/Mod/McCad/App/Python_SRC
                   ${CMAKE_BINARY_DIR}/Mod/McCad)

#fc_target_copy_resource(McCad
#    ${CMAKE_SOURCE_DIR}/src/Mod/McCad/App
#    ${CMAKE_BINARY_DIR}/Mod/McCad
#    ${McCadPython_Scripts})

SET_BIN_DIR(McCad McCad /Mod/McCad)
SET_PYTHON_PREFIX_SUFFIX(McCad)

install(TARGETS McCad DESTINATION ${CMAKE_INSTALL_LIBDIR})
