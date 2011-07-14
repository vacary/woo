#ifdef YADE_OPENGL

#pragma once

#include<yade/core/Field.hpp>

struct ScalarGlRep: public NodeGlRep{
	void render(const shared_ptr<Node>&,GLViewInfo*);
	YADE_CLASS_BASE_DOC_ATTRS(ScalarGlRep,NodeGlRep,"Render scalar value at associated node",
		((Real,val,0,,"Value to be rendered"))
		((int,how,0,,"Different ways to render given value; 0=number, 1=colored point, 2=colored sphere"))
		((int,prec,5,,"Precision for rendering numbers"))
		((Real,relSz,.05,,"Size of rendered spheres (if selected), relative to scene radius"))
		((shared_ptr<ScalarRange>,range,,,"Extrema values for the scalar, to determine colors."))
	);
};
REGISTER_SERIALIZABLE(ScalarGlRep);

struct VectorGlRep: public NodeGlRep{
	void render(const shared_ptr<Node>&, GLViewInfo*);
	YADE_CLASS_BASE_DOC_ATTRS(VectorGlRep,NodeGlRep,"Render vector value at associated node, as an arrow",
		((Vector3r,val,Vector3r::Zero(),,"Value to be rendered"))
		((Real,relSz,.2,,"Size of maximum-length arrows, relative to scene radius"))
		((Real,scaleExp,1,,"Exponent for scaling arrow size as |normalized val|^scaleExp. NaN disables scaling (all arrows the same size)."))
		((shared_ptr<ScalarRange>,range,,,"Extrema values for vector norm, to determine colors."))
	);
};
REGISTER_SERIALIZABLE(VectorGlRep);

struct TensorGlRep: public NodeGlRep{
	void render(const shared_ptr<Node>&, GLViewInfo*);
	void postLoad(TensorGlRep&);
	YADE_CLASS_BASE_DOC_ATTRS(TensorGlRep,NodeGlRep,"Render tensor (given as 3x3 matrix) as its principal components.",
		((Matrix3r,val,Matrix3r::Zero(),Attr::triggerPostLoad,"Value to be rendered."))
		((Matrix3r,eigVec,Matrix3r::Zero(),(Attr::noSave|Attr::readonly),"eigenvectors as columns, updated in postLoad."))
		((Vector3r,eigVal,Vector3r::Zero(),(Attr::noSave|Attr::readonly),"eigenvalues of corresponding eigenvectors, updated in postLoad."))
		((Vector3r,skew,Vector3r::Zero(),(Attr::noSave),"skew (asymmetric) components of the tensor"))
		((Real,relSz,.1,,"Size of maximum-length arrows, relative to scene radius"))
		((Real,skewRelSz,-1,,"Size of maximum-length skew curved arrows; if negative, use relSz instead."))
		((Real,scaleExp,1,,"Exponent for scaling arrow sizem kuje wutg VectorGlRep. NaN disables scaling, making all arrows the same size."))
		((shared_ptr<ScalarRange>,range,,,"Extrema values for symmetric components."))
		((shared_ptr<ScalarRange>,skewRange,,,"Extrema values for skew components"))
	);
};
REGISTER_SERIALIZABLE(TensorGlRep);


#endif