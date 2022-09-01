구현된 클래스들

1. Viewer

  1) Main Viewer 
    - MPR View

  2) CustomSliceWidget 
    - MPR View에 포함된 2D SliceViewer
    - QmitkSliceWidget 하단에 Reslice 옵션 기능 추가됨.

  3) PlaneInteractor
    - 2D & 3D Viewer에서 Plane 이동 시킬 수 있는 Interactor
    - Ctrl + MouseLButton + move

  4) ImageProcessingWidget
    - Brightness, Contrast, LevelWindow, ImageFilter 옵션 UI

  5) ImageProcessingParamWidget
    - ImageProcessingWidget의 ImageFilter Parameter값 조절 가능한 UI


2. ImageProcessing (구현된 Filter : SharpenFilter, MedianBlur, GaussianBlur, BilateralFilter)

  1) ImageProcessing
    - ImageFilter 실행을 위한 클래스

  2) ImageProcessingPiplineObject
    - mitk Filter구조를 가진 클래스
    - 내부에서 ImageProcessing->runFilter 호출함.

  3) CustomImageMapper2D
    - 2D Viewer에 Filter적용을 위한 Mapper 클래스
    - 이 클래스를 생성하여 2D Mapper에 넣어주면, Filter가 동작함.
   
  4) ImageProcessingImpl
    - ImageFilter의 내부 로직 구현을 위한 클래스 모음
    - IMPL_PROCESSING_RUN_MACRO 매크로를 사용해 PixelType별로 Template변경하여 run함수 호출시킴.

  5) ImageProcessingKernel
    - Processing시 필요한 Kernel 클래스


3. Utility

  1) Util::Image
    - Image Load, Save 함수 구현됨.
    - 좌표변환에 도움되는 함수 구현됨.

  2)  Util::Stopwatch
    - 시간측정시 도움되는 클래스







=========================
The MITK Project Template
=========================

This project provides a complete CMake-based set-up to get started with [MITK](https://github.com/MITK/MITK).

Features
--------

- Example module
  - ITK-based image filter
  - Interactor to paint in images
- Example command-line app
  - Uses the image filter of the example module
- Example plugin
  - GUI for the image filter and interactor of the example module
- Example external project
  - Microsoft's Guidelines Support Library (GSL)

How it works
------------

1. Clone [MITK](https://github.com/MITK/MITK) and checkout the latest release tag or at least the stable master branch
2. Click on "Use this template", or clone/fork the MITK-ProjectTemplate, checking out the matching tag or branch
3. Configure the MITK superbuild and set the CMake cache variable `MITK_EXTENSION_DIRS` to your working copy of the project template
4. Generate and build the MITK superbuild

The project template is virtually integrated right into the MITK superbuild and MITK build as if it would be part of MITK. You can extend MITK with your own modules, plugins, command-line apps, and external projects without touching the MITK source code resp. repository.

Supported platforms and other requirements
------------------------------------------

See the [MITK documentation](http://docs.mitk.org/2021.10/).

License
-------

Copyright (c) [German Cancer Research Center (DKFZ)](https://www.dkfz.de)<br>
All rights reserved.

The MITK-ProjectTemplate is part of [MITK](https://github.com/MITK/MITK) and as such available as free open-source software under a [3-clause BSD license](https://github.com/MITK/MITK-ProjectTemplate/blob/master/LICENSE).
