set(CPP_FILES
  Viewer/MainViewer.cpp
  Viewer/CustomSliceWidget.cpp
  Viewer/PlaneInteractor.cpp
  Viewer/ImageProcessingWidget.cpp
  Viewer/ImageProcessingParamWidget.cpp
  ImageProcessing/ImageProcessing.cpp
  ImageProcessing/ImageProcessingImpl.cpp
  ImageProcessing/ImageProcessingKernel.cpp
  Utility/UtilImage.cpp
  Utility/UtilStopwatch.cpp
)

set(UI_FILES
)

set(MOC_H_FILES
  Viewer/CustomSliceWidget.h
  Viewer/ImageProcessingWidget.h
  Viewer/ImageProcessingParamWidget.h
)

set(RESOURCE_FILES
  Interactions/PlaneMove.xml
  Interactions/PlaneMoveConfig.xml
)
