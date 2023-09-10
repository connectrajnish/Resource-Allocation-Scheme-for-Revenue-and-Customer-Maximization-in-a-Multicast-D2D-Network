#include <iostream>
#include <bits/stdc++.h>
using namespace std;


class user{
public:
    unsigned int profit=0, cqi=1, data_request=0, rb=0;
    string name;
    user(){}
    user(string name, unsigned int profit, unsigned int cqi, unsigned int data_request);
};

user::user(string name, unsigned int profit, unsigned int cqi, unsigned int data_request){
    this->name=name;
    this->profit=profit;
    this->cqi=1;
    this->data_request=0;
    rb=ceil(data_request/cqi);
}

class cellular_user{
public:
    user *self=new user;
    string name;
    vector<user*> children;
    unsigned int cqi=1;
    cellular_user(){}
    void add_du(user* child){children.push_back(child);}
};

class baseStation{
private:
    vector<cellular_user*> children;
    unsigned long long int rb=0;
public:
    baseStation(){}
    baseStation(unsigned long long int rb);
    void add_cellular_user(cellular_user* cu){children.push_back(cu);}
    unsigned long long int number_of_rb(){return rb;}

};

baseStation::baseStation(unsigned long long int rb){
    while(rb<0){
        cout<<"Invalid number\nEnter valid number of resource blocks: ";
        cin>>rb;
    }
    this->rb=rb;
}

void helper(vector<user*> &users, vector<user*> &result, vector<user*> &temp_user, int i,
            int curr_profit, int &ans_profit, int curr_rb, int &ans_rb, int total_rb){
        bool flag=false;
        if(i>= users.size() || (curr_rb > total_rb)){
            if(curr_rb > total_rb && temp_user.size()>0){
                user* temp=temp_user.back();
                temp_user.pop_back();
                flag=true;
                curr_profit-=temp->profit;
                curr_rb-=temp->rb;
            }

            if(temp_user.size()>result.size()){
                result=temp_user;
                ans_profit=curr_profit;
                ans_rb=curr_rb;
            }
            else if(temp_user.size()==result.size()){
                if((curr_profit > ans_profit) || ((curr_profit == ans_profit) && (curr_rb < ans_rb))){
                    result=temp_user;
                    ans_profit=curr_profit;
                    ans_rb=curr_rb;
                }
            }
            if(flag)
                temp_user.push_back(NULL);
            return;
        }

        helper(users,result,temp_user,i+1,curr_profit,ans_profit,curr_rb,ans_rb,total_rb);
        temp_user.push_back(users[i]);
        helper(users,result,temp_user,i+1,curr_profit+users[i]->profit,ans_profit,curr_rb+users[i]->rb,ans_rb,total_rb);            
        temp_user.pop_back();
}

vector<user*> distribute_rb(vector<user*> &users, int total_rb){
    vector<user*> result, temp_user;
    int ans_rb=0,ans_profit=0;
  //helper(users,result,temp_user,i,curr_profit,ans_profit,curr_rb,ans_rb,total_rb);
    helper(users,result,temp_user,0,0,ans_profit,0,ans_rb,total_rb);
            
    return result;
}

bool comparator(user* first, user* second){
    if(first->rb == second->rb)
        return first->profit > second->profit;
    
    return first->rb < second->rb;
}

// Finding maximum numbers of users

// vector<user*> distribute_rb(vector<user*> &users, unsigned long long int total_rb){
//     vector<user*> result;
//     unsigned int rb=0,i,last=-1,profit=0;
//     sort(users.begin(),users.end(),comparator);
//     for(i=0; i<users.size(); i++){
//         if((rb+users[i]->rb)<=total_rb){
//             result.push_back(users[i]);
//             last=i;
//             rb=rb+users[i]->rb;
//         }
//         else if(last!=-1){
//             int temp_profit=profit, temp_rb=rb;
//             temp_profit-=users[last]->profit;
//             temp_rb-=users[last]->rb;
//             if((temp_rb+users[i]->rb <= total_rb) && temp_profit+users[i]->profit > profit){
//                 result.pop_back();
//                 result.push_back(users[i]);
//                 last=i;
//                 rb=temp_rb+users[i]->rb;
//                 profit=temp_profit+users[i]->profit;
//             }
//         }
//     }
//     return result;
// }



int main()
{
    vector<user*> users;
    //creating basestation detail
    unsigned long long int rb=0;
    cout<<"Enter total number of resource blocks available at the basestation: ";
    cin>>rb;
    baseStation *bs= new baseStation(rb);

    //cellular user info
    unsigned int number_of_cu=0;
    cout<<"Enter number of cellular users: ";
    cin>>number_of_cu;
    for(int i=0; i<number_of_cu; i++){
        cellular_user* cu= new cellular_user;
        cu->name="CU"+to_string(i+1);
        cout<<cu->name<<endl;

        //cu info for self use
        cu->self->name=cu->name;

        cout<<"Data Request of this CU for self use: ";
        cin>>cu->self->data_request;

        cout<<"Profit: ";
        cin>>cu->self->profit;

        cout<<"CQI (cqi between this cu & basestation): ";
        cin>>cu->cqi;
        cu->self->cqi=cu->cqi;

        cu->self->rb=ceil((double)cu->self->data_request/cu->self->cqi);
        users.push_back(cu->self);

        unsigned int number_of_du=0;
        cout<<"Enter number of DU connected to "<<cu->name<<" : ";
        cin>>number_of_du;


        //DU info
        for(int j=0 ;j<number_of_du; j++){
            user* du= new user;
            du->name="DU"+to_string(j+1)+" of "+cu->name;
            cout<<du->name<<endl;

            cout<<"Data Request of this DU: ";
            cin>>du->data_request;

            cout<<"CQI: ";
            unsigned int temp_cqi=1;
            cin>>temp_cqi;
            du->cqi=min(temp_cqi,cu->cqi);

            cout<<"Profit: ";
            cin>>du->profit;
            
            du->rb=ceil((double)du->data_request/du->cqi);

            cu->children.push_back(du);
            users.push_back(du);
        }

        bs->add_cellular_user(cu);
    }
    
    vector<user*> result=distribute_rb(users,bs->number_of_rb());
    unsigned long long int total_rb_distributed=0, total_profit=0;
    cout<<endl<<"******************************* Result *******************************"<<endl;
    for(int i=0; i<result.size(); i++){
        total_rb_distributed+=result[i]->rb;
        total_profit+=result[i]->profit;
        cout<<result[i]->name<<"\t\t";
        if(result[i]->name[0]=='C')cout<<"\t";
        cout<<result[i]->rb<<"\t\t"<<result[i]->profit<<endl;
    }
    cout<<endl;
    cout<<"Total RB distributed: "<<total_rb_distributed<<endl;
    cout<<"Total profit: "<<total_profit<<endl;
    return 0;
}