/*----- PROTECTED REGION ID(TangoSnmpClass.h) ENABLED START -----*/
//=============================================================================
//
// file :        TangoSnmpClass.h
//
// description : Include for the TangoSnmp root class.
//               This class is the singleton class for
//                the TangoSnmp device class.
//               It contains all properties and methods which the
//               TangoSnmp requires only once e.g. the commands.
//
// project :     Tango<-->Snmp link
//
// This file is part of Tango device class.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
// $Author$
//
// $Revision$
// $Date$
//
// $HeadURL$
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef TangoSnmpClass_H
#define TangoSnmpClass_H

#include <tango.h>
#include <TangoSnmp.h>

// Due to READ, WRITE definition in net-snmp/library/snmp_impl.h file
// included by net-snmp/net-snmp-includes.h included by TangoSnmp.h

#undef READ
#undef WRITE

/*----- PROTECTED REGION END -----*/	//	TangoSnmpClass.h


namespace TangoSnmp_ns
{
/*----- PROTECTED REGION ID(TangoSnmpClass::classes for dynamic creation) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	TangoSnmpClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute MIBFileDirs class definition
class MIBFileDirsAttrib: public Tango::SpectrumAttr
{
public:
	MIBFileDirsAttrib():SpectrumAttr("MIBFileDirs",
			Tango::DEV_STRING, Tango::READ, 128) {};
	~MIBFileDirsAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TangoSnmp *>(dev))->read_MIBFileDirs(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoSnmp *>(dev))->is_MIBFileDirs_allowed(ty);}
};

//	Attribute LoadedMIBModules class definition
class LoadedMIBModulesAttrib: public Tango::SpectrumAttr
{
public:
	LoadedMIBModulesAttrib():SpectrumAttr("LoadedMIBModules",
			Tango::DEV_STRING, Tango::READ, 1024) {};
	~LoadedMIBModulesAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<TangoSnmp *>(dev))->read_LoadedMIBModules(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<TangoSnmp *>(dev))->is_LoadedMIBModules_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command GetSnmpData class definition
class GetSnmpDataClass : public Tango::Command
{
public:
	GetSnmpDataClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	GetSnmpDataClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~GetSnmpDataClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TangoSnmp *>(dev))->is_GetSnmpData_allowed(any);}
};

//	Command SetSnmpData class definition
class SetSnmpDataClass : public Tango::Command
{
public:
	SetSnmpDataClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SetSnmpDataClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SetSnmpDataClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TangoSnmp *>(dev))->is_SetSnmpData_allowed(any);}
};


//=========================================
//	Define classes for dynamic commands
//=========================================
//	Command Simulate class definition
class SimulateClass : public Tango::Command
{
public:
	SimulateClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	SimulateClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~SimulateClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<TangoSnmp *>(dev))->is_Simulate_allowed(any);}
};


/**
 *	The TangoSnmpClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  TangoSnmpClass : public Tango::DeviceClass
#else
class TangoSnmpClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(TangoSnmpClass::Additionnal DServer data members) ENABLED START -----*/

	friend class TangoSnmp;

	/*----- PROTECTED REGION END -----*/	//	TangoSnmpClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static TangoSnmpClass *init(const char *);
		static TangoSnmpClass *instance();
		~TangoSnmpClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		TangoSnmpClass(string &);
		static TangoSnmpClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	TangoSnmp_H
