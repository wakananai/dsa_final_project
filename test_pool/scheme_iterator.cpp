#include<iostream>
#include<string>
#include<map>
#include<list>
#include<vector>
#include<algorithm>
#include <sstream>
#include <regex>
#include <set>
#include "md5.h"
#include "account.h"

extern "C"{
//#include "wildcard.h"
#include "wildcard_nonre.h"

};



//typedef unsigned long long int MONEY;
//When you compile the file , make sure you include md5.cpp and -std=c++11
// in you command line.
map<string,account> ID_passwordMAP;
map<string,account>::iterator it_last_successful_login_ID ;
//string last_successful_login_ID;
using namespace std;
vector <set <string> > IDs(101);
int recommand,max_recommand;
long long nID;

//function for merge two records
bool compare (Record first, Record second)
{ return ( first.timestamp < second.timestamp ); }

//function for match wildcard
/*
bool match(string wilcard_input,string target){

	for(int i=0;i<wilcard_input.length();i++){
		if(wilcard_input[i]=='?')
			wilcard_input[i]='.';
		if(wilcard_input[i] == '*'){
			wilcard_input[i] = '.';
			wilcard_input.insert(i+1,"*");
			i++;
		}
	}
	regex e(wilcard_input);
	return regex_match(target,e);
}
*/
//functions for recommend ID
typedef struct
{
	string ID;
	int score;
}IDscore;
bool cmp(IDscore c,IDscore d)
{
	if(c.score>d.score) return false;
	else if(c.score==d.score)
	{
		if(c.ID>d.ID) return false;
		else return true;
	}
	else return true;
}
bool found(string s)
{
	int m=s.length();
	bool ans=false;
	set<string>:: iterator it;
	for(it=IDs[m].begin();it!=IDs[m].end();it++)
	{
		if(*it==s)
		{
			ans=true;
			break;
		}
	}
	return ans;
}
int score_compute(string s,string t,int minlength)
{
	int ans=0;
	if(s.length()>minlength)
	{
		int n=s.length()-minlength;
		ans+=n*(n+1)/2;
	}
	if(t.length()>minlength)
	{
		int n=t.length()-minlength;
		ans+=n*(n+1)/2;
	}
	for(int i=0;i<minlength;i++)
	{
		if(s[i]!=t[i]) ans+=(minlength-i);
	}
	return ans;
}
void ExhaustiveAttack(string s,int score,int* changed,int* recommand,int x,int dlen,int y)
{
	if(*recommand==max_recommand) return;
	if(score==0)
	{
		if(y==1)
		{
			if(!found(s))
			{
				cout<<s;
				(*recommand)++;
				if((*recommand)<max_recommand) cout<<",";
			}
		}
		else
		{
			if(found(s))
			{
				cout<<s;
				(*recommand)++;
				if((*recommand)<max_recommand) cout<<",";
			}
		}

		return;
	}
	string t=s;
	int m=s.length();
	if(x==0)
	{
		//cout<<t<<" "<<dlen;
		if(dlen==0)
		{
			//cout<<s;
			if(y==1)
			{
				if(!found(s))
				{
					cout<<s;
					(*recommand)++;
					if((*recommand)<max_recommand) cout<<",";
				}
			}
			else
			{
				if(found(s))
				{
					cout<<s;
					(*recommand)++;
					if((*recommand)<max_recommand) cout<<",";
				}
			}
			return;
		}
		if(m==1) return;
		t.pop_back();
		//cout<<t<<endl;
		//cout<<t<<endl;
		ExhaustiveAttack(t,score,changed,recommand,0,dlen-1,y);
		t.push_back(s[m-1]);
		//cout<<t<<endl;
	}
	if(x==1)
	{
		int n=sqrt(score*2);
		if(score*2==n*(n+1)) ExhaustiveAttack(t,score,changed,recommand,0,n,y);
		for(int i=m-score;i<m;i++)
		{
			if(changed[i]==1) continue;
			int j;
			for(j='0';j<='9';j++)
			{
				if(j>=s[i]) break;
				t[i]=j;
				changed[i]=1;
				//cout<<t<<endl;
				ExhaustiveAttack(t,score-(m-i),changed,recommand,1,0,y);
				t[i]=s[i];
				changed[i]=0;
			}
			for(j='A';j<s[i],j<='Z';j++)
			{
				if(j>=s[i]) break;
				t[i]=j;
				changed[i]=1;
				//cout<<t<<endl;
				ExhaustiveAttack(t,score-(m-i),changed,recommand,1,0,y);
				t[i]=s[i];
				changed[i]=0;
			}
			for(j='a';j<s[i],j<='z';j++)
			{
				if(j>=s[i]) break;
				t[i]=j;
				changed[i]=1;
				//cout<<t<<endl;
				ExhaustiveAttack(t,score-(m-i),changed,recommand,1,0,y);
				t[i]=s[i];
				changed[i]=0;
			}
		}
		n=sqrt(score*2);
		//cout<<"n*(n+1)="<<2*score<<endl;
		//cout<<n<<" "<<score<<endl;
		if(score*2==n*(n+1)) ExhaustiveAttack(t,score,changed,recommand,2,n,y);
		for(int i=m-score;i<m;i++)
		{
			int j;
			for(j=s[i]+1;j<='9';j++)
			{
				t[i]=j;
				changed[i]=1;
				//cout<<t<<endl;
				ExhaustiveAttack(t,score-(m-i),changed,recommand,1,0,y);
				t[i]=s[i];
				changed[i]=0;
			}
			for(j=s[i]+1;j<='Z';j++)
			{
				t[i]=j;
				changed[i]=1;
				//cout<<t<<endl;
				ExhaustiveAttack(t,score-(m-i),changed,recommand,1,0,y);
				t[i]=s[i];
				changed[i]=0;
			}
			for(j=s[i]+1;j<='z';j++)
			{
				t[i]=j;
				changed[i]=1;
				//cout<<t<<endl;
				ExhaustiveAttack(t,score-(m-i),changed,recommand,1,0,y);
				t[i]=s[i];
				changed[i]=0;
			}
		}
	}
	if(x==2)
	{
		//cout<<t<<endl;
		if(dlen==0)
		{
			if(y==1)
			{
				if(!found(s))
				{
					cout<<s;
					(*recommand)++;
					if((*recommand)<max_recommand) cout<<",";
				}
			}
			else
			{
				if(found(s))
				{
					cout<<s;
					(*recommand)++;
					if((*recommand)<max_recommand) cout<<",";
				}
			}
			return;
		}
		int j;
		for(j='0';j<='9';j++)
		{
			t.push_back(j);
			//cout<<t<<endl;
			//cout<<t<<endl;
			ExhaustiveAttack(t,score,changed,recommand,2,dlen-1,y);
			t.pop_back();
			//cout<<t<<endl;
		}
		for(j='A';j<='Z';j++)
		{
			t.push_back(j);
			//cout<<t<<endl;
			ExhaustiveAttack(t,score,changed,recommand,2,dlen-1,y);
			t.pop_back();
			//cout<<t<<endl;
		}
		for(j='a';j<='z';j++)
		{

			t.push_back(j);
			//cout<<t<<endl;
			//cout<<t<<endl;
			ExhaustiveAttack(t,score,changed,recommand,2,dlen-1,y);
			t.pop_back();
			//cout<<t<<endl;
		}
	}
}

//Function that are not bound to last successful login


void login(string ID,string password){
	string encoded = md5(password);

	if(ID_passwordMAP.find(ID) == ID_passwordMAP.end()){
		cout << "ID " << ID << " not found" << endl;
		return;
	}

	if(ID_passwordMAP[ID].password_encoded == encoded){
		cout << "success" << endl;
		it_last_successful_login_ID = ID_passwordMAP.find(ID);
		return;
	}

	else{
		cout << "wrong password" << endl;
		return;
	}

}


void create(string ID,string password){

	string encoded = md5(password);
	account tmp(ID,encoded);
	std::pair<std::map<string,account>::iterator,bool> ret;
	ret = ID_passwordMAP.insert(pair<string ,account>(ID,tmp));
	if(ret.second == true)
	{
		cout << "success" << endl;
		int m=ID.length();
		IDs[m].insert(ID);
		nID++;
	}
	else{
		cout << "ID "<<  ID << " exists, ";
		recommand=0;
		max_recommand=10;
		int score=1;
		while(1)
		{
			if(recommand==max_recommand) break;
			int l=ID.length();
			int changed[l];
			memset(changed,0,sizeof(changed));
			ExhaustiveAttack(ID,score,changed,&recommand,1,0,1);
			score++;
		}
		cout<<endl;
	}
	return ;
}

void _delete(string& ID,string password){
	string encoded = md5(password);

	if(ID_passwordMAP.find(ID) == ID_passwordMAP.end()){
		cout << "ID " << ID << " not found" << endl;
		return;
	}

	if(ID_passwordMAP[ID].password_encoded == encoded){
		int m=ID.length();
		ID_passwordMAP.erase(ID);
		set <string>:: iterator it;
		for(it=IDs[m].begin();it!=IDs[m].end();it++)
		{
			if(*it==ID)
			{
				IDs[m].erase(it);
				break;
			}
		}
		cout << "success" << endl;
		nID--;
		return;
	}

	else{
		cout << "wrong password" << endl;
		return;
	}
}
void merge(const string& ID1,const string& password1,const string& ID2,const string& password2){
	if(ID_passwordMAP.find(ID1) == ID_passwordMAP.end()){
		cout << "ID " << ID1 << " not found" << endl;
		return;
	}
	if(ID_passwordMAP.find(ID2) == ID_passwordMAP.end()){
		cout << "ID " << ID2 << " not found" << endl;
		return;
	}
	string encoded1 = md5(password1);
	string encoded2 = md5(password2);
	if(ID_passwordMAP[ID1].password_encoded != encoded1){
		cout << "wrong password1" << endl;
		return ;
	}
	if(ID_passwordMAP[ID2].password_encoded != encoded2){
		cout << "wrong password2" << endl;
		return ;
	}
	// add the money of ID2 to ID1
	ID_passwordMAP[ID1].balance += ID_passwordMAP[ID2].balance;

	//change others' transfer record about ID2
	list <Record>:: iterator its;
	list <Record>:: iterator its2;

	for(its=ID_passwordMAP[ID2].transfer_record.begin();its!=ID_passwordMAP[ID2].transfer_record.end();its++)
		{
			auto changeID = its->target_ID;
			if(ID_passwordMAP.find(changeID) != ID_passwordMAP.end()){
			
				for(its2=ID_passwordMAP[changeID].transfer_record.begin();its2!=ID_passwordMAP[changeID].transfer_record.end();its2++){
				
				if((*its2).timestamp == (*its).timestamp){
				
					(*its2).target_ID = ID1;
					break;
					}
				}
			}
			
		}

	//merge ID2's transfer record into ID1
	ID_passwordMAP[ID1].transfer_record.merge(ID_passwordMAP[ID2].transfer_record,compare);

	// delete ID2
	int m=ID2.length();
		ID_passwordMAP.erase(ID2);
		set <string>:: iterator it;
		for(it=IDs[m].begin();it!=IDs[m].end();it++)
		{
			if(*it==ID2)
			{
				IDs[m].erase(it);
				break;
			}
		}

	cout << "success, " << ID1 <<" has " << ID_passwordMAP[ID1].balance <<" dollars" << endl;
	nID--;

	return;
}
//Function that are bound to last successful login
void transfer(const string& ID, unsigned long long int num  , long long int timestamp){
	if(ID_passwordMAP.find(ID) == ID_passwordMAP.end()){
		cout << "ID " << ID << " not found, ";
		IDscore scores[nID];
		int x=0;
		for(int i=0;i<=100;i++)
		{
			set <string> :: iterator it;
			for(it=IDs[i].begin();it!=IDs[i].end();it++)
			{
				string t=*it;
				int mins=ID.length()<t.length()?ID.length():t.length();
				scores[x].ID=t;
				scores[x++].score=score_compute(ID,t,mins);
			}
		}
		sort(scores,scores+nID,cmp);
		max_recommand=nID>10?10:nID;
		for(int i=0;i<max_recommand;i++)
		{
			cout<<scores[i].ID;
			if(i!=max_recommand-1)cout<<",";
		}
		cout<<endl;
		return;
	}
	else if( it_last_successful_login_ID->second.balance < num ){
		cout << "fail, " << it_last_successful_login_ID->second.balance  << " dollars only in current account" << endl;
		return;
	}
	else{
		it_last_successful_login_ID->second.add_record(timestamp,false,ID,num);
		it_last_successful_login_ID->second.balance -= num;
		ID_passwordMAP[ID].add_record(timestamp,true,it_last_successful_login_ID->second.ID,num);
		ID_passwordMAP[ID].balance += num;
		cout << "success, " << it_last_successful_login_ID->second.balance << " dollars left in current account" << endl;
		return;
	}


}

void find(string& s_wildcard){
	vector<string> tmp;
	for(auto it = ID_passwordMAP.begin(); it !=ID_passwordMAP.end() ;++it){
		if( match(s_wildcard.c_str(), it -> second.ID.c_str() ) )
			tmp.push_back(it -> second.ID);
	}
	sort(tmp.begin(),tmp.end());

	stringstream ss;

	for(auto it = tmp.begin() ; it!=tmp.end() ; ++it){ //	for(auto it = tmp.begin()+1 ; it!=tmp.end() ; ++it){
														 //	~~~~~~~~~~~~~~~~~~~~~~~~~~↑ ?
		if(*it != it_last_successful_login_ID->second.ID)
			ss  << *it << "," ;
	}
	string ans;
	ss >> ans;
	ans[ans.length()-1] = '\0';
	cout << ans << endl;
	return;
}

void search(const string& ID){
	//if ( it_last_successful_login_ID->second.transfer_record.empty() ){
	//	cout << "no record" << endl;
	//}
	//else{
		it_last_successful_login_ID->second.printRecord(ID);
	//}
	return;
}

//extend operation
void print_all_account(void){
	for(auto it = ID_passwordMAP.begin(); it!=ID_passwordMAP.end();it++)
		cout  << it->first << " " << it->second.balance << endl;
	return;
}

int main(){
	//freopen("test_in_small.txt","r",stdin);
	long long int timestamp = 1;
	//last_successful_login_ID.clear();
	ID_passwordMAP.clear();
	it_last_successful_login_ID = ID_passwordMAP.end();
	IDs.clear();
	nID=0;

	
	string cmd;
	while(cin >> cmd){
		//Operation that are not bound to last successful login
		if(cmd == "login"){
			string ID ,password;
			cin >> ID >> password;
			login(ID ,password);
		}
		if(cmd == "create"){
			string ID,password;
			cin >> ID >> password;
			create(ID,password);
		}
		if(cmd == "delete"){
			string ID,password;
			cin >> ID >> password;
			_delete(ID,password);
		}
		if(cmd == "merge"){
			string ID1,password1,ID2,password2;
			cin >> ID1 >> password1 >> ID2 >> password2;
			merge(ID1,password1,ID2,password2);
		}


		//Operation that are bound to last successful login

		//notice : maybe we can use iterator (while "login") than the global last_successful_login_ID
		// it may reduce the time cost in the finding
		if(cmd == "deposit" ){
			unsigned long long int num;
			cin >> num;
			it_last_successful_login_ID->second.deposit(num);
		}
		if(cmd == "withdraw"){
			unsigned long long int num;
			cin >> num;
			it_last_successful_login_ID->second.withdraw(num);
		}
		if(cmd == "transfer"){
			string target_ID;
			unsigned long long int num;
			cin >> target_ID >> num;
			transfer( target_ID , num,timestamp);
		}
		if(cmd == "find"){
			string s_wildcard;
			cin >> s_wildcard;
			find(s_wildcard);
		}
		if(cmd == "search"){
			string ID;
			cin >> ID;
			search(ID);
		}
		//extend option
		if(cmd == "print")
			print_all_account();
		if(cmd == "exit")
			break;

		//notice : that might have problem , because the time might be increase in transfer function
		//the time passby
		timestamp++;
	}
	return 0;

}

