#include <string>
#include "BerkelyFileSystem.h"
#include "Master.h"
#include "log_load.h"

void  test_BerkelyFileSystem()
{
	printf("********************************************************************\n");
	printf("berkelyfilesystem test case begin\n");
	printf("********************************************************************\n\n");

	std::string m_base_dir = "./BerkelyDB";
	BerkelyDBFileSystem* fs = new BerkelyDBFileSystem(m_base_dir);
	fs->mkdir("/PictureTable/");
	fs->mkdir("/PictureTable/tables/");
	fs->mkdir("/PictureTable/servers/");
	fs->mkdir("/PictureTable/");
	
	AttrMap attrs;
	attrs["time"] = "2011/5/17";
	attrs["master-id"] = "1";
	fs->create_node("/PictureTable/master",attrs);
	fs->set_node_attr("/PictureTable/master","local", "192.168.2.105:40032");
	fs->set_node_attr("/PictureTable/master","time", "2011/5/18");
	attrs["table-id"] = "10";
	attrs["server-id"] = "20";
	fs->set_node_attr("/PictureTable/master",attrs);
	string val;
	fs->get_node_attr("/PictureTable/master","local",val);
	printf("local: %s\n", val.c_str());
	
	////////////////////////
	AttrKeyMap keys;
	AttrMap attrsget;
	keys.push_back("time");
	keys.push_back("local");
	keys.push_back("master-id");

	fs->get_node_attr("/PictureTable/master", keys, attrsget);

	/////////////////////////
	bool exits = fs->exits("/PictureTable/master");

	printf("/PictureTable/master, %s\n", exits? "exits":"not exits");

	exits = fs->exits("/PictureTable/m");

	printf("/PictureTable/m, %s\n", exits? "exits":"not exits");

	/////////////////////////////
	fs->set_node_attr("/PictureTable/", "name","picture");
	AttrMap dirattrs;

	fs->getList("/PictureTable/", dirattrs);

	//fs->del("/");
	

	fs->scan_db_info();
	printf("\n\n\n");
	printf("********************************************************************\n");
	printf("press any key for next test case\n");
	printf("********************************************************************\n\n");
	getchar();
}

void test_master()
{
	printf("********************************************************************\n");
	printf("master test case begin\n");
	printf("********************************************************************\n\n");

	Network::PTransmitter * pTransmitter = new Network::PTransmitter();	
	Master master(pTransmitter);
	Network::PInetAddr  tcpaddr ( "192.168.1.104",40032);
	AttrMap attrs;

	master.keepalive(0, tcpaddr);
	master.keepalive(0, tcpaddr);
	master.open(1,"/PictureTable/master",0x00008 ,attrs,0);
	master.open(2,"/PictureTable/master",0x00001 ,attrs,0);

	AttrKeyMap keys;
	keys.push_back("time");
	keys.push_back("local");
	keys.push_back("master-id");

	attrs["test"] = "helloworld";
	master.read(1,"",1,3,keys,0,false);
	master.write(1,"",1,attrs,0,false);

	master.lock(1,1,LOCK_EXCLUSIVE_MODE,false,0);
	master.lock(2,2,LOCK_EXCLUSIVE_MODE,false,0);


	master.close(1, 1, 0);
	
	attrs["time"] = "2011/5/17";
	attrs["master-id"] = "1";

	master.write(1, "/PictureTable/master/", 0, attrs,0,true);
	master.mkDir(1, "/PictureTable/master/replica/", 0);

	master.del(1, 1, 0);

	master.getlist(1, "/PictureTable/",0);

	BerkelyDBFileSystem * fs = master.getFS();

	fs->scan_db_info();

	printf("\n\n");
	printf("********************************************************************\n");
	printf("press any key for next test case\n");
	printf("********************************************************************\n\n");
	getchar();
}

void main()
{
	log_load();
	test_BerkelyFileSystem();
	test_master();
}