#include "stdafx.h"
#include "Node.h"

t_Colour::t_Colour()
{
}

t_Colour::t_Colour(int r, int g, int b)
{
	Red = r;
	Green = g;
	Blue = b;
}


Node::Node(void)
{
}


Node::~Node(void)
{
}


/** \brief Return global transofrmation matrix, RPY notation is used, l is the bone length */
Matrix4f Node::GetGlobalRotationMatrix(double r, double p, double y, double l)
{
#if 1
	Matrix3f ry, rp, rr;
	ry << cos(y), -sin(y), 0,
		sin(y), cos(y), 0,
		0,0,1;

	rp << cos(p), 0, sin(p),
		0,1,0,
		-sin(p), 0, cos(p);

	rr << 1,0,0,
		0, cos(r), -sin(r),
		0, sin(r), cos(r);
#endif
	Matrix3f rotmat = ry*rp*rr;
	Matrix4f retm;
	int i,j ;
	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			retm(i,j) = rotmat(i,j);
		}
	}
	retm(0,3) = l;
	retm(1,3) = 0;
	retm(2,3) = 0;

	retm(3,0) = 0;
	retm(3,1) = 0;
	retm(3,2) = 0;
	retm(3,3) = 1;
	return retm;
}



Matrix4f Node::GetRotationMatrix(double r, double p, double y, double l)
{
#if 1
	Matrix3f ry, rp, rr;
	ry << cos(y), -sin(y), 0,
		sin(y), cos(y), 0,
		0,0,1;

	rp << cos(p), 0, sin(p),
		0,1,0,
		-sin(p), 0, cos(p);

	rr << 1,0,0,
		0, cos(r), -sin(r),
		0, sin(r), cos(r);
#endif
	Matrix3f rotmat = ry*rp*rr;
	Matrix4f retm;
	int i,j ;
	for (i=0;i<3;i++){
		for (j=0;j<3;j++){
			retm(i,j) = rotmat(i,j);
		}
	}
	retm(0,3) = 0;
	retm(1,3) = 0;
	retm(2,3) = 0;

	retm(3,0) = 0;
	retm(3,1) = 0;
	retm(3,2) = 0;
	retm(3,3) = 1;
	return retm;
}

#if 1
double *Node::Vector2Pos(Vector4f ve)
{
	double pos[3];
	pos[0] = ve(0);
	pos[1] = ve(1);
	pos[2] = ve(2);
	return pos;
}
#endif


/** \brief Update relative orientation of the node with respect ot the parent */
void Node::UpdateAngles(double r, double p, double y, double l)
{
	q = Vector4f(r,p,y,l);
	//aggiorno matrice di rotazione locale
	rotMat = GetRotationMatrix(D2RAD(r),D2RAD(p),D2RAD(y),l);
	globrotMat = GetRotationMatrix(D2RAD(r),D2RAD(p),D2RAD(y),l);
	rotMatInv = rotMat.inverse();
}

/** \brief Set bone length */
void Node::SetLenght(double l)
{
	boneL = l;
	q(3) = l ;
	rotMat = GetRotationMatrix(D2RAD(q(0)),D2RAD(q(1)),D2RAD(q(2)),l);
	globrotMat = GetRotationMatrix(D2RAD(q(0)),D2RAD(q(1)),D2RAD(q(2)),l);
}


/** \brief Node initialization */
void Node::Init(double r, double p, double y, double l, t_Colour scol, t_Colour ccol, Node *par, int pospar, const char *lab)
{
	sprintf(label,"%s",lab);
	posparent = pospar;
	q = Vector4f(r,p,y,l);
	qini = q;
	globpos = Vector4f(0,0,0,0);
	rotMat = GetRotationMatrix(D2RAD(r),D2RAD(p),D2RAD(y),l);
	rotMatInv = rotMat.inverse();
	globrotMat = GetGlobalRotationMatrix(D2RAD(r),D2RAD(p),D2RAD(y),l);
	globrotMatInv = rotMatInv;
	parent = par;
	colNode = scol;
	colCyl = ccol;
	boneL = l;

	//sphere representing the node
	sphere = vtkSmartPointer<vtkSphereSource>::New();
	sphere->SetRadius(5.0);
	sphere->SetCenter(Vector2Pos(globpos));
	sphere->Update();

	mapperS = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperS->SetInputConnection(sphere->GetOutputPort());
	actorS = vtkSmartPointer<vtkActor>::New();
	actorS->SetMapper(mapperS);
	actorS->GetProperty()->SetColor(scol.Red/255.0,scol.Green/255.0,scol.Blue/255.0);

	//line representing the bone connection this node to the parent
	line = vtkSmartPointer<vtkLineSource>::New();
	line->SetPoint1(Vector2Pos(globpos));
	line->SetPoint2(Vector2Pos(globpos));
	line->Update();

	mapperL = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperL->SetInputConnection(line->GetOutputPort());
	actorL = vtkSmartPointer<vtkActor>::New();
	actorL->SetMapper(mapperL);
	actorL->GetProperty()->SetColor(ccol.Red/255.0,ccol.Green/255.0,ccol.Blue/255.0);

}

/** \brief Set node absolute position, with respect to the root */
void Node::SetPosition(double x, double y, double z)
{
	globpos(0) = x;
	globpos(1) = y;
	globpos(2) = z;
}


