#include "ImageProcessingKernel.h"
#include "Utility/UtilImage.h"

ProcessingKernel::ProcessingKernel()
{

}

ProcessingKernel::ProcessingKernel(int sizeX, int sizeY, int sizeZ)
	: m_kernel(sizeX* sizeY* sizeZ, 0)
{
	createArray(sizeX, sizeY, sizeZ);
}

ProcessingKernel::~ProcessingKernel()
{

}

void ProcessingKernel::createArray(int sizeX, int sizeY, int sizeZ)
{
	m_kernel.clear();
	m_kernel.assign(sizeX * sizeY * sizeZ, 0.f);
	m_kernel.shrink_to_fit();

	m_size[0] = sizeX;
	m_size[1] = sizeY;
	m_size[2] = sizeZ;
}

double ProcessingKernel::getElement(int x, int y, int z)
{
	if (x < 0 || x >= m_size[0] || y < 0 || y >= m_size[1] || z < 0 || z >= m_size[2])
		throw std::exception("Kernel out of range");

	itk::Index<3> idx;
	idx[0] = x;
	idx[1] = y;
	idx[2] = z;

	return m_kernel[Util::Image::coordinateToBufferOffset(idx, getKernelSize())];
}
double* ProcessingKernel::getElementPtr()
{
	return &m_kernel.front();
}
void ProcessingKernel::setElement(int x, int y, int z, double val)
{
	if (x < 0 || x >= m_size[0] || y < 0 || y >= m_size[1] || z < 0 || z >= m_size[2])
		throw std::exception("Kernel out of range");

	itk::Index<3> idx;
	idx[0] = x;
	idx[1] = y;
	idx[2] = z;

	m_kernel[Util::Image::coordinateToBufferOffset(idx, getKernelSize())] = val;
}
void ProcessingKernel::setElement(const std::vector<double>& arr)
{
	if (arr.size() != m_kernel.size())
		throw std::exception("Kernel InputArray size is different");
	m_kernel = arr;
}

itk::Index<3> ProcessingKernel::getCenter()
{
	itk::Index<3> center;
	center[0] = (m_size[0] / 2);
	center[1] = (m_size[1] / 2);
	center[2] = (m_size[2] / 2);
	return center;
}
itk::Size<3> ProcessingKernel::getKernelSize()
{
	return m_size;
}
int ProcessingKernel::getElementCount()
{
	return m_kernel.size();
}

ProcessingKernel::Pointer ProcessingKernel::createSharpenKernel2D()
{
	ProcessingKernel::Pointer kernel(new ProcessingKernel(3, 3, 1));
	std::vector<double> kernelElement = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
	kernel->setElement(kernelElement);

	return kernel;
}

ProcessingKernel::Pointer ProcessingKernel::createSharpenKernel3D()
{
	ProcessingKernel::Pointer kernel(new ProcessingKernel(3, 3, 3));
	std::vector<double> kernelElement = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	kernel->setElement(kernelElement);

	return kernel;
}

ProcessingKernel::Pointer ProcessingKernel::createGaussianKernel2D(float sigma, int size)
{
	// change size by sigma value
	if (size == 0)
		size = sigma * 6 + 1;
	//
	// change size to odd
	if (size % 2 == 0)
		size++;
	//

	ProcessingKernel::Pointer kernel(new ProcessingKernel(size, size, 1));

	float radius = (size - 1.f) / 2.f;

	std::vector<double> kernelElement(size * size);

	float sum = 0;

	int kIdx = 0;
	for (float y = -radius; y <= radius; y += 1.f)
	{
		for (float x = -radius; x <= radius; x += 1.f)
		{
			double val = exp(-((x * x) + (y * y)) / (2.f * sigma * sigma))/* / (2 * 3.14159265358979 * sigma * sigma)*/;

			kernelElement[kIdx++] = val;
			sum += val;
		}
	}

	for (auto& iter : kernelElement)
		iter /= sum;

	kernel->setElement(kernelElement);

	return kernel;
}

