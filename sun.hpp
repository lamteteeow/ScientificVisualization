#pragma once

#include "crtbp.hpp"

#include <vtkTexturedSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkSmartPointer.h>
#include <vtkTexture.h>
#include <vtkImageData.h>
#include <vtkImagePointIterator.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkOpenGLGPUVolumeRayCastMapper.h>


/// <summary>
/// Class that represents the sun.
/// </summary>
class Sun
{
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	Sun()
	{
		// Assign path to a variable
		std::string path = "C:/Users/Admin/Code Projects/Scientific Visualization framework/scivis-ss23-main/images/sun.jpg";

		// Load sun image using vtkJPEGReader
		vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
		reader->SetFileName(path.c_str());

		// Create an instance of vtkTexture and assign the image
		mTexture = vtkSmartPointer<vtkTexture>::New();
		mTexture->SetInputConnection(reader->GetOutputPort());

		//// Create a sphere with texture coordinates
		const double sphere_radius = 0.1;
		vtkNew<vtkTexturedSphereSource> sphereSource;
		sphereSource->SetRadius(sphere_radius);
		sphereSource->SetPhiResolution(100);
		sphereSource->SetThetaResolution(100);
		vtkNew<vtkPolyDataMapper> polyMapper;
		polyMapper->SetInputConnection(sphereSource->GetOutputPort());

		mActor = vtkSmartPointer<vtkActor>::New();
		mActor->SetMapper(polyMapper);
		mActor->SetTexture(mTexture);
		mActor->SetPosition(CRTBP::Sun().x(), CRTBP::Sun().y(), 0);

		// Initialize variables
		const double halo_radius = 2 * sphere_radius; //radius of halo
		int dimensions[3] = { 64, 64, 64 };
		double spacing[3] = { 0.01, 0.01, 0.01 };
		double origin[3] = { (CRTBP::Sun().x() - 0.5 * (dimensions[0] - 1)) * spacing[0] , (CRTBP::Sun().y() - 0.5 * (dimensions[1] - 1)) * spacing[1] , -0.5 * (dimensions[2] - 1) * spacing[2]};

		
		mHalo = vtkSmartPointer<vtkImageData>::New(); // Create a vtkImageData object for the halo volume
		mHalo->SetDimensions(dimensions);
		mHalo->SetOrigin(origin);
		mHalo->SetSpacing(spacing);
		mHalo->AllocateScalars(VTK_FLOAT, 1); // Allocate memory for the image data

		

		//double sigma = 1.0;  // Controls the spread of the Gaussian
		//double maxIntensity = 1.0;  // Maximum intensity at the origin
		//for (int z = 0; z < dimensions[2]; z++) {
		//	for (int y = 0; y < dimensions[1]; y++) {
		//		for (int x = 0; x < dimensions[0]; x++) {
		//			// Compute the distance from the origin
		//			double dx = (x * spacing[0]) - origin[0];
		//			double dy = (y * spacing[1]) - origin[1];
		//			double dz = (z * spacing[2]) - origin[2];
		//			double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
		//			// Compute the scalar value based on the Gaussian distribution
		//			float scalarValue = maxIntensity * std::exp(-(distance * distance) / (2 * sigma * sigma));
		//			// Assign the scalar value to the voxel
		//			*scalarData = scalarValue;
		//			scalarData++;
		//		}
		//	}
		//}

		// Create a transfer function that maps scalar values to colors and opacities
		vtkSmartPointer<vtkColorTransferFunction> rgb = vtkSmartPointer<vtkColorTransferFunction>::New();
		rgb->AddRGBPoint(0, 0.8, 0.8, 0.0); // ?? why does x not affect
		//rgb->AddRGBPoint(0.0, 0.0, 0.3, 0.0);

		vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
		//opacity->AddPoint( 1, 0.1 ); // ?? why does x not affect
		//opacity->AddPoint(0.3, 0.3);

		// Get a pointer to the scalar data array to initialize
		//float* scalarData = static_cast<float*>(mHalo->GetScalarPointer());
		vtkSmartPointer<vtkFloatArray> scalars = vtkFloatArray::SafeDownCast(mHalo->GetPointData()->GetScalars());

		double sigma = 40;  // Controls the spread of the Gaussian

		vtkImagePointIterator iter(mHalo);
		while (!iter.IsAtEnd())
		{
			double point[3];
			iter.GetPosition(point);
			double dx = point[0] * spacing[0];
			double dy = point[1] * spacing[1];
			double dz = point[2] * spacing[2];
			double distance = std::sqrt(dx * dx + dy * dy + dz * dz);
			//double normalizedDistance = distance / (dimensions[0] * spacing[0] / 2); 
			double normalizedDistance = distance / (halo_radius * spacing[0]); //normalized distance relative to the halo radius
			double value;
			if (normalizedDistance >= 1) {
				value = 0;
			}
			else if (normalizedDistance == 0) {
				value = 0.99;
			}
			else {
				//value = distance / (dimensions[0] * spacing[0]); //normalized distance
				value = 1 * std::exp(-(normalizedDistance * normalizedDistance) / (2 * sigma * sigma));
			}
			// insert the value into the image
			scalars->SetValue(iter.GetId(), value);
			//opacity->AddPoint(iter.GetId(), value);
			//rgb->AddRGBPoint(iter.GetId(), value, 1 - value, value);
			iter.Next();
		}

		// Create a volume property that defines how the volume is rendered
		vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
		volumeProperty->SetColor(rgb); //or rgb.GetPointer()
		volumeProperty->SetScalarOpacity(opacity);
		//volumeProperty->SetShade(1);
		//volumeProperty->SetInterpolationTypeToLinear();

		// Create a volume mapper that will generate images for the volume
		vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkOpenGLGPUVolumeRayCastMapper>::New();
		volumeMapper->SetInputData(mHalo);

		// Create volume as actor
		mVolume = vtkSmartPointer<vtkVolume>::New();
		mVolume->SetMapper(volumeMapper);
		mVolume->SetProperty(volumeProperty);
		mVolume->SetPosition(CRTBP::Sun().x(), CRTBP::Sun().y(), 0);
	}

	/// <summary>
	/// Updates the properties of the Sun.
	/// </summary>
	/// <param name="dt">Time passed since the last Update in milliseconds.</param>
	/// <param name="t">Total time passed since start of the application in milliseconds.</param>
	void Update(double dt, double t)
	{
		// Rotate actor
		double angle_dt = dt * 0.01;			// Not a realistic rotation speed!
		mActor->RotateWXYZ(angle_dt, 0, 0, 1);   // 0 tilted rotation axis along the equator
		//mVolume->RotateWXYZ(angle_dt * 5, 0, 0, 1);	
	}

	/// <summary>
	/// Adds the actors to the renderer.
	/// </summary>
	/// <param name="renderer">Renderer to add the actors to.</param>
	void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
	{
		renderer->AddVolume(mVolume); //AddActor also worked ?
		renderer->AddActor(mActor);
	}

private:
	Sun(const Sun&) = delete;				// Delete the copy-constructor.
	void operator=(const Sun&) = delete;	// Delete the assignment operator.

	vtkSmartPointer<vtkVolume> mVolume;		// Actor that represents the sun volume.
	vtkSmartPointer<vtkTexture> mTexture;	// Texture for the sun volume.
	vtkSmartPointer<vtkImageData> mHalo;	// Image data for the halo volume.
	vtkSmartPointer<vtkActor> mActor;		// Actor for the sun sphere
};
