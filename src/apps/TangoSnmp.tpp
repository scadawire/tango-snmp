
//=============================================================================
//
// file :        TangoSnmp.tpp
//
// description : Include file for the TangoSnmp class
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
//================================================================================


#ifndef TangoSnmp_TPP
#define TangoSnmp_TPP

#include <tango.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>


namespace TangoSnmp_ns
{

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::read_snmp_data()
//
// description :
//		Method to read SNMP data with a GET network call.
//
// argument :
//		in :
//			- att_name: The attribute name
//			- a_ptr: Pointer to the attribute Attr instance. Itś in this class that is stored the data used for
//					 attribute data transfer.
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::read_snmp_data(string &att_name,ScalarSnmpData<T> *a_ptr)
{
	DEBUG_STREAM << "Entering read_snmp_data for attribute data " << att_name << endl;

//
// Try to retrieve OID from our map from attribute name
//

	map<string,Snmp_OID>::iterator ite = att_OIDs.find(att_name);

	if (ite == att_OIDs.end())
	{
		stringstream ss;
		ss << "The attribute " << att_name << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_snmp_data");
	}

//
// Ask SNMP for this data
//

	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	struct variable_list *vars;
    int status;

	pdu = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(pdu,ite->second.anOID,ite->second.anOID_len);

    if (simulationMode) 
    {
        //do what the agent response would do
        status = STAT_SUCCESS;
        response = snmp_clone_pdu(pdu);
        response->errstat = SNMP_ERR_NOERROR;
        int x = ((ite->second.sim_limits[1] - ite->second.sim_limits[0])/10.0) * (ite->second.sim_count % 100);
        ite->second.sim_count++;
        int y = snmp_set_var_typed_value(response->variables, ASN_INTEGER, &x, sizeof(x));
        if (y != 0) 
        {
            ERROR_STREAM << "Y !=0 " << y << endl;
        }
    } 
    else 
    {
        if(NULL == ss)
            return;
        status = snmp_sess_synch_response(ss,pdu,&response);
    }
	error_handling(status,att_name,response,"TangoSnmp::read_snmp_data","SNMP_GET",ATTR);

	vars = response->variables;
//	print_variable(vars->name,vars->name_length,vars);

	if (vars->val.integer == 0x0)
	{
		stringstream ss;
		ss << "No error returned by snmp call but data pointer is null!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_snmp_data");		
	}

	memcpy(a_ptr->get_attr_data(),(void *)(vars->val.integer),sizeof(T));

	snmp_free_pdu(response);
}

//
// We have to specialize this method for the DevString case
//

template <>
void TangoSnmp::read_snmp_data<Tango::DevString>(string &att_name,ScalarSnmpData<Tango::DevString> *a_ptr)
{
	DEBUG_STREAM << "Entering read_snmp_data for attribute data (Type DevString)" << att_name << endl;

//
// Try to retrieve OID from our map from attribute name
//

	map<string,Snmp_OID>::iterator ite = att_OIDs.find(att_name);

	if (ite == att_OIDs.end())
	{
		stringstream ss;
		ss << "The attribute " << att_name << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_snmp_data");
	}
// Skip the SNMP processes when simulating.
    if(simulationMode)
    {
        a_ptr->set_attr_data(CORBA::string_dup("simulating"));
        return;
    }

//
// Ask SNMP for this data
//

	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	struct variable_list *vars;

	pdu = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(pdu,ite->second.anOID,ite->second.anOID_len);

    if(NULL == ss)
        return;

    int status = snmp_sess_synch_response(ss,pdu,&response);
    error_handling(status,att_name,response,"TangoSnmp::read_snmp_data","SNMP_GET",ATTR);

	vars = response->variables;

//	print_variable(vars->name,vars->name_length,vars);

	if (vars->type == ASN_OBJECT_ID)
	{

//
// The SNMP data is one object ID, convert it to string
//

		stringstream ss;

		oid *tmp_ptr = vars->val.objid;
		int nb = vars->val_len / sizeof(oid);
		for (int loop = 0;loop < nb;loop++)
		{
			ss << *tmp_ptr;
			if (loop != (nb - 1))
				ss << ".";
			tmp_ptr++;
		}
		a_ptr->set_attr_data(CORBA::string_dup(ss.str().c_str()));
	}
	else
	{
		char *ptr = CORBA::string_alloc(vars->val_len + 1);
		a_ptr->set_attr_data(ptr);
		memcpy(ptr,(void *)(vars->val.string),vars->val_len);
		ptr[vars->val_len] = '\0';
	}

	snmp_free_pdu(response);
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::read_snmp_data_bulk
//
// description :
//		Method to read the ScalarSnmpDataAttr attribute. This attribute is a dynamic attribute
//
// argument :
//		in :
//			- att: Reference to the Tango Attribute object
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::read_snmp_data_bulk(string &att_name,SpectrumSnmpData<T> *a_ptr)
{
	DEBUG_STREAM << "Entering read_snmp_data_bulk for attribute " << att_name << endl;

//
// Try to retrieve OID from our map from attribute name
//

	map<string,Snmp_OID>::iterator ite = att_OIDs.find(att_name);

	if (ite == att_OIDs.end())
	{
		stringstream ss;
		ss << "The attribute " << att_name << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_snmp_data_bulk");
	}

//
// Ask SNMP for this data
//
    if(NULL == ss)
        return;

	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	struct variable_list *vars;

	string full_name(ite->second.snmp_name);
	string::size_type pos = full_name.rfind(':');
	string endname = full_name.substr(pos + 1);

	pdu = snmp_pdu_create(SNMP_MSG_GETBULK);
	pdu->non_repeaters = 0;
	pdu->max_repetitions = 50;

	snmp_add_null_var(pdu,ite->second.anOID,ite->second.anOID_len);

	int status = snmp_sess_synch_response(ss,pdu,&response);
	error_handling(status,att_name,response,"TangoSnmp::read_snmp_data_bulk","SNMP_GETBULK",ATTR);

	vector<T> &d_ref = a_ptr->get_attr_data_ref();
	if (d_ref.empty() == false)
		d_ref.clear();

	for (vars = response->variables;vars;vars = vars->next_variable)
	{

//
// Limit data to those we really want
//

		struct tree *tree_ptr = get_tree(vars->name,vars->name_length,get_tree_head());
		if (strcmp(tree_ptr->label,endname.c_str()) == 0)
		{
//			print_variable(vars->name,vars->name_length,vars);

			T tmp_data;
			memcpy(&tmp_data,(void *)(vars->val.integer),sizeof(T));
			d_ref.push_back(tmp_data);
		}
		else
			break;
	}

	snmp_free_pdu(response);
}

//
// Specialisation for strings
//

template <>
void TangoSnmp::read_snmp_data_bulk<Tango::DevString>(string &att_name,SpectrumSnmpData<Tango::DevString> *a_ptr)
{
	DEBUG_STREAM << "Entering read_snmp_data_bulk for attribute " << att_name << " - DevString data type" << endl;

//
// Try to retrieve OID from our map from attribute name
//

	map<string,Snmp_OID>::iterator ite = att_OIDs.find(att_name);

	if (ite == att_OIDs.end())
	{
		stringstream ss;
		ss << "The attribute " << att_name << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_snmp_data_bulk");
	}

//
// Ask SNMP for this data
//
    if(NULL == ss)
        return;
    
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	struct variable_list *vars;

	string full_name(ite->second.snmp_name);
	string::size_type pos = full_name.rfind(':');
	string endname = full_name.substr(pos + 1);

	pdu = snmp_pdu_create(SNMP_MSG_GETBULK);
	pdu->non_repeaters = 0;
	pdu->max_repetitions = 50;

	snmp_add_null_var(pdu,ite->second.anOID,ite->second.anOID_len);

    int status = snmp_sess_synch_response(ss,pdu,&response);
	error_handling(status,att_name,response,"TangoSnmp::read_snmp_data_bulk","SNMP_GETBULK",ATTR);

	vector<Tango::DevString> &d_ref = a_ptr->get_attr_data_ref();

	for (vars = response->variables;vars;vars = vars->next_variable)
	{

//
// Limit data to those we really want
//

		struct tree *tree_ptr = get_tree(vars->name,vars->name_length,get_tree_head());
		if (strcmp(tree_ptr->label,endname.c_str()) == 0)
		{
//			print_variable(vars->name,vars->name_length,vars);

			Tango::DevString tmp_data = CORBA::string_alloc(vars->val_len + 1);
			memcpy(tmp_data,(void *)(vars->val.string),vars->val_len);
			tmp_data[vars->val_len] = '\0';
			d_ref.push_back(tmp_data);

		}
		else
			break;
	}

	snmp_free_pdu(response);
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::read_ScalarSnmpDataAttr()
//
// description :
//		Method to read the ScalarSnmpDataAttr attribute. This attribute is a dynamic attribute
//
// argument :
//		in :
//			- att: Reference to the Tango Attribute object
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::read_ScalarSnmpDataAttr(Tango::Attribute &att)
{
	map<string,BaseSnmpData *>::iterator ite;
	ite = dattr_scalar_data.find(att.get_name());
	if (ite == dattr_scalar_data.end())
	{
		stringstream ss;
		ss << "The attribute " << att.get_name() << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_ScalarSnmpDataAttr");
	}

	ScalarSnmpData<T> *a_ptr = static_cast<ScalarSnmpData<T> *>(ite->second);

	read_snmp_data<T>(att.get_name(),a_ptr);

	att.set_value(a_ptr->get_attr_data());
}

//
// Specialization of this method for the DevString data type which requires specific memory management
//

template <>
void TangoSnmp::read_ScalarSnmpDataAttr<Tango::DevString>(Tango::Attribute &att)
{
	map<string,BaseSnmpData *>::iterator ite;
	ite = dattr_scalar_data.find(att.get_name());
	if (ite == dattr_scalar_data.end())
	{
		stringstream ss;
		ss << "The attribute " << att.get_name() << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_ScalarSnmpDataAttr");
	}

	ScalarSnmpData<Tango::DevString> *a_ptr = static_cast<ScalarSnmpData<Tango::DevString> *>(ite->second);

	//if (*(a_ptr->get_attr_data()) != NULL)
	//	CORBA::string_free(*(a_ptr->get_attr_data()));

	read_snmp_data<Tango::DevString>(att.get_name(),a_ptr);

	att.set_value(a_ptr->get_attr_data());
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::write_snmp_data()
//
// description :
//		Method to write SNMP data with a SET network call.
//
// argument :
//		in :
//			- att_name: The attribute name
//			- data: Data to be written
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::write_snmp_data(string &att_name,T data)
{
	DEBUG_STREAM << "Entering write_snmp_data for attribute " << att_name << endl;
    if(simulationMode)
    {
        return;
    }

//
// Try to retrieve OID from our map from attribute name
//

	map<string,Snmp_OID>::iterator ite = att_OIDs.find(att_name);

	if (ite == att_OIDs.end())
	{
		stringstream ss;
		ss << "The attribute " << att_name << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::write_snmp_data");
	}

//
// Prepare SNMP call
//

	struct snmp_pdu* response = NULL;
	struct snmp_pdu* pdu = snmp_pdu_create(SNMP_MSG_SET);

	size_t len = sizeof(data);

	snmp_pdu_add_variable(pdu,ite->second.anOID,ite->second.anOID_len,ite->second.snmp_type,(u_char*)&data,len);

//
// Send the Request out.
//
    if(NULL == ss)
        return;

    int status = snmp_sess_synch_response(ss,pdu,&response);
    error_handling(status,att_name,response,"TangoSnmp::write_ScalarSnmpDataAttr","SNMP_SET",ATTR);

	if (response)
		snmp_free_pdu(response);
}

//
// Once more, we need a specialisation for string
//

template <>
void TangoSnmp::write_snmp_data<Tango::DevString>(string &att_name,Tango::DevString data)
{
	DEBUG_STREAM << "Entering write_snmp_data for attribute " << att_name << " - DevString data type" << endl;

//
// Try to retrieve OID from our map from attribute name
//

	map<string,Snmp_OID>::iterator ite = att_OIDs.find(att_name);

	if (ite == att_OIDs.end())
	{
		stringstream ss;
		ss << "The attribute " << att_name << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::write_snmp_data");
	}

//
// Prepare SNMP call
//

	struct snmp_pdu* response = NULL;
	struct snmp_pdu* pdu = snmp_pdu_create(SNMP_MSG_SET);

	size_t len = ::strlen(data);

	snmp_pdu_add_variable(pdu,ite->second.anOID,ite->second.anOID_len,ite->second.snmp_type,(u_char*)data,len);

//
// Send the Request out.
//

    int status = snmp_sess_synch_response(ss,pdu,&response);
	error_handling(status,att_name,response,"TangoSnmp::write_ScalarSnmpDataAttr","SNMP_SET",ATTR);

	if (response)
		snmp_free_pdu(response);
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::write_ScalarSnmpDataAttr()
//
// description :
//		Method to write attribute ScalarSnmpDataAttr. This attribute is used as a dynamic attribute
//
// argument :
//		in :
//			- att: The Tango WAttribute object reference
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::write_ScalarSnmpDataAttr(Tango::WAttribute &att)
{
    if(simulationMode)
    {
        return;
    }

	DEBUG_STREAM << "Entering write_SpectrumSnmpDataAttr for attribute " << att.get_name() << endl;

	T data;
	att.get_write_value(data);

	write_snmp_data<T>(att.get_name(),data);
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::read_SpectrumSnmpDataAttr()
//
// description :
//		Method to read the SpectrumSnmpDataAttr attribute. This attribute is a dynamic attribute
//
// argument :
//		in :
//			- att: Reference to the Tango Attribute object
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::read_SpectrumSnmpDataAttr(Tango::Attribute &att)
{
	map<string,BaseSnmpData *>::iterator ite;
	ite = dattr_spectrum_data.find(att.get_name());
	if (ite == dattr_spectrum_data.end())
	{
		stringstream ss;
		ss << "The attribute " << att.get_name() << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_SpectrumSnmpDataAttr");
	}

	SpectrumSnmpData<T> *a_ptr = static_cast<SpectrumSnmpData<T> *>(ite->second);

	read_snmp_data_bulk<T>(att.get_name(),a_ptr);

	att.set_value(a_ptr->get_attr_data_ptr(),a_ptr->get_attr_data_ref().size());
}

template <>
void TangoSnmp::read_SpectrumSnmpDataAttr<Tango::DevString>(Tango::Attribute &att)
{
	map<string,BaseSnmpData *>::iterator ite;
	ite = dattr_spectrum_data.find(att.get_name());
	if (ite == dattr_spectrum_data.end())
	{
		stringstream ss;
		ss << "The attribute " << att.get_name() << " can't be found in the internal maps!";

		Tango::Except::throw_exception("TangoSnmp_InternalError",ss.str(),"TangoSnmp::read_SpectrumSnmpDataAttr");
	}

//
// Free previously allocated memory
//

	SpectrumSnmpData<Tango::DevString> *a_ptr = static_cast<SpectrumSnmpData<Tango::DevString> *>(ite->second);
	vector<Tango::DevString> &att_data = a_ptr->get_attr_data_ref();
	if (att_data.empty() == false)
	{
		for (size_t loop = 0;loop < att_data.size();loop++)
			CORBA::string_free(att_data[loop]);
		att_data.clear();
	}

//
// Get data and associate them to Tango attribute
//

	read_snmp_data_bulk<Tango::DevString>(att.get_name(),a_ptr);

	att.set_value(a_ptr->get_attr_data_ptr(),a_ptr->get_attr_data_ref().size());
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::write_SpectrumSnmpDataAttr()
//
// description :
//		Method to write attribute SpectrumSnmpDataAttr. This attribute is used as a dynamic attribute
//
// argument :
//		in :
//			- att: The Tango WAttribute object reference
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
void TangoSnmp::write_SpectrumSnmpDataAttr(Tango::WAttribute &att)
{
	DEBUG_STREAM << "Entering write_SpectrumSnmpDataAttr for attribute " << att.get_name() << endl;

	Tango::Except::throw_exception("TangoSnmp_NotImplementedYet",
								    "It is not implemented yet to write array of SNMP data",
								    "TangoSnmp::write_SpectrumSnmpDataAttr");
}

//---------------------------------------------------------------------------------------------------------------------
//
// method :
//		TangoSnmp::check_write_authorized()
//
// description :
//		Method to check if the snmp data associated to the attribute is really writable.
//		IT happens that even if the data is defined as read-write in the MIB, the egent refuses to write this value.
//		For instance, this is the case for the data ipForwarding which is defined as read-write in the RFC1213
//		but which is not writable with the default agent.
//
// argument :
//		in :
//			- att_name: The attribute name
//			- attr: Pointer to the Tango object
//			- first_coherency_failed: Boolean used for status string management
//
// return :
//		This method returns:
//			0 is everything works well
//			-1 if it is not possible to write the data
//
//--------------------------------------------------------------------------------------------------------------------

template <typename T>
int TangoSnmp::check_write_authorized(string &att_name,bool &first_coherency_failed)
{
	int ret = 0;

	try
	{
		ScalarSnmpData<T> tmp_data;
		read_snmp_data<T>(att_name,&tmp_data);
		write_snmp_data<T>(att_name,*(tmp_data.get_attr_data()));
	}
	catch (Tango::DevFailed &e)
	{
		string desc = e.errors[0].desc.in();
		if (desc.find("notWritable") != string::npos)
		{
			set_state(Tango::ALARM);

			stringstream ss;
			ss << "SNMP agent does not allow writing the attribute " << att_name;

			if (first_coherency_failed == true)
			{
				set_status(ss.str());
				first_coherency_failed = false;
			}
			else
				append_status(ss.str(),true);

			ret = -1;
		}
	}

	return ret;
}

}	//	End of namespace

#endif   //	TangoSnmp_TPP
