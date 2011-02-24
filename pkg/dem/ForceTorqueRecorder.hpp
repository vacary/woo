#pragma once
#include <yade/pkg/common/Recorder.hpp>
#include <yade/core/Scene.hpp>

class ForceRecorder: public Recorder {
	public:
		Vector3r totalForce;
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS_CTOR(ForceRecorder,Recorder,"Engine saves the resulting force affecting to bodies, listed in `ids`. For instance, can be useful for defining the forces, which affects to _buldozer_ during its work.",
		((std::vector<int>,ids,,,"List of bodies whose state will be measured")),
		initRun=true;
	);
	DECLARE_LOGGER;
};
REGISTER_SERIALIZABLE(ForceRecorder);

class TorqueRecorder: public Recorder {
	public:
		Vector3r totalTorque;
		virtual void action();
	YADE_CLASS_BASE_DOC_ATTRS_CTOR(TorqueRecorder,Recorder,"Engine saves the total torque according to the given axis and ZeroPoint, the force is taken from bodies, listed in `ids`  For instance, can be useful for defining the torque, which affects on ball mill during its work.",
		((std::vector<int>,ids,,,"List of bodies whose state will be measured"))
		((Vector3r,rotationAxis,Vector3r::UnitX(),,"Rotation axis"))
		((Vector3r,zeroPoint,Vector3r::Zero(),,"Point of rotation center")),
		initRun=true;
	);
	DECLARE_LOGGER;
};

REGISTER_SERIALIZABLE(TorqueRecorder);

