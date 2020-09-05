//
//  ChatTracker.cpp
//  PROJECT 4 FINAL
//
//  Created by Sanchit Agarwal on 6/2/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#include "ChatTracker.h"

#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <utility>
#include <iostream>

using namespace std;

class Head;
class User;
class Chat;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////**********ChatTrackerImpl Class**************
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class ChatTrackerImpl
{
  public:
    
    //Constructor
    ChatTrackerImpl(int maxBuckets):m_Ctable(maxBuckets) , m_Utable(maxBuckets) , m_buckets(maxBuckets)
    {
    }
    
    void join(string user, string chat);
    
    int terminate(string chat);
    
    int contribute(string user);
    
    int leave(string user, string chat);
    
    int leave(string user);
    
    int targetRow(const string& name);
    
    ~ChatTrackerImpl();

  private:
    
    vector<list<User*>> m_Utable;
    
    vector<list<Chat*>> m_Ctable;
    
    int m_buckets;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////**********Chat Class**************
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class Chat
{
public:

    //CONSTRUCTOR
    Chat(string name) : m_name(name) , m_contributions(0) {};

    string getName() { return m_name; }

    /* increases the Contributions made <<generally by 1>> */
    void updateContributions() { ++m_contributions;}

    /* returns the total number of contributions made to the chat */
    int getContributions()  { return m_contributions; }

    /* add User */
    void addUser(User* user)
    {
        //Add the user to the beginning of the user forward_list
        m_userlist.insert_after(m_userlist.before_begin(), user);
    }

    int terminateChat();

    //DESTRUCTOR
    ~Chat()
    {}


private:
    string m_name;  //Name of the chat

    int m_contributions;    //Contributions made to the chat

    forward_list<User*> m_userlist;  //List of all the Users in the chat
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////**********User Class**************
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class User
{
public:
    //CONSTRUCTOR
    User(string name) : m_name(name) {};


    string getName() { return m_name; }

    /* makes a contribution to the current chat */
    /* returns the total number of contributions made to that chat */
    int makeContribution();

    /* leave Current chat */
    /* returns the total number of Contributions made to the current chat */
    int leaveCurrentChat();

    /* leave given chat */
    /* returns the total number of Contributions made to the given chat */
    int leaveChat(Chat* tobeLeft);


    /* add chat */
    void addChat(Chat* tobeAdded , int contributions = 0)
    {
        m_chatlist.push_back(make_pair(tobeAdded,contributions));
    }


    /* finds a chat based on its name */
    /* it only is called once in join */
    /* if a chat exists it is deleted deleted */
    Chat* fAdChat(Chat* t_chat , int& contributions);

    ~User()
    {};


private:

    string m_name;
    list<pair<Chat*,int>> m_chatlist;  //List of all the Chats a User is in

};


/////////////////////////////////////////////////////////////////////////////////////////////////
//***************CLASS IMPLEMENTATIONS//////////
/////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**********CHAT/USER IMPLEMENTATION**************
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int User::makeContribution()
{
    //if chatlist is empty return 0
    if(m_chatlist.empty())
        return 0;
    
    //increase contributions to current chat by 1
    m_chatlist.back().second += 1;

    //update contributions in the chat
    (m_chatlist.back().first)->updateContributions();

    //return contributions made
    return m_chatlist.back().second;
}


int User::leaveCurrentChat()
{
    //if chalist is empty return -1;
    if(m_chatlist.empty())
        return -1;

    //save the contributions to the chat
    int contributions = m_chatlist.back().second;

    //erase the current chat
    list<pair<Chat*,int>>::iterator i = m_chatlist.end();
    i--;    //points to last element
    m_chatlist.erase(i);

    //return contributions made to it
    return contributions;
}


int User::leaveChat(Chat* tobeLeft)
{
    //Set the iterator to the beggining of the chatlist
    list<pair<Chat*,int>>::iterator i = m_chatlist.begin();

    //try to find the chat
    for(;i != m_chatlist.end(); i++)
    {
        //if found
        if((*i).first == tobeLeft)
        {
            //save the contributions to the chat
            int contributions = (*i).second;
            
            //erase the chat
            i = m_chatlist.erase(i);

            return contributions;
        }
    }

    //if no such chat
    return -1;
}

//FIND AND DELETE CHAT FROM USERLIST
//Description : - This function is utilized while joining a chat which the user is already present it .
Chat* User::fAdChat(Chat* t_chat, int& contributions)
{
    //Set the iterator to the beggining of the chatlist
    list<pair<Chat*,int>>::iterator i = m_chatlist.begin();

    for(;i != m_chatlist.end(); i++)
    {
        Chat* chat = (*i).first;
        //if found
        if(t_chat == chat)
        {
            //save the contributions to the chat
            int contri = (*i).second;
              
            //ERASE the chat from user list
            i = m_chatlist.erase(i);

            contributions = contri;
            return chat;
        }
    }
    return nullptr;
}

//Terminates the chat and deletes the chat from all users
int Chat::terminateChat()
{
    //Set the iterator to the beggining of the userlist
    forward_list<User*>::iterator i = m_userlist.begin();
    
    for(;i != m_userlist.end(); i++)
    {
        //remove the chat from user's chatlist
        (*i)->leaveChat(this);
    }
    return m_contributions;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**********CHAT TRACKER IMPL IMPLEMENTATION**************
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//************THE HASH FUNCTION*****************
int ChatTrackerImpl::targetRow(const string& name)
{
    unsigned int hash_num = std::hash<string>()(name);
    return hash_num % m_buckets;
}


//************THE JOIN FUNCTION*****************
void ChatTrackerImpl::join(string user, string chat)
{
    
    Chat* chatToJoin = nullptr;
    User* UserJoining = nullptr;
    
    //Search the chat in the Hash Table
    int chat_row = targetRow(chat);

    list<Chat*>::iterator chat_itr = m_Ctable[chat_row].begin();

    for(; chat_itr != m_Ctable[chat_row].end(); chat_itr++)
    {
        if(chat == (*chat_itr)->getName())
        {
            chatToJoin = *chat_itr;
        }
    }
    
    //Search the user in the Hash Table
    int user_row = targetRow(user);

    list<User*>::iterator user_itr = m_Utable[user_row].begin();

    for(; user_itr != m_Utable[user_row].end(); user_itr++)
    {
        if(user == (*user_itr)->getName())
        {
            UserJoining = *user_itr;
        }
    }
    
    //if neither chat nor user is new
    
    if(UserJoining != nullptr && chatToJoin != nullptr)
    {
        //to restore the contributions of a chat being joined again
        int ctbs;
        
        //Look if the user is already in the chat or not
        Chat* found = UserJoining->fAdChat(chatToJoin,ctbs);

        //if yes make it the current chat
        if(found != nullptr)
        {
            //when added contributions are PRESERVED
            UserJoining->addChat(found,ctbs);
            return;
        }
    }
    
    //if new user
    if(UserJoining == nullptr)
    {
        m_Utable[user_row].push_back(new User(user));
        UserJoining = m_Utable[user_row].back();
    }
    
    //if new chat
    if(chatToJoin == nullptr)
    {
        m_Ctable[chat_row].push_back(new Chat(chat));
        chatToJoin = m_Ctable[chat_row].back();
    }
    
    //link chat and user
    UserJoining->addChat(chatToJoin);
    chatToJoin->addUser(UserJoining);
}

int ChatTrackerImpl::terminate(string chat)
{
    Chat* toBeTerminated;
    
    //Search the chat in the Hash Table
    int chat_row = targetRow(chat);

    list<Chat*>::iterator chat_itr = m_Ctable[chat_row].begin();

    for(; chat_itr != m_Ctable[chat_row].end(); chat_itr++)
    {
        if(chat == (*chat_itr)->getName())
        {
            toBeTerminated = (*chat_itr);
            //save contributions
            int contributions = toBeTerminated->getContributions();
            
            //terminate the chat
            /* deletes the chat from all users */
            toBeTerminated->terminateChat();
            
            //erase and delete it from the Hash Table
            m_Ctable[chat_row].erase(chat_itr);
            delete toBeTerminated;
            
            return contributions;
        }
    }
    
    //CHAT DOES NOT EXIST !!!!
    return 0;
}

int ChatTrackerImpl::contribute(string user)
{
    //Search the user in the Hash Table
    int user_row = targetRow(user);

    list<User*>::iterator user_itr = m_Utable[user_row].begin();

    for(; user_itr != m_Utable[user_row].end(); user_itr++)
    {
        //if found
        if(user == (*user_itr)->getName())
        {
            //make contribution
            return (*user_itr)->makeContribution();
        }
    }

    //If no such User Exists !!!!!
    return 0;
}

int ChatTrackerImpl::leave(string user, string chat)
{
    User* UserLeaving = nullptr;
    Chat* ChattoBeLeft = nullptr;
    //Search the chat in the Hash Table
    int chat_row = targetRow(chat);

    list<Chat*>::iterator chat_itr = m_Ctable[chat_row].begin();

    for(; chat_itr != m_Ctable[chat_row].end(); chat_itr++)
    {
        //if found
        if(chat == (*chat_itr)->getName())
        {
            //make it tobeleft chat
            ChattoBeLeft = *chat_itr;
        }
    }
    
    //Search the user in the Hash Table
    int user_row = targetRow(user);

    list<User*>::iterator user_itr = m_Utable[user_row].begin();

    for(; user_itr != m_Utable[user_row].end(); user_itr++)
    {
        //if found
        if(user == (*user_itr)->getName())
        {
            //make it the leaving user
            UserLeaving = *user_itr;
        }
    }

    //if both chat and user are not bogus
    if(UserLeaving != nullptr && ChattoBeLeft != nullptr)
    {
        //LEAVE!!!!!!!!!!!!!!!!
        return UserLeaving->leaveChat(ChattoBeLeft);
    }
    
    return -1;
}

int ChatTrackerImpl::leave(string user)
{
    //Search the user in the Hash Table
    int user_row = targetRow(user);

    list<User*>::iterator user_itr = m_Utable[user_row].begin();

    for(; user_itr != m_Utable[user_row].end(); user_itr++)
    {
        if(user == (*user_itr)->getName())
        {
            return (*user_itr)->leaveCurrentChat();
        }
    }
    
    return -1;
}

ChatTrackerImpl::~ChatTrackerImpl()
{
    //destroy Chat
    for(int i = 0; i < m_Ctable.size(); i++)
    {
        if(!m_Ctable[i].empty())
        {
            list<Chat*>::iterator j = m_Ctable[i].begin();
            while(j != m_Ctable[i].end())
            {
                delete (*j);
                j++;
            }
        }
    }
    
    //destroy User
    for(int i = 0; i < m_Utable.size(); i++)
    {
        if(!m_Utable[i].empty())
        {
            list<User*>::iterator j = m_Utable[i].begin();
            while(j != m_Utable[i].end())
            {
                delete (*j);
                j++;
            }
        }
    }
}



//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}










