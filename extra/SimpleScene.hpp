
#pragma once
#include<yade/core/FileGenerator.hpp>
#include<yade/core/PhysicalAction.hpp>
#include<yade/extra/Shop.hpp>


class SimpleScene: public FileGenerator {
	public:
		USCTGen(){};
		~USCTGen (){};
		virtual bool generate();
	protected :
		void registerAttributes(){ FileGenerator::registerAttributes(); }
	NEEDS_BEX("Force","Momentum");
	REGISTER_CLASS_NAME(SimpleScene);
	REGISTER_BASE_CLASS_NAME(FileGenerator);
	DECLARE_LOGGER;
};
REGISTER_SERIALIZABLE(SimpleScene,false);

