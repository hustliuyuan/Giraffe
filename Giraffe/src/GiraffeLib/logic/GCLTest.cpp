#include <stdio.h>
#include "Base/GHeader.h"
#include "Session.h"
#include "logic/GCLCommLogger.h"

void test_open(Session *session, string filename)
{
	cout << "[test:open file] " << "[filename]: " << filename << " [/test]"<< endl;
	session->open(filename);
}

void test_read(Session *session, string filename)
{
	cout << "[test:read file] " << "[filename]: " << filename << " [/test]"<< endl;
	string content = session->getData(filename);
	cout << "[test:read file] " << "[filename]: " << filename << " [content]: "<< content <<" [/test]"<< endl;
}

void test_write(Session *session, string filename, string content)
{
	//cout << "[test:write file] " << "[filename]: " << filename << " [content]: "<< content <<" [/test]"<< endl;
	if(session->setData(filename, content))
	{
		cout << "[test:write file] " << "[filename]: " << filename << " [result]: "<< "write success" <<" [/test]"<< endl;
	}
	else
	{
		cout << "[test:write file] " << "[filename]: " << filename << " [result]: "<< "write fail, file is locked by someone" <<" [/test]"<< endl;
	}
}

void test_mkdir(Session *session, string dirname)
{
	cout << "[test:make dir] " << "[dirname]: " << dirname << " [/test]"<< endl;
	if(session->mkdir(dirname))
	{
		cout << "[test:make dir] " << "[dirname]: " << dirname <<" [result:]"<< " success"<< " [/test]"<< endl;
	}
}

void test_get_children(Session *session, string dirname)
{
	cout << "[test:get dir children] " << "[dirname]: " << dirname << " [/test]"<< endl;
	vector<string> children = session->getChildren(dirname);

	vector<string>::iterator it = children.begin();

	for( ; it != children.end(); it ++)
	{
		cout << "[GRFild] " << *it << " [/GRFild]  " << endl;
	}
	cout << "[/test]" <<endl;
}

void test_lock(Session *session, string filename)
{
	cout << "[test:lock] " << "[lock file]: " << filename << " [/test]"<< endl;
	if(session->lock(filename))
	{
		cout << "[test:lock] " << "[result]: " << "lock success" << " [/test]"<< endl;
	}
	else
	{
		cout << "[test:lock] " << "[result]: " << "lock fail" << " [/test]"<< endl;
	}
}

void test_release(Session *session, string filename)
{
	cout << "[test:release lock] " << "[filename]: " << filename << " [/test]"<< endl;
	session->release(filename);
}

void test_exits(Session *session, string filename)
{
	cout << "[test:exits] " << "[filename]: " << filename << " [/test]"<< endl;
	if(session->exits(filename))
	{
		cout << "[test:exits] " << "[filename]: " << filename << "[result]: exits" <<" [/test]"<< endl;
	}
	else
	{
		cout << "[test:exits] " << "[filename]: " << filename << "[result]: not exits" <<" [/test]"<< endl;
	}
}

void test_close(Session *session, string filename)
{
	cout << "[test:close file] " << "[filename]: " << filename << " [/test]"<< endl;
	session->close(filename);
}

void test_sleep()
{
	cout << "[test:sleep] 1000ms [/test]" << endl;
	PUtils::sleep(1000);
}

vector<string> parserContent(string content)
{ 
	unsigned int i = 0;
	int begin = 0, len = 0;
	bool new_term = true;
	char ch;
	vector<string> terms;
	if(content[0] == '#')
	{
		terms.push_back(content);
		return terms;
	}
		
 	for( ; i < content.length() ; i++)
	{
		ch = content[i];
		if(ch == ' '||ch == '\0'|| ch == '\t' || ch == '\n'|| ch == EOF)
		{
			if(new_term)
			{
				begin = i + 1;
			}
			else
			{
				string term = content.substr(begin, len);
				terms.push_back(term);
				new_term = true;
				begin = i + 1;
				len = 0;
			}
		}
		else
		{
			len ++;
			new_term = false;
		}
	}
	if(len != 0) 
	{
		string term = content.substr(begin, len);
		terms.push_back(term);
	}
	if(terms.size() == 0)
		terms.push_back("\n");
	return terms;
}

void dispatch(Session* session, vector<string> terms)
{
	vector<string>::iterator it = terms.begin();
	string command = *it;

	if(command == "open")
	{
		if((++ it) != terms.end())
		{
			string filename = *it;
			test_open(session,filename);
		}
	}
	else if(command == "read")
	{
		if((++ it) != terms.end())
		{
			string filename = *it;
			test_read(session,filename);
		}
	}
	else if(command == "write")
	{
		if((++ it) != terms.end())
		{
			string filename = *it;
			if((++ it) != terms.end())
			{
				string content = *it;
				test_write(session, filename, content);
			}
		}
	}
	else if(command == "lock")
	{
		if((++ it) != terms.end())
		{
			string filename = *it;
			test_lock(session,filename);
		}
	}
	else if(command == "unlock")
	{
		if((++ it) != terms.end())
		{
			string filename = *it;
			test_release(session,filename);
		}
	}
	else if(command == "exits")
	{
		if((++ it) != terms.end())
		{
			string dirname = *it;
			test_exits(session,dirname);
		}
	}
	else if(command == "mkdir")
	{
		if((++ it) != terms.end())
		{
			string dirname = *it;
			test_mkdir(session,dirname);
		}
	}
	else if(command == "getchildren")
	{
		if((++ it) != terms.end())
		{
			string dirname = *it;
			test_get_children(session,dirname);
		}
	}
	else if(command == "close")
	{
		if((++ it) != terms.end())
		{
			string filename = *it;
			test_close(session,filename);
		}
	}
	else if(command == "sleep")
	{
		test_sleep();
	}
	else
	{
		//cout << command << endl;
	}
}

int  main()
{
	GCLCommLogger::setCommConfig();
	PSettings & settings = PSettings::getInstance();		
	string test_file_path = settings.getConfig("client-test-file-path").m_value.strvalue;
	Session *session = new Session();
	session->connect();
	FILE *test_file = fopen(test_file_path.c_str(),"r");

	if(test_file == NULL) 
	{
		cout << "test file not found!!" << endl;
		session->sessionTerminate();	
		return 0;
	}
	char line[100];
	int i = 0;
	long lines = 0;
	S64 begin = PUtils::localSeconds();
	while(!feof(test_file))
	{
		if( fgets( line, 100, test_file))
		{
			lines ++;
			dispatch(session, parserContent(line));
			cout << "lines: " << lines << endl;
		S64 end = PUtils::localSeconds();
		cout << "operations per second:" << lines/(double)(end - begin) << endl;
		}
	}
	S64 end = PUtils::localSeconds();
	cout << "lines: " << lines << endl;
	cout << "interval time:" << end - begin << endl;
	cout << "operations per second:" << lines/(double)(end - begin) << endl;
	session->sessionTerminate();	
	return 0;
}

