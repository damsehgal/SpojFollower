//compile with g++ -std=c++11
#include <bits/stdc++.h>
using namespace std;
struct PROBLEM
{
	string name;
	string links;
	int users;
	double points;
};
bool compare (PROBLEM & p1 , PROBLEM & p2)
{
	return p1.users < p2.users;
}
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
bool checkRegexProb(string & str)
{
	return regex_match(str , std::regex("^\\s{0,}<td\\sclass=\"t.{0,}$"));
}
int getNum(string & str)
{
	int i;
	string temp;
	for (i = 0 ; str[i] != '>' ; i++);
	i++;	
	for(;str[i] != '<' ; i++)
		temp += str[i];
	return stoi(temp);
}
int getUsers(string & str)
{
	string url = "http://www.spoj.com/ranks/" + str + "/";
	string cmd = "curl " +url + " >" + str +".txt";
	system(cmd.c_str());
	string filename = str + ".txt";
	int Count = 0;
	ifstream myfile(filename.c_str());
	if (myfile)  
	{
		string line;
		while (getline( myfile, line ))  
		{
			if (checkRegexProb(line) )
			{
				Count ++;
				if(Count == 2)
				{
					myfile.close();
					string deleteFile = "rm -r " + filename;
					system(deleteFile.c_str());  
					return getNum(line);
				}			
			}	
		}
	}	
}
vector<PROBLEM> readFile(string & username)
{
	std::vector<PROBLEM> v;
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
				{
					PROBLEM p;
					p.name = temp;
					p.users = getUsers(p.name);
					while(p.users == 0)
						p.users = getUsers(p.name); // bug -> curl gives empty response
					v.push_back(p);
				}	
			}
		}
		myfile.close();
	}
	return v;

}
void makeLink(std::vector<PROBLEM> & v)
{
	//www.spoj.com/problems/ABA12D/
	for(int i = 0 ; i < v.size() ; i++)
		v[i].points = 80.0/(40.0+ v[i].users),
		v[i].links = "<a href =\" http:://www.spoj.com/problems/" + v[i].name +"\"> " + v[i].name +" "+ to_string(v[i].points) +"</a> <br>";
}
string makeHtml(std::vector<PROBLEM> & v)
{
	string list ;
	for (int i = 0 ; i < v.size() ; i++ )
		list += v[i].links;
	string HTML = 
	"<html><body>"+ list +"</body></html>";
	return HTML;
}
int main(int argc, char const *argv[])
{
	string username ;
	cin >> username ;
	savePage(username) ;
	std::vector<PROBLEM> v = readFile(username);
	sort(v.rbegin(), v.rend(),compare);
	makeLink(v);
	std::ofstream outfile ((username + ".html").c_str());
	outfile <<makeHtml(v);
	system(("rm -f "+ username + ".txt").c_str());
	system(("gnome-open " + username + ".html").c_str());	
	return 0;
}