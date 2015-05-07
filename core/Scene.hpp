#pragma once

#include<woo/core/Cell.hpp>
#include<woo/core/Engine.hpp>
#include<woo/core/EnergyTracker.hpp>
#include<woo/core/Plot.hpp>
#include<woo/core/LabelMapper.hpp>
#include<woo/core/Preprocessor.hpp>
#include<woo/core/ScalarRange.hpp>

#ifdef WOO_OPENCL
	#define __CL_ENABLE_EXCEPTIONS
	#include<CL/cl.hpp>
#endif

#ifdef WOO_OPENGL
	#include<woo/core/DisplayParameters.hpp>
#endif

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255 
#endif

struct Bound;
struct Field;

struct Scene: public Object{
		// this is managed by methods of Scene exclusively
		boost::mutex runMutex;
		bool runningFlag;
		boost::thread::id bgThreadId;
		// this will be std::atomic<bool>
		// once libstdc++ headers are accepted by clang++
		bool stopFlag; 
		bool stopFlagSet(){ boost::mutex::scoped_lock l(runMutex); return stopFlag; }
		shared_ptr<std::exception> except;
	public:
		// interface for python,
		void pyRun(long steps=-1, bool wait=false, Real time_=NaN);
		void pyStop();         
		void pyOne();         
		void pyWait();         
		bool running(); 
		void backgroundLoop();

		// initialize tags (author, date, time)
		void fillDefaultTags();
		// advance by one iteration by running all engines
		void doOneStep();
		// force: run regardless of step number, otherwise only when selfTestEvery is favorable
		void selfTest_maybe(bool force=false);
		void pySelfTest(){ selfTest_maybe(/*force*/true); }

		void postLoad(Scene&,void*);
		void preSave(Scene&){ preSaveDuration=pyGetDuration(); }

		#ifdef WOO_OPENCL
			shared_ptr<cl::Platform> platform;
			shared_ptr<cl::Device> device;
			shared_ptr<cl::Context> context;
			shared_ptr<cl::CommandQueue> queue;
		#endif

		// keep track of created labels; delete those which are no longer active and so on
		// std::set<std::string> pyLabels;

		std::vector<shared_ptr<Engine> > pyEnginesGet();
		void pyEnginesSet(const std::vector<shared_ptr<Engine> >&);
		shared_ptr<ScalarRange> getRange(const std::string& l) const;
		
		struct pyTagsProxy{
			Scene* scene;
			pyTagsProxy(Scene* _scene): scene(_scene){};
			std::string getItem(const std::string& key);
			void setItem(const std::string& key, const std::string& value);
			void delItem(const std::string& key);
			py::list keys();
			py::list items();
			py::list values();
			bool has_key(const std::string& key);
			void update(const pyTagsProxy& b);
		};
		pyTagsProxy pyGetTags(){ return pyTagsProxy(this); }
		shared_ptr<Cell> pyGetCell(){ return (isPeriodic?cell:shared_ptr<Cell>()); }

		boost::posix_time::ptime clock0;
		bool clock0adjusted; // flag to adjust clock0 in postLoad only once
		long pyGetDuration(){ return (boost::posix_time::second_clock::local_time()-clock0).total_seconds(); }

		typedef std::map<std::string,std::string> StrStrMap;

		void ensureCl(); // calls initCL or throws exception if compiled without OpenCL
		#ifdef WOO_OPENCL
			void initCl(); // initialize OpenCL using clDev
		#endif


		/*
			engineLoopMutex synchronizes access to particles/engines/contacts between
			* simulation itself (at the beginning of Scene::doOneStep) and
			* user access from python via Scene.paused() context manager
			PausedContextManager obtains the lock (warns every few seconds that the lock was not yet
			obtained, for diagnosis of deadlock) in __enter__ and releases it in __exit__.
		*/

		// workaround; will be removed once
		// http://stackoverflow.com/questions/12203769/boosttimed-mutex-guarantee-of-acquiring-the-lock-when-other-thread-unlocks-i
		// is solved
		#define WOO_LOOP_MUTEX_HELP
		#ifdef WOO_LOOP_MUTEX_HELP
			bool engineLoopMutexWaiting;
		#endif
		boost::timed_mutex engineLoopMutex;
		struct PausedContextManager{
			shared_ptr<Scene> scene;
			#ifdef WOO_LOOP_MUTEX_HELP
				bool& engineLoopMutexWaiting;
			#endif
			// stores reference to mutex, but does not lock it yet
			PausedContextManager(const shared_ptr<Scene>& _scene): scene(_scene)
				#ifdef WOO_LOOP_MUTEX_HELP
					, engineLoopMutexWaiting(scene->engineLoopMutexWaiting)
				#endif
			{}
			void __enter__();
			void __exit__(py::object exc_type, py::object exc_value, py::object traceback);
			static void pyRegisterClass(){
				py::class_<PausedContextManager,boost::noncopyable>("PausedContextManager",py::no_init).def("__enter__",&PausedContextManager::__enter__).def("__exit__",&PausedContextManager::__exit__);
			}
		};
		PausedContextManager* pyPaused(){ return new PausedContextManager(static_pointer_cast<Scene>(shared_from_this())); }

		// override Object::boostSave, to set lastSave correctly
		void boostSave(const string& out) WOO_CXX11_OVERRIDE;
		void saveTmp(const string& slot, bool quiet=true);

		// expand {tagName} in given string
		string expandTags(const string& s) const;

		#ifdef WOO_OPENGL
			#define woo_core_Scene__DisplayParameters__OPENGL /*WOO_OPENGL*/ ((vector<shared_ptr<DisplayParameters>>,dispParams,,AttrTrait<Attr::hidden>().noGui(),"'hash maps' of display parameters (since woo::serialization had no support for maps, emulate it via vector of strings in format key=value)"))
		#else
			#define woo_core_Scene__DisplayParameters__OPENGL
		#endif

	#define woo_core_Scene__CLASS_BASE_DOC_ATTRS_INI_CTOR_DTOR_PY \
		Scene,Object,ClassTrait().doc("Object comprising the whole simulation.").section("Scene","TODO",{"SceneAttachedObject","Field","LabelMapper","Cell","EnergyTracker"}) \
		, /*attrs*/ \
		((Real,dt,NaN,AttrTrait<>().timeUnit(),"Current timestep for integration.")) \
		((Real,nextDt,NaN,AttrTrait<>().timeUnit(),"Timestep for the next step (if not NaN, :obj:`dt` is automatically replaced by this value at the end of the step).")) \
		((Real,dtSafety,.9,,"Safety factor for automatically-computed timestep.")) \
		((Real,throttle,0,,"Insert this delay before each step when running in loop; useful for slowing down simulations for visual inspection if they happen too fast.\n\n.. note:: Negative values will not make the simulation run faster.")) \
		((long,step,0,AttrTrait<Attr::readonly>(),"Current step number")) \
		((bool,subStepping,false,,"Whether we currently advance by one engine in every step (rather than by single run through all engines).")) \
		((int,subStep,-1,AttrTrait<Attr::readonly>(),"Number of sub-step; not to be changed directly. -1 means to run loop prologue (cell integration), 0…n-1 runs respective engines (n is number of engines), n runs epilogue (increment step number and time.")) \
		((Real,time,0,AttrTrait<Attr::readonly>().timeUnit(),"Simulation time (virtual time) [s]")) \
		((long,stopAtStep,0,,"Iteration after which to stop the simulation.")) \
		((Real,stopAtTime,NaN,,":obj:`time` around which to stop the simulation.\n\n..note:: This value is not exact, has the granularity of :math:`\\Dt`: simulation will stopped at the moment when :obj:`stopAtTime` ≤ :obj:`time` < :obj:`dt`+:obj:`stopAtTime`. This condition may have some corner cases due to floating-point comparisons involved.")) \
		\
		((bool,isPeriodic,false,/*exposed as "periodic" in python */AttrTrait<Attr::hidden>(),"Whether periodic boundary conditions are active.")) \
		((bool,trackEnergy,false,,"Whether energies are being tracked.")) \
		((bool,deterministic,false,,"Hint for engines to order (possibly at the expense of performance) arithmetic operations to be independent of thread scheduling; this results in simulation with the same initial conditions being always the same. This is disabled by default, because of performance issues. Note that deterministic result is not \"more correct\" (neither physically, nor theoretically) than other result with different operation ordering; it is only self-consistent and feels better.")) \
		((int,selfTestEvery,0,,"Periodicity with which consistency self-tests will be run; 0 to run only in the very first step, negative to disable.")) \
		\
		((Vector2i,clDev,Vector2i(-1,-1),AttrTrait<Attr::triggerPostLoad>(),"OpenCL device to be used; if (-1,-1) (default), no OpenCL device will be initialized until requested. Saved simulations should thus always use the same device when re-loaded.")) \
		((Vector2i,_clDev,Vector2i(-1,-1),AttrTrait<Attr::readonly|Attr::noSave>(),"OpenCL device which is really initialized (to detect whether clDev was changed manually to avoid spurious re-initializations from postLoad")) \
		\
		((AlignedBox3r,boxHint,AlignedBox3r(),,"Hint for displaying the scene; overrides node-based detection. Set an element to empty box to disable.")) \
		\
		((bool,runInternalConsistencyChecks,true,AttrTrait<Attr::hidden>(),"Run internal consistency check, right before the very first simulation step.")) \
		\
		((StrStrMap,tags,,AttrTrait<Attr::hidden>(),"Arbitrary key=value associations (tags like mp3 tags: author, date, version, description etc.)")) \
		((shared_ptr<LabelMapper>,labels,make_shared<LabelMapper>(),AttrTrait<>().noGui().readonly(),"Atrbitrary key=object, key=list of objects, key=py::object associations which survive saving/loading. Labeled objects are automatically added to this container. This object is more conveniently accessed through the :obj:`lab` attribute, which exposes the mapping as attributes of that object rather than as a dictionary.")) \
		\
		((string,uiBuild,"",,"Command to run when a new main-panel UI should be built for this scene (called when the Controller is opened with this simulation, or the simulation is new to the controller).")) \
		\
		((vector<shared_ptr<Engine>>,engines,,,"Engines sequence in the simulation (direct access to the c++ sequence is shadowed by python property which access it indirectly).")) \
		((vector<shared_ptr<Engine>>,_nextEngines,,AttrTrait<Attr::hidden>(),"Engines to be used from the next step on; is returned transparently by S.engines if in the middle of the loop (controlled by subStep>=0).")) \
		((shared_ptr<EnergyTracker>,energy,make_shared<EnergyTracker>(),AttrTrait<Attr::readonly>().noGui(),"Energy values, if energy tracking is enabled.")) \
		((vector<shared_ptr<Field>>,fields,,AttrTrait<Attr::triggerPostLoad>().noGui(),"Defined simulation fields.")) \
		((shared_ptr<Cell>,cell,make_shared<Cell>(),AttrTrait<Attr::hidden>(),"Information on periodicity; only should be used if Scene::isPeriodic.")) \
		((std::string,lastSave,,AttrTrait<Attr::readonly>(),"Name under which the simulation was saved for the last time; used for reloading the simulation. Updated automatically, don't change.")) \
		((long,preSaveDuration,,AttrTrait<Attr::readonly>().noGui(),"Wall clock duration this Scene was alive before being saved last time; this count is incremented every time the scene is saved. When Scene is loaded, it is used to construct clock0 as current_local_time - lastSecDuration.")) \
		woo_core_Scene__DisplayParameters__OPENGL \
		((vector<shared_ptr<ScalarRange>>,ranges,,,"Scalar ranges to be rendered on the display as colormaps")) \
		((vector<shared_ptr<Object>>,any,,,"Storage for arbitrary Objects; meant for storing and loading static objects like Gl1_* functors to restore their parameters when scene is loaded.")) \
		((py::object,pre,,AttrTrait<>().noGui(),"Preprocessor used for generating this simulation; to be only used in user scripts to query preprocessing parameters, not in c++ code.")) \
		\
		/* postLoad checks the new value is not None */ \
		((shared_ptr<Plot>,plot,make_shared<Plot>(),AttrTrait<Attr::triggerPostLoad>().noGui(),"Data and settings for plots.")) \
		((shared_ptr<SceneCtrl>,ctrl,/*empty by default*/,AttrTrait<Attr::triggerPostLoad>(),"High-level control interface for this particular scene.")) \
		, /*ini*/ \
		, /*ctor*/ \
			fillDefaultTags(); \
			runningFlag=false; \
			clock0=boost::posix_time::second_clock::local_time(); \
			clock0adjusted=false; \
		, /* dtor */ pyStop();  \
		, /* py */ \
		.add_property("tags",&Scene::pyGetTags,"Arbitrary key=value associations (tags like mp3 tags: author, date, version, description etc.") \
		.add_property("duration",&Scene::pyGetDuration,"Number of (wall clock) seconds this instance is alive (including time before being loaded from file") \
		.add_property("cell",&Scene::pyGetCell,"Periodic space configuration (is None for aperiodic scene); set :obj:`periodic` to enable/disable periodicity") \
		.def_readwrite("periodic",&Scene::isPeriodic,"Set whether the scene is periodic or not") \
		.add_property("engines",&Scene::pyEnginesGet,&Scene::pyEnginesSet,"Engine sequence in the simulation") \
		.add_property("_currEngines",py::make_getter(&Scene::engines,py::return_value_policy<py::return_by_value>()),"Current engines, debugging only") \
		.add_property("_nextEngines",py::make_getter(&Scene::_nextEngines,py::return_value_policy<py::return_by_value>()),"Next engines, debugging only") \
		.def("getRange",&Scene::getRange,"Retrieve a *ScalarRange* object by its label") \
		/* WOO_OPENCL */ .def("ensureCl",&Scene::ensureCl,"[for debugging] Initialize the OpenCL subsystem (this is done by engines using OpenCL, but trying to do so in advance might catch errors earlier)") \
		.def("saveTmp",&Scene::saveTmp,(py::arg("slot")="",py::arg("quiet")=false),"Save into a temporary slot inside Master (loadable with O.loadTmp)") \
		\
		.def("run",&Scene::pyRun,(py::arg("steps")=-1,py::arg("wait")=false,py::arg("time")=NaN)) \
		.def("stop",&Scene::pyStop) \
		.def("one",&Scene::pyOne) \
		.def("wait",&Scene::pyWait) \
		.add_property("running",&Scene::running) \
		.def("paused",&Scene::pyPaused,py::return_value_policy<py::manage_new_object>()) \
		.def("selfTest",&Scene::pySelfTest,"Run self-tests (they are usually run automatically with, see :obj:`selfTestEvery`).") \
		.def("expandTags",&Scene::expandTags,"Expand :obj:`tags` written as ``{tagName}``, returns the expanded string.") \
		; /* define nested class */ \
		py::scope foo(_classObj); \
		py::class_<Scene::pyTagsProxy>("TagsProxy",py::init<pyTagsProxy>()).def("__getitem__",&pyTagsProxy::getItem).def("__setitem__",&pyTagsProxy::setItem).def("__delitem__",&pyTagsProxy::delItem).def("has_key",&pyTagsProxy::has_key).def("__contains__",&pyTagsProxy::has_key).def("keys",&pyTagsProxy::keys).def("update",&pyTagsProxy::update).def("items",&pyTagsProxy::items).def("values",&pyTagsProxy::values); \
		Scene::PausedContextManager::pyRegisterClass();
	
	WOO_DECL__CLASS_BASE_DOC_ATTRS_INI_CTOR_DTOR_PY(woo_core_Scene__CLASS_BASE_DOC_ATTRS_INI_CTOR_DTOR_PY);
	WOO_DECL_LOGGER;
};

WOO_REGISTER_OBJECT(Scene);
