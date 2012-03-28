#include <cassert>
#include "Base/GHeader.h"
#include "BerkelyFileSystem.h"

const char* ms_name_namespace_db = "../namespace.db";

BerkelyDBFileSystem::BerkelyDBFileSystem(const string base_dir, int verbose):m_base_dir(base_dir),m_verbose(verbose)
{
	m_base_dir = base_dir;
	using std::cerr;
	/*
	m_pDbEnv = new DbEnv(0);
	cout << "test point 1" << endl;
	if(m_pDbEnv == NULL)
	{
		LOG_ERROR("GRF initialization: "," Berkely File System initialize environment error ");
	//	fprintf(stderr,"Failed to create environment of DB %s", m_base_dir);
		assert(0);
	}
	m_pDbEnv->set_error_stream(&cerr);
	m_pDbEnv->set_cachesize(0, 256 *1024, 0);

	cout << "test point 1" << endl;
	m_pDbEnv->set_data_dir(m_base_dir.c_str());

	cout << "test pointuuuuu " << endl;
	m_pDbEnv->open(m_base_dir.c_str(),
	    DB_CREATE  | DB_INIT_MPOOL | 
	    DB_INIT_TXN, 0);
	LOG_INFO("GRF initialization: "," Berkely File System create environment");
*/
	cout << "test point 1" << endl;
	try{
		m_pNamespaceDb= new Db(NULL, 0) ;
		//m_pNamespaceDb= new Db(m_pDbEnv, 0) ;
		m_pNamespaceDb->open(NULL, ms_name_namespace_db , NULL, DB_BTREE, DB_CREATE, 0664);
		LOG_INFO("GRF initialization: "," Berkely File System create DB");

		int ret;
		Dbt key, data;


		key.set_data((void*) "/");
		key.set_size(strlen("/") + 1);
		
		ret = m_pNamespaceDb->get(NULL,&key, &data, DB_GET_BOTH);
		if(ret == DB_NOTFOUND)
		{
			data.set_data((void*)"");
			data.set_size(strlen("") + 1);
			m_pNamespaceDb->put(NULL, &key, &data, 0);

			key.set_data((void*)"/giraffe/");
			key.set_size(strlen("/giraffe/") + 1);
			m_pNamespaceDb->put(NULL, &key, &data, 0);
			LOG_INFO("giraffe initial: ","create /giraffe/ directory success!");

			key.set_data((void*)"/giraffe/metadata/");
			key.set_size(strlen("/giraffe/metadata/") + 1);
			m_pNamespaceDb->put(NULL, &key, &data, 0);
			LOG_INFO("giraffe initial: ","create /giraffe/metadata/ directory success!");
		}
		
	}
	catch(DbException &e)
	{
		LOG_ERROR("giraffe initialization: "," Berkely File System initialize file error ");
		m_pNamespaceDb->err(e.get_errno(), "Error!");
	}
	catch(std::exception &e)
	{
		LOG_ERROR("giraffe initialization: "," Berkely File System initialize file error ");
		//m_pNamespaceDb->err(e.get_errno(), "Error!");
		std::cerr << e.what() << endl;
	}
}

BerkelyDBFileSystem::~BerkelyDBFileSystem()
{
	m_pNamespaceDb->close(0) ;
	delete m_pNamespaceDb;

	m_pDbEnv->close(0);
	delete m_pDbEnv;
	
	LOG_INFO("Berkely Database close","");
}

int BerkelyDBFileSystem::mkdir(const string dir)
{
	string dirname;
	string normal;
	decode_fpath(dir, dirname, normal);
	if(normal[normal.length() - 1] != '/') normal += '/';

	int ret;
	Dbt data, key;
	data = Dbt();
	key = Dbt();
	
	key.set_data((void *)normal.c_str());
	key.set_size(normal.length() + 1);
	data.set_data((void*)"");
	data.set_size(strlen("")+1);
	ret = m_pNamespaceDb->get(NULL,&key, &data, 0);
	if(0 != ret)	
	{
		ret = m_pNamespaceDb->put(NULL, &key, &data, 0);
		LOG_INFO("DIR CREATE :", normal.c_str());
	}
	else 
	{
		LOG_INFO("DIR ALREADY EXITS: ", normal.c_str());
	}

	return ret;
}

string BerkelyDBFileSystem::read(const string fpath)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	int ret;
	Dbc *cursorp;
	Dbt *data, *key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = &Dbt();
	key = &Dbt();

	//key->set_data((void *)normal.c_str());
	//key->set_size(normal.length() + 1);
	while((ret = cursorp->get(key, data, DB_NEXT)) == 0)
	{
		if(normal.compare((char *)key->get_data()) == 0)
		{
			break;
		}
	}
	cursorp->close();
	string content((char *)data->get_data());
	//delete data;
	//delete key;
	
	if(DB_NOTFOUND == ret)  return NULL;
	return content;
}

int BerkelyDBFileSystem::write(const string fpath, const string context)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	int ret;
	Dbc *cursorp;
	Dbt *data, *key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = &Dbt();
	key = &Dbt();
	
	key->set_data((void *) fpath.c_str());
	key->set_size(fpath.length() + 1);
	ret = cursorp->get(key, data, DB_SET);
	/*while((ret = cursorp->get(key, data, DB_NEXT)) == 0)
	{
		if(normal.compare((char *)key->get_data()) == 0)
		{
			break;
		}
	}*/
	if(0 == ret)
	{
		string cont = string((char *)data->get_data()) + context;
		data->set_data((void *)cont.c_str());
		data->set_size(cont.length() + 1);
		cursorp->put(key,data,DB_KEYFIRST);
	}
	cursorp->close();
	//delete data;
	//delete key;
	if(0 == ret)	return true;
	return false;
}


int BerkelyDBFileSystem::create(const string fpath, const string &attr)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	int ret;
	Dbc *cursorp;
	Dbt *data, *key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = &Dbt();
	key = &Dbt();

	key->set_data((void *)dirname.c_str());
	key->set_size(dirname.length() + 1);
	ret = cursorp->get(key, data, DB_SET);
	
	if(DB_NOTFOUND == ret)
	{
		data->set_data((void*)"");
		data->set_size(strlen("")+1);
		cursorp->put(key, data, DB_KEYFIRST);
	}

	key->set_data((void *)normal.c_str());
	key->set_size(normal.length() + 1);
	
	ret = cursorp->get(key, data, DB_SET);
	if(DB_NOTFOUND == ret)
	{
		data->set_data((void *) attr.c_str());
		data->set_size(attr.length() + 1);
		cursorp->put(key, data, DB_KEYFIRST);
	}

	cursorp->close();
	//delete key;
	//delete data;
	return true;
}

int BerkelyDBFileSystem::getList(const string dir, AttrMap& attrs)
{
	string dirname;
	string normal;
	decode_fpath(dir, dirname, normal);
	if(normal[normal.length() - 1] != '/') normal += '/';

	string dirkey = normal + NODE_ATTR_DELIM;

	int ret;
	Dbc *cursorp;
	Dbt *data, *key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = &Dbt();
	key = &Dbt();
	//key->set_data((void *)dirkey.c_str());
	key->set_data((void *)normal.c_str());
	key->set_size(normal.length() + 1);

	ret = cursorp->get(key, data, DB_SET);

	string tmp;
	string keystr, valstr;
	string cur_file_key = normal + NODE_ATTR_DELIM;

	if(0 == ret)
	{
		while((ret = cursorp->get(key, data, DB_NEXT)) == 0)
		{
			
			tmp = string((char *)key->get_data());
			if(tmp.find(normal) == 0 )	
			{
				if(!is_node_file(tmp))
					continue;
				keystr = (char *) key->get_data();
				valstr = (char *) data->get_data();
				attrs[keystr] = valstr;
				cur_file_key = keystr + NODE_ATTR_DELIM;
			}
			else
				break;
		}
	}

	cursorp->close();
	return true;
}

int BerkelyDBFileSystem::getChildren(const string dir, AttrKeyMap& attrs)
{
	string dirname;
	string normal;
	decode_fpath(dir, dirname, normal);
	if(normal[normal.length() - 1] != '/') normal += '/';

	int ret;
	Dbc *cursorp;
	Dbt *data, *key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = &Dbt();
	key = &Dbt();
	//key->set_data((void *)dirkey.c_str());
	key->set_data((void *)normal.c_str());
	key->set_size(normal.length() + 1);

	ret = cursorp->get(key, data, DB_SET);

	string tmp;
	string keystr, valstr;
	string cur_file_key = normal;

	if(DB_NOTFOUND != ret)
	{
		while((ret = cursorp->get(key, data, DB_NEXT)) != DB_NOTFOUND)
		{
			tmp = string((char *)key->get_data());
			if(tmp.find(normal) == 0 )	
			{
				if(!is_node_file(tmp))
					continue;
				keystr = (char *) key->get_data();
				valstr = (char *) data->get_data();
				attrs.push_back(keystr);
			}
			else
				break;
		}
	}

	cursorp->close();
	return true;
}

int BerkelyDBFileSystem::del(const string fname)
{
	string dirname;
	string normal;
	decode_fpath(fname, dirname, normal);

	string zttrkey_predix = normal + NODE_ATTR_DELIM;

	int ret;
	Dbc *cursorp;
	Dbt data, key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = Dbt();
	key = Dbt();
	
	key.set_data((void *)normal.c_str());
	key.set_size(normal.length() + 1);

	ret = cursorp->get(&key, &data, DB_SET);

	if(0 == ret)
	{
		cursorp->del(0);
		while ((ret = cursorp->get(&key, &data, DB_NEXT)) == 0)
		{
			string attrkey = (char*)key.get_data();
			if(is_node_attr(normal, attrkey))
				cursorp->del(0);
			else 
				break;
		}
	}

	//delete data;
	//delete key;
	cursorp->close();
	return ret;
}

int BerkelyDBFileSystem::create_node(const string node, AttrMap attrs)
{
	string dirname;
	string normal;
	decode_fpath(node, dirname, normal);

	int ret;
	string attrkey;
	Dbt data, key;
	data = Dbt();
	key = Dbt();

	key.set_data((void *)normal.c_str());
	key.set_size(normal.length() + 1);

	m_pNamespaceDb->put(NULL, &key, &data, 0);
	LOG_INFO("CREATE FILE:", "file name: " << normal);
	
	AttrMap::const_iterator cit = attrs.begin();

	while(cit != attrs.end())
	{
		build_attr_key(normal,cit->first, attrkey);
		key.set_data((void *)attrkey.c_str());
		key.set_size(attrkey.length() + 1);

		data.set_data((void *)(cit->second).c_str());
		data.set_size((cit->second).length() + 1);

		ret = m_pNamespaceDb->put(NULL, &key, &data, 0);
		
		LOG_INFO("INSERT FILE ATTRINUTE SUCCESS: ", "fiel: " << normal.c_str() << " , attr : "<< cit->first << " value: " << cit->second);
		cit ++;
	}

	return ret;
}

bool BerkelyDBFileSystem::exits(const string fpath)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	int ret;
	Dbt data, key;
	data = Dbt();
	key = Dbt();

	key.set_data((void *)normal.c_str());
	key.set_size(normal.length() + 1);

	if((ret = m_pNamespaceDb->get(NULL, &key, &data, 0)) == DB_NOTFOUND)
		return false;

	return true;
}

int BerkelyDBFileSystem::get_node_attr(const string fpath, AttrKeyMap keys, AttrMap& attrs)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	string attrkey;
	string attrname, attrval;
	int ret;
	AttrKeyMap::const_iterator cit = keys.begin();

	for( ; cit != keys.end(); cit ++)
	{
		attrname = cit->data();
		build_attr_key(normal, attrname, attrkey);

		Dbt data, key;
		data = Dbt();
		key = Dbt();

		key.set_data((void *)attrkey.c_str());
		key.set_size(attrkey.length() + 1);

		attrval = "";

		if((ret = m_pNamespaceDb->get(NULL, &key, &data, 0)) != DB_NOTFOUND)
		{
			attrval = (char*)data.get_data();
			LOG_INFO("GET FILE ATTRINUTE SUCCESS: ", "fiel: " << normal.c_str() << " , attr : "<< attrname.c_str() << " value: " << attrval.c_str());
		}
		else
		{
			LOG_ERROR("GET FILE ATTRINUTE FAILURE: ", "can not get fiel: " << normal << "attr: " << attrname);
		}
		
		attrs[attrname] = attrval;
	}
	
	
	return ret;
}

int BerkelyDBFileSystem::set_node_attr(const string fpath, AttrMap attrs)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	string attrkey,attrname,attrval;
	int ret;
	AttrMap::const_iterator cit = attrs.begin();
	Dbt data, key;
	data = Dbt();
	key = Dbt();
	key.set_data((void *)normal.c_str());
	key.set_size(normal.length() + 1);
	if((ret = m_pNamespaceDb->get(NULL, &key, &data, 0)) != DB_NOTFOUND)
	{
		for(; cit != attrs.end(); cit ++)
		{
			attrname =cit->first;
			attrval = cit->second;
			build_attr_key(normal, attrname, attrkey);
	
			data = Dbt();
			key = Dbt();
		
			{
				key.set_data((void *)attrkey.c_str());
				key.set_size(attrkey.length() + 1);
		
				data.set_data((void *)attrval.c_str());
				data.set_size(attrval.length() + 1);

				ret = m_pNamespaceDb->put(NULL, &key, &data, 0);
				//attrval = (char*)data.get_data();
				LOG_INFO("SET FILE ATTRINUTE SUCCESS: ", "fiel: " << normal.c_str() << " , attr : "<< attrname.c_str() << " value: " << attrval.c_str());
			}
		}
	}
	else
	{
		LOG_ERROR("SET FILE ATTRINUTE FAILURE: ", "file not exits " << normal);
	}
	
	
	
	return ret;
}

int BerkelyDBFileSystem::set_node_attr(const string fpath, const string attrname, const string attrval)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	string attrkey;
	build_attr_key(normal, attrname, attrkey);

	int ret;
	Dbt data, key;
	data = Dbt();
	key = Dbt();

	key.set_data((void *)normal.c_str());
	key.set_size(normal.length() + 1);
	//key.set_data((void *)attrkey.c_str());
	//key.set_size(attrkey.length() + 1);

	if((ret = m_pNamespaceDb->get(NULL, &key, &data, 0)) != DB_NOTFOUND)
	{
		key.set_data((void *)attrkey.c_str());
		key.set_size(attrkey.length() + 1);
		
		data.set_data((void *)attrval.c_str());
		data.set_size(attrval.length() + 1);

		ret = m_pNamespaceDb->put(NULL, &key, &data, 0);
		//attrval = (char*)data.get_data();
		LOG_INFO("SET FILE ATTRINUTE SUCCESS: ", "fiel: " << normal.c_str() << " , attr : "<< attrname.c_str() << " value: " << attrval.c_str());
	}
	else
	{
		LOG_ERROR("SET FILE ATTRINUTE FAILURE: ", "file not exits " << normal);
	}
	
	return ret;
}

int BerkelyDBFileSystem::get_node_attr(const string fpath, const string attrname, string& attrval)
{
	string dirname;
	string normal;
	decode_fpath(fpath, dirname, normal);

	string attrkey;
	build_attr_key(normal, attrname, attrkey);

	int ret;
	Dbt data, key;
	data = Dbt();
	key = Dbt();

	key.set_data((void *)attrkey.c_str());
	key.set_size(attrkey.length() + 1);

	if((ret = m_pNamespaceDb->get(NULL, &key, &data, 0)) != DB_NOTFOUND)
	{
		attrval = (char*)data.get_data();
		LOG_INFO("GET FILE ATTRINUTE SUCCESS: ", "fiel: " << normal.c_str() << " , attr : "<< attrname.c_str() << " value: " << attrval.c_str());
	}
	else
	{
		LOG_ERROR("GET FILE ATTRINUTE FAILURE: ", "can not get fiel: " << normal << "attr: " << attrname);
	}
	
	return ret;
}

void BerkelyDBFileSystem::build_attr_key(const string fname, const string attrname, string& attrkey)
{
	attrkey = fname + NODE_ATTR_DELIM +attrname;
}

bool BerkelyDBFileSystem::is_node_attr(const string node, const string attrkey)
{
	string node_key_predix = node + NODE_ATTR_DELIM;
	unsigned int i;

	for(i = 0; i < node_key_predix.length(); i ++)
	{
		if( node_key_predix[i] != attrkey[i])
			break;
	}

	if( i != node_key_predix.length() )
		return false;

	return true;
}

void BerkelyDBFileSystem::scan_db_info()
{	
	int ret;
	Dbc *cursorp;
	Dbt data, key;
	m_pNamespaceDb->cursor(NULL, &cursorp, 0);
	data = Dbt();
	key = Dbt();
	cursorp->get(&key, &data, DB_FIRST);

	while ((ret = cursorp->get(&key, &data, DB_NEXT)) == 0)
	{
		printf("%s: %s\n", key.get_data(), data.get_data());
	}

}

bool BerkelyDBFileSystem::is_node_file(const string node)
{
	
	if(node[node.length() - 1] != '/'&& node.rfind(NODE_ATTR_DELIM) == -1)
		return true;
	return false;
}

