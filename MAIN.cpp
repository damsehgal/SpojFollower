//compile with g++ -std=c++11
#include <bits/stdc++.h>
using namespace std;
void savePage(string & username)
{
	string url = "http://www.spoj.com/users/" + username +"/";
	string cmd = "curl " + url + " >" + username + ".txt"; 
	system (cmd.c_str());
}
bool checkRegex(string & str)
{
	return regex_match (str , std::regex("^\\s{0,}<td\\salign.{0,}$"));
}
vector<string> readFile(string & username)
{
	std::vector<string> v;
	string filename = username + ".txt";
	ifstream myfile(filename.c_str());
	if (myfile)  
	{
		string line;
		while (getline( myfile, line ))  
		{
			if (checkRegex(line) )
			{
				string temp;
				size_t found = line.find("/status/");
				for(size_t i = found + 8 ; line[i]!=',' ; i++)
					temp += line[i];
				if(temp != "")
					v.push_back(temp);
			}
		}
		myfile.close();
	}
	return v;

}
void printVector (std::vector<string>& v)
{
	for (int i = 0 ;i < v.size() ;i++)
		cout << v[i] << "\n";
}

int main(int argc, char const *argv[])
{
	string username ;
	cin >> username ;
	savePage(username) ;
	std::vector<string> v = readFile(username);
	printVector(v);
	return 0;
}