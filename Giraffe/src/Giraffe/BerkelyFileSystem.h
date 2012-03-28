#ifndef __GIRAFFE_BERKELY_FILESYSTEM_H__
#define __GIRAFFE_BERKELY_FILESYSTEM_H__

#include <db_cxx.h>

struct DirEntry{
	string name;
	bool isDir;
};

typedef std::map<string, string> AttrMap;
typedef std::vector<string> AttrKeyMap;
	
static const char NODE_ATTR_DELIM = 0x01;

class BerkelyDBFileSystem{
	public:
		BerkelyDBFileSystem(const string dbname,int verbose = 0);
		~BerkelyDBFileSystem();
		int mkdir(const string dir);
		string read(const string fpath);
		int write(const string fpath, const string context);
		int create(const string fpath, const string &attr);
		int getList(const string dir, AttrMap& attrs);
		int getChildren(const string dir, AttrKeyMap& attrs);
		int del(const string fpath);
		bool exits(const string fpath);

		int create_node(const string fname, AttrMap attrs);
		int get_node_attr(const string fpath, AttrKeyMap keys, AttrMap& attrs);
		int set_node_attr(const string fpath, AttrMap attrs);
		int set_node_attr(const string fpath, const string attrname, const string attrval);
		int get_node_attr(const string fpath, const string attrname, string& attrval);
		bool is_node_attr(const string node, const string attrkey);
		bool is_node_file(const string node);
		void scan_db_info();

		void build_attr_key(const string fname, const string attrname, string& attrkey);



			
		inline void decode_fpath(const string fpath, string &dir, string &normal)
		{
			if(fpath[0] != '/')  
			{
				dir += "/"; 
			}
			int pos = fpath.rfind("/");
			if(pos >= 0)
			{
				dir += fpath.substr(0, pos + 1);
			}
				
			normal = dir + fpath.substr(pos + 1, fpath.length());
		}

	private:
		Db  *m_pNamespaceDb;
		DbEnv *m_pDbEnv;
		string m_base_dir;
		int m_verbose;
};
#endif// __GIRAFFE_BERKELY_FILESYSTEM_H__
