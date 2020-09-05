//
//  ChatTracker.hpp
//  PROJECT 4
//
//  Created by Sanchit Agarwal on 5/28/20.
//  Copyright © 2020 Sanchit Agarwal. All rights reserved.
//

#ifndef CHATTRACKER_INCLUDED
#define CHATTRACKER_INCLUDED

#include <string>

class ChatTrackerImpl;

class ChatTracker
{
  public:
    ChatTracker(int maxBuckets = 20000);
    ~ChatTracker();
    void join(std::string user, std::string chat);
    int terminate(std::string chat);
    int contribute(std::string user);
    int leave(std::string user, std::string chat);
    int leave(std::string user);
      // We prevent a ChatTracker object from being copied or assigned
    ChatTracker(const ChatTracker&) = delete;
    ChatTracker& operator=(const ChatTracker&) = delete;

  private:
    ChatTrackerImpl* m_impl;
};

#endif // SYMBOLTABLE_INCLUDED

