#ifndef __SDECPERMANENTLINKPHYSICS_HPP__
#define __SDECPERMANENTLINKPHYSICS_HPP__

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <yade/yade-core/InteractionPhysics.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class SDECLinkPhysics : public InteractionPhysics
{
	public : SDECLinkPhysics();
	public : virtual ~SDECLinkPhysics();
	
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Attributes											///
///////////////////////////////////////////////////////////////////////////////////////////////////

	public : Real kn;				// normal elastic constant.
	public : Real ks;				// shear elastic constant.

	public : Real knMax;
	public : Real ksMax;

	public : Real initialKn;			// initial normal elastic constant.
	public : Real initialKs;			// initial shear elastic constant.
	public : Real equilibriumDistance;		// equilibrium distance
	public : Real initialEquilibriumDistance;	// initial equilibrium distance
	public : Vector3r prevNormal;			// unit normal of the contact plane.
	public : Vector3r normalForce;			// normal force applied on a DE
	public : Vector3r shearForce;			// shear force applied on a DE

/////////////////////////////////////////////////////// FIXME : this goes to another dynlib - MDEM
	//public : Vector3r initRotation1;
	//public : Vector3r initRotation2;

	public : Quaternionr prevRotation1;
	//public : Quaternionr currentRotation1;
	public : Quaternionr prevRotation2;
	//public : Quaternionr currentRotation2;
	
	public : Vector3r thetar;
	public : Real heta;
	public : Real averageRadius;
	public : Real kr;
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
/// Serialization										///
///////////////////////////////////////////////////////////////////////////////////////////////////

	protected : virtual void postProcessAttributes(bool deserializing);
	public : void registerAttributes();

	REGISTER_CLASS_NAME(SDECLinkPhysics);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

REGISTER_SERIALIZABLE(SDECLinkPhysics,false);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __SDECPERMANENTLINKPHYSICS_HPP__

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

