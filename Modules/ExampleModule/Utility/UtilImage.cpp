#include "UtilImage.h"
#include <algorithm>
#include <mitkIOUtil.h>
#include <mitkIOConstants.h>

namespace Util
{
	namespace Image
	{
		void checkSameImageFormat(mitk::Image::Pointer lhsImage, mitk::Image::Pointer rhsImage)
		{
			if (lhsImage == nullptr)
				throw std::exception("lhs Image is nullptr");
			if (rhsImage == nullptr)
				throw std::exception("rhs Image is nullptr");
			if (lhsImage->GetPixelType() != rhsImage->GetPixelType())
				throw std::exception("Images are different PixelType");
		}

		mitk::Image::Pointer Load(std::string filePath, RawImageInfo rawImageInfo)
		{
			mitk::Image::Pointer retImage = nullptr;

			size_t dotIdx = filePath.rfind('.');
			

			std::string extension;
			if (dotIdx == std::string::npos)
				cout << "Extension not Found" << endl;
			else
			{
				extension.assign(filePath.size() - dotIdx, 0);
				std::transform(filePath.begin() + dotIdx, filePath.end(), extension.begin(), std::tolower);
			}

			if (extension == ".raw")
			{
				mitk::IFileReader::Options options;
				options[mitk::IOConstants::DIMENSION()] = rawImageInfo.dimension;
				options[mitk::IOConstants::SIZE_X()] = rawImageInfo.sizeX;
				options[mitk::IOConstants::SIZE_Y()] = rawImageInfo.sizeY;
				options[mitk::IOConstants::SIZE_Z()] = rawImageInfo.sizeZ;
				options[mitk::IOConstants::ENDIANNESS()] = mitk::IOConstants::ENDIANNESS_LITTLE();
				switch (rawImageInfo.pixelType)
				{
				case mitk::PixelType::ItkIOComponentType::UCHAR: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_UCHAR(); break;
				case mitk::PixelType::ItkIOComponentType::CHAR: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_CHAR(); break;
				case mitk::PixelType::ItkIOComponentType::USHORT: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_USHORT(); break;
				case mitk::PixelType::ItkIOComponentType::SHORT: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_SHORT(); break;
				case mitk::PixelType::ItkIOComponentType::UINT: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_UINT(); break;
				case mitk::PixelType::ItkIOComponentType::INT: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_INT(); break;
				case mitk::PixelType::ItkIOComponentType::FLOAT: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_FLOAT(); break;
				case mitk::PixelType::ItkIOComponentType::DOUBLE: options[mitk::IOConstants::PIXEL_TYPE()] = mitk::IOConstants::PIXEL_TYPE_DOUBLE(); break;
				}

				retImage = mitk::IOUtil::Load<mitk::Image>(filePath, options);
			}
			else
			{
				retImage = mitk::IOUtil::Load<mitk::Image>(filePath);
			}

			return retImage;
		}

		void Save(mitk::Image::Pointer image, std::string filePath)
		{
			mitk::Image::Pointer retImage = nullptr;

			size_t dotIdx = filePath.rfind('.');
			if (dotIdx == std::string::npos)
			{
				cout << "Extension not Found" << endl;
				return;
			}

			std::string extension(filePath.size() - dotIdx, 0);
			std::transform(filePath.begin() + dotIdx, filePath.end(), extension.begin(), std::tolower);

			mitk::IOUtil::Save(image, filePath);
		}
	}
}