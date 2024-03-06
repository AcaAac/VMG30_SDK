#pragma once
#include "stdafx.h"
#include "vtkCylinderSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPLYReader.h"
#include "vtkGlyph3D.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProgrammableFilter.h"
#include "vtkCallbackCommand.h"
#include "vtkPolyDataMapper.h"
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkLineSource.h>
#include <vtkLight.h>
#include <vtkLightActor.h>


#define EIGEN_DONT_ALIGN_STATICALLY 1

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::Vector4f;

#define D2RAD(x) 3.1415*x/180.0
#define RAD2D(x) 180.0*x/3.1415

class t_Colour
{
public:
	t_Colour(void);
	t_Colour(int r, int g, int b);
	int Red;
	int Green;
	int Blue;
};

class Node
{
public:
	Node(void);
	~Node(void);

	char label[256]		;	//name of the node

	double boneL			;	//bone length
	Vector4f q			;	//vector containing rotation and length
	Vector4f qini			;	//initial pose
	Vector4f globpos		;	//global node position

	Matrix4f rotMat		;	//local transform
	Matrix4f rotMatInv		;	//inverse local trnasform
	Matrix4f globrotMat		;	//global transform
	Matrix4f globrotMatInv	;	//inverse global transform

	int posparent			;	//position of the parent node
	Node *parent			;	//pointer to parent node

	t_Colour colNode		;	//node colour
	t_Colour colCyl		;	//link colour

	//sphere representing the node
	vtkSmartPointer<vtkSphereSource> sphere;
	vtkSmartPointer<vtkPolyDataMapper> mapperS ;	
	vtkSmartPointer<vtkActor> actorS;
	
	//line representing the link
	vtkSmartPointer<vtkLineSource> line;
	vtkSmartPointer<vtkPolyDataMapper> mapperL ;	
	vtkSmartPointer<vtkActor> actorL;

	double *Vector2Pos(Vector4f ve);
	void Init(double r, double p, double y, double l, t_Colour scol, t_Colour ccol, Node *parent, int posparent, const char *lab);
	Matrix4f GetRotationMatrix(double r, double p, double y, double l);
	Matrix4f GetGlobalRotationMatrix(double r, double p, double y, double l);
	void UpdateAngles(double r, double p, double y, double l);
	void SetLenght(double l);
	void SetPosition(double x, double y, double z);
};

