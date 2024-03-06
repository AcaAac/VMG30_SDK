#include "stdafx.h"
#include "wxVTK.h"

#define GETR(x) rots[3*x]
#define GETP(x) rots[3*x+1]
#define GETY(x) rots[3*x+2]
#define GETL(x) bonesL[x]
#define GREEN t_Colour(0,255,0)
#define WHITE t_Colour(255,255,255)
#define BLUE t_Colour(0,0,255)
#define RED t_Colour(255,0,0)
#define YELLOW t_Colour(0,255,255)

const double rots[] =
{
	0.0, 0.0, 0.0,	//rotazione root	(sempre 0)
	0.0, 0.0, 0.0,	//rotazione metacarpal	
	0.0, 0.0, 0.0,	//rotazione carpal
	//thumb
	0.0, 0.0, 115.0,	//rotazione tmb0
	0.0, 0.0, -70.0,	//rotazione tmb1
	0.0, 0.0, 0.0,		//rotazione tmb2
	0.0, 0.0, 0.0,		//rotazione tmb3
	//index
	0.0, 0.0, 15.0,	//rotazione ind0
	0.0, 0.0, 5.0,		//rotazione ind1
	0.0, 0.0, 0.0,		//rotazione ind2
	0.0, 0.0, 0.0,		//rotazione ind3
	//middle
	0.0, 0.0, 0.0,		//rotazione mid0
	0.0, 0.0, 0.0,		//rotazione mid1
	0.0, 0.0, 0.0,		//rotazione mid2
	0.0, 0.0, 0.0,		//rotazione mid3
	//ring
	0.0, 0.0, -15.0,	//rotazione rng0
	0.0, 0.0, -5.0,		//rotazione rng1
	0.0, 0.0, 0.0,		//rotazione rng2
	0.0, 0.0, 0.0,		//rotazione rng3
	//little
	0.0, 0.0, -35.0,	//rotazione rng0
	0.0, 0.0, -5.0,		//rotazione rng1
	0.0, 0.0, 0.0,		//rotazione rng2
	0.0, 0.0, 0.0,		//rotazione rng3
};

//lunghezze dei bones in mm
const double bonesL[] =
{
	0.0,
	60.0,	//lunghezza root -> meta
	30.0,	//lunghezza meta -> carpal

	//thumb
	45.0,	//lunghezza meta -> tmb0
	60.0,
	28.0,
	25.0,
	//index
	90.0,	//lunghezza meta -> ind0
	40.0,
	28.0,
	20.0,
	//middle
	85.0,	//lunghezza meta -> mid0
	35.0,
	32.0,
	25.0,
	//ring
	85.0,	//lunghezza meta -> rng0
	30.0,
	28.0,
	25.0,
	//little
	75.0,	//lunghezza meta -> lit0
	30.0,
	25.0,
	17.0,

	0.0,
};


wxVTK::wxVTK()
{
	counter = 0;
	InitNodes();
	//create rendering windows
	ren1 = vtkRenderer::New();
	renWin = vtkRenderWindow::New();//vtkRenderWindow::New();
	renWin->SetSize(1000,1000);
	renWin->AddRenderer(ren1);

	//add all actors from the nodes
	int i = 0;
	for (i=0;i<nodes.size();i++){
		ren1->AddActor(nodes[i].actorS);
		ren1->AddActor(nodes[i].actorL);
	}
	//set background 
	ren1->SetBackground(0.0, 0.0, 0.0);
	
	//create camera
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	camera->SetClippingRange(0.1,2000.0);
	camera->SetPosition(1000, 0, 400);
	camera->SetFocalPoint(0, 0, 0);
	camera->SetViewUp(0,0, 1);
	ren1->SetActiveCamera(camera);

	//update nodes positions
	UpdateChildren(0);

	//render points
	renWin->Render();
}


wxVTK::~wxVTK(void)
{
}

/** \brief Nodes initialization */
void wxVTK::InitNodes()
{

	Node n;
	//root
	n.Init(GETR(0),GETP(0),GETY(0),GETL(0),WHITE,BLUE,NULL,-1,"ROOT");
	nodes.push_back(n);
	//wrist
	n.Init(GETR(1),GETP(1),GETY(1),GETL(1),WHITE,BLUE,&nodes[0],0,"WRIST");
	nodes.push_back(n);
	//hand
	n.Init(GETR(2),GETP(2),GETY(2),GETL(2),WHITE,BLUE,&nodes[1],1,"HAND");
	nodes.push_back(n);

	//thumb0
	n.Init(GETR(3),GETP(3),GETY(3),GETL(3),WHITE,BLUE,&nodes[2],2,"THUMB0");
	nodes.push_back(n);
	//thumb1
	n.Init(GETR(4),GETP(4),GETY(4),GETL(4),WHITE,BLUE,&nodes[3],3,"THUMB1");
	nodes.push_back(n);
	//thumb2
	n.Init(GETR(5),GETP(5),GETY(5),GETL(5),WHITE,BLUE,&nodes[4],4,"THUMB2");
	nodes.push_back(n);
	//thumb3
	n.Init(GETR(6),GETP(6),GETY(6),GETL(6),WHITE,BLUE,&nodes[5],5,"THUMB3");
	nodes.push_back(n);

	//index0
	n.Init(GETR(7),GETP(7),GETY(7),GETL(7),WHITE,BLUE,&nodes[2],2,"INDEX0");
	nodes.push_back(n);
	//index1
	n.Init(GETR(8),GETP(8),GETY(8),GETL(8),WHITE,BLUE,&nodes[7],7,"INDEX1");
	nodes.push_back(n);
	//index2
	n.Init(GETR(9),GETP(9),GETY(9),GETL(9),WHITE,BLUE,&nodes[8],8,"INDEX2");
	nodes.push_back(n);
	//index3
	n.Init(GETR(10),GETP(10),GETY(10),GETL(10),WHITE,BLUE,&nodes[9],9,"INDEX3");
	nodes.push_back(n);

	//middle0
	n.Init(GETR(11),GETP(11),GETY(11),GETL(11),WHITE,BLUE,&nodes[2],2,"MIDDLE0");
	nodes.push_back(n);
	//index1
	n.Init(GETR(12),GETP(12),GETY(12),GETL(12),WHITE,BLUE,&nodes[11],11,"MIDDLE1");
	nodes.push_back(n);
	//index2
	n.Init(GETR(13),GETP(13),GETY(13),GETL(13),WHITE,BLUE,&nodes[12],12,"MIDDLE2");
	nodes.push_back(n);
	//index3
	n.Init(GETR(14),GETP(14),GETY(14),GETL(14),WHITE,BLUE,&nodes[13],13,"MIDDLE3");
	nodes.push_back(n);

	//ring0
	n.Init(GETR(15),GETP(15),GETY(15),GETL(15),WHITE,BLUE,&nodes[2],2,"RING0");
	nodes.push_back(n);
	//ring1
	n.Init(GETR(16),GETP(16),GETY(16),GETL(16),WHITE,BLUE,&nodes[15],15,"RING1");
	nodes.push_back(n);
	//ring2
	n.Init(GETR(17),GETP(17),GETY(17),GETL(17),WHITE,BLUE,&nodes[16],16,"RING2");
	nodes.push_back(n);
	//ring33
	n.Init(GETR(18),GETP(18),GETY(18),GETL(18),WHITE,BLUE,&nodes[17],17,"RING3");
	nodes.push_back(n);

	//little0
	n.Init(GETR(19),GETP(19),GETY(19),GETL(19),WHITE,BLUE,&nodes[2],2,"LITTLE0");
	nodes.push_back(n);
	//little1
	n.Init(GETR(20),GETP(20),GETY(20),GETL(20),WHITE,BLUE,&nodes[19],19,"LITTLE1");
	nodes.push_back(n);
	//little2
	n.Init(GETR(21),GETP(21),GETY(21),GETL(21),WHITE,BLUE,&nodes[20],20,"LITTLE2");
	nodes.push_back(n);
	//little3
	n.Init(GETR(22),GETP(22),GETY(22),GETL(22),WHITE,BLUE,&nodes[21],21,"LITTLE3");
	nodes.push_back(n);
}


/** \brief Update node position, values taken from SDK */
void wxVTK::SetNodePosition(const char *label, double x, double y, double z)
{
	//update the node position
	Vector4f pos1, pos2;
	Node *n = GetNode(label);
	if (n){
		n->SetPosition(x,y,z);
		pos1 = n->globpos;
		int posparent = n->posparent;
		if ((posparent>=0)&&(posparent<nodes.size())){
			Node *np = &nodes[posparent];
			pos2 = np->globpos;
			//update sphere position for rendering
			n->sphere->SetCenter(n->Vector2Pos(n->globpos));
			n->sphere->Update();
			n->line->SetPoint1(n->Vector2Pos(np->globpos));
			n->line->SetPoint2(n->Vector2Pos(n->globpos));
			n->line->Update();
		}
	}
}


/** \brief Recursive update of bones, not used in realtime animation, postion are used, this function is computed directly in the SDK */
void wxVTK::UpdateChildren(int pospar)
{
	int i = 0;
	Node *np = &nodes[pospar];
	for (i=0;i<nodes.size();i++){
		Node *n = &nodes[i];
		if (n->posparent == pospar){
			n->rotMat = n->GetGlobalRotationMatrix(D2RAD(n->q(0)),D2RAD(n->q(1)),D2RAD(n->q(2)),np->q(3));
			Vector4f vl = Vector4f(n->q(3),0.0f,0.0f,1.0f);
			//posizione nel sistema di riferimento del padre
			Vector4f newpos = np->globrotMat*n->rotMat*vl;
			n->globpos = newpos;
			n->globrotMat = np->globrotMat*n->rotMat;
			n->sphere->SetCenter(np->Vector2Pos(n->globpos));
			n->sphere->Update();
			n->line->SetPoint1(np->Vector2Pos(np->globpos));
			n->line->SetPoint2(np->Vector2Pos(n->globpos));
			n->line->Update();
			UpdateChildren(i);
		}
	}
}

/** \brief Set node sphere colour */
void wxVTK::SetNodeColour(const char *name, t_Colour col)
{
	Node *n = GetNode(name);
	if (n){
		n->actorS->GetProperty()->SetColor(col.Red/255.0,col.Green/255.0,col.Blue/255.0);
	}
}

/** \brief Get a node by its name */
Node *wxVTK::GetNode(const char *label)
{
	int i = 0;
	for (i=0;i<nodes.size();i++){
		Node *n = &nodes[i];
		if (!strcmp(n->label,label)) return n;
	}
	return NULL;
}

/** \brief Set a node orientation (RPY notation used) */
void wxVTK::SetNodeOrientation(const char *name, double r, double p, double y)
{
	Node *n = GetNode(name);
	if (n){
		if (r>9000.0) r = n->q(0);
		if (p>9000.0) p = n->q(1);
		if (y>9000.0) y = n->q(2);
		n->UpdateAngles(r,p,y,n->q(3));
	}
}

/** \brief Set node relative orientation, with respect to parent node */
void wxVTK::SetNodeRelOrientation(const char *name, double r, double p, double y)
{
	Node *n = GetNode(name);
	if (n){
		r += n->qini(0);
		p += n->qini(1);
		y += n->qini(2);
		n->UpdateAngles(r,p,y,n->q(3));
	}
}

/** Updat rendering */
void wxVTK::Update()
{
	//UpdateChildren(0);
	renWin->Render();
}