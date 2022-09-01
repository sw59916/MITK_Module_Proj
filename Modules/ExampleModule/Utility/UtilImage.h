#pragma once
#include "MitkExampleModuleExports.h"
#include <mitkImage.h>
#include <string>

namespace Util
{
	namespace Image
	{
		void checkSameImageFormat(mitk::Image::Pointer lhsImage, mitk::Image::Pointer rhsImage);

		struct MITKEXAMPLEMODULE_EXPORT RawImageInfo
		{
			RawImageInfo() : dimension(2), pixelType(mitk::PixelType::ItkIOComponentType::UNKNOWNCOMPONENTTYPE), sizeX(0), sizeY(0), sizeZ(0)
			{
			}
			int dimension;
			mitk::PixelType::ItkIOComponentType pixelType;
			int sizeX;
			int sizeY;
			int sizeZ;
		};

		MITKEXAMPLEMODULE_EXPORT mitk::Image::Pointer Load(std::string filePath, RawImageInfo imageInfo = RawImageInfo());
		MITKEXAMPLEMODULE_EXPORT void Save(mitk::Image::Pointer image, std::string filePath);


		template<int Dimension>
		inline int coordinateToBufferOffset(itk::Index<Dimension>& coordinate, itk::Size<Dimension>& dimSize)
		{
			int bufferOffset = 0;
			switch (Dimension)
			{
			case 3:
				bufferOffset += coordinate[2] * dimSize[0] * dimSize[1];
			case 2:
				bufferOffset += coordinate[0] + coordinate[1] * dimSize[0];
			}
			return bufferOffset;
		}
		template<int Dimension>
		inline itk::Index<Dimension> bufferOffsetToCoordinate(int bufferOffset, itk::Size<Dimension>& dimSize)
		{
			itk::Index<Dimension> coordinate;

			for (int dim = 0; dim < Dimension; dim++)
			{
				int curDimStepSize = 0;
				int nextDimStepSize = 0;
				for (int dim2 = 0; dim2 < dim; dim2++)
					curDimStepSize += dimSize[dim2];
				for (int dim2 = 0; dim2 < dim + 1; dim2++)
					nextDimStepSize += dimSize[dim2];

				int tempCoordinate = curDimStepSize == 0 ? bufferOffset : bufferOffset / curDimStepSize;
				tempCoordinate = tempCoordinate % nextDimStepSize;
				coordinate[dim] = tempCoordinate;
			}

			return coordinate;
		}

	}
}