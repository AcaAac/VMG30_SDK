#pragma once
#include "stdafx.h"
#include "Node.h"

#include <vector>
using namespace std;

class wxVTK
{
public:
	wxVTK();
	~wxVTK(void);
	vtkRenderer *ren1;
	vtkRenderWindow *renWin;

	std::vector <vtkSmartPointer<vtkActor>> actors;
	std::vector <vtkSmartPointer<vtkPolyDataMapper>> mappers ;	
	std::vector <vtkSmartPointer<vtkSphereSource>> spheres;

	void SetNodeOrientation(const char *str,double r, double p, double y);
	void SetNodeRelOrientation(const char *str,double r, double p, double y);
	
	Node *GetNode(const char *label);

	void SetNodeColour(const char *name, t_Colour col);

	std::vector <Node> nodes;

	void InitNodes();
	void UpdateChildren(int pospar);
	double teta;
	int counter;
	void SetNodePosition(const char *label, double x, double y, double z);
	void Update();

};

