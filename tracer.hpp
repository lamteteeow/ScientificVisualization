#pragma once

#include "crtbp.hpp"

/// <summary>
/// Class that represents the third body.
/// </summary>
class Tracer
{
private:
	static constexpr double MaxRadius = 0.01;				// max radius of the tube
	static constexpr int NumIntegrationSteps = 5000;		// number of intergration steps
	static constexpr double IntegrationStepSize = 0.01;		// integration step size
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	Tracer()
	{
	}

	/// <summary>
	/// Updates the properties of the tail geometry.
	/// </summary>
	/// <param name="dt">Time passed since the last Update in milliseconds.</param>
	/// <param name="t">Total time passed since start of the application in milliseconds.</param>
	void Update(double dt, double t)
	{
	}

	/// <summary>
	/// Calculates a trajectory from the picked point.
	/// </summary>
	/// <param name="pnt">3D world coordinate that was picked.</param>
	void Pick(const Vector3d& pnt)
	{
	}

	/// <summary>
	/// Adds the actors to the renderer.
	/// </summary>
	/// <param name="renderer">Renderer to add the actors to.</param>
	void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
	{
	}

private:

	Tracer(const Tracer&) = delete;				// Delete the copy-constructor.
	void operator=(const Tracer&) = delete;		// Delete the assignment operator.
};