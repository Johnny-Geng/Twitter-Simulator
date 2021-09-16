#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "twiteng.h"
#include "util.h"
using namespace std;

TwitEng::TwitEng()
{

}

TwitEng::~TwitEng()
{
  if (!usermap.empty())
  {
    for (map<string, User*>::iterator it = usermap.begin();
        it != usermap.end();)
    {
      map<string, User*>::iterator it_toDelete = it;
      ++it;
      list<Tweet*> templist = it_toDelete->second->tweets();
      for (list<Tweet*>::iterator tweet_it = templist.begin();
          tweet_it != templist.end();)
      {
        list<Tweet*>::iterator tweet_it_toDelete = tweet_it;
        ++tweet_it;
        delete *tweet_it_toDelete;
      }
      delete it_toDelete->second;
    }
  }
}

bool TwitEng::parse(char* filename)
{
  ifstream ifile(filename);
  if (ifile.fail()) {
    cerr << "Unable to open " << filename << endl;
    return true;
  }
  int numUsers;
  ifile >> numUsers;
  if (ifile.fail()) {
    cerr << "Unable to read number of users " << endl;
    ifile.close();
    return true;
  }
  // Finish the parsing of the input file. Return false if successful,
  // true otherwise.

  // extract users info according to numUsers provided
  for(int i=0; i < numUsers; ++i)
  {
    string infoLine;
    if (getline(ifile,infoLine))
    {
      if(i==0)
      {
        getline(ifile,infoLine);
      }

      stringstream ss;
      string username, following_username;
      ss << infoLine;

      if (ss >> username)
      {
        // create a new user if not already
        if (usermap.find(username) == usermap.end())
        {
          User* newUser = new User(username);
          usermap[username] = newUser;
        }
      }
      else // mismatched number of lines for user info
      {
        ifile.close();
        return true;
      }

      while(ss >> following_username)
      {
        // create a new user if not already
        if (usermap.find(following_username) == usermap.end())
        {
          User* newUser = new User(following_username);
          usermap[following_username] = newUser;
        }

        // add to the following list
        usermap[username]->addFollowing(usermap[following_username]);
        // add to the follower list
        usermap[following_username]->addFollower(usermap[username]);
      }
    }
    else // mismatched number of lines for user info
    {
      ifile.close();
      return true;
    }
  }

  // extract tweets
  DateTime tweetDateTime;
  DateTime currTime;
  string tweetLine;
  while (ifile >> tweetDateTime)
  {
    // when an error occurs and tweetDateTime has been set to current system time
    if (!(tweetDateTime < currTime) && !(currTime < tweetDateTime))
    {
      ifile.close();
      return true;
    }

    getline(ifile,tweetLine);
    string tweetUser, tempWord, tweetContent="";
    stringstream ss;
    ss << tweetLine;

    // extract username
    ss >> tweetUser;
    if (ss.fail())
    {
      ifile.close();
      return true;
    }
    // invalid username
    else if (!validUser(tweetUser))
    {
      ifile.close();
      return true;
    }

    // extract tweet content
    while (ss >> tempWord)
    {
      tweetContent += tempWord;
      tweetContent += " ";
    }
    rtrim(tweetContent);

    // construct the tweet and attach it to the user
    addTweet(tweetUser,tweetDateTime,tweetContent);

  }

  ifile.close();
  return false;

}

void TwitEng::addTweet(const std::string& username, const DateTime& time, const std::string& text)
{
  if (!validUser(username))
  {
    return;
  }
  Tweet* newTweet = new Tweet(usermap[username],time,text);
  usermap[username]->addTweet(newTweet);

  // update hashmap
  set<string> hashwords = newTweet->hashTags();
  for (set<string>::iterator it = hashwords.begin(); it != hashwords.end(); ++it)
  {
    // new hash tag
    if (hashmap.find(*it) == hashmap.end())
    {
      vector<Tweet*>* tweetList = new vector<Tweet*>;
      tweetList->push_back(newTweet);
      hashmap[*it] = *tweetList;
    }

    // hash tag already exists; add tweet to it
    else
    {
      hashmap.find(*it)->second.push_back(newTweet);
    }
  }
}

std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy)
{
  set<Tweet*> matchedTweets;
  vector<Tweet*> outputTweets;
  //empty search list
  if (terms.size()== 0)
  {return outputTweets;}

  // OR
  if (strategy == 1)
  {
    // for each hash tag word
    for (vector<string>::iterator it = terms.begin(); it != terms.end(); ++it)
    {
      // hash map contains this hash tag
      if (hashmap.find(*it) != hashmap.end())
      {
        vector<Tweet*> temp = hashmap.find(*it)->second;
        // add all tweets under this hash tag
        for (vector<Tweet*>::iterator vec_it = temp.begin(); vec_it != temp.end(); ++vec_it)
        {
          // handle tweets with multiple hash tags; avoid duplicates
          if(matchedTweets.find(*vec_it) == matchedTweets.end())
          {
            matchedTweets.insert(*vec_it);
          }
        }
      }
    }
  }

  // AND
  else if (strategy == 0)
  {
    // check if every hash tag exists
    for (vector<string>::iterator it = terms.begin(); it != terms.end(); ++it)
    {
      if (hashmap.find(*it) == hashmap.end())
      {
        return outputTweets;
      }
    }

    vector<Tweet*> firstHash = hashmap.find(terms[0])->second;
    // assign matchedTweet to be the set of tweets of the 0th hash tag
    for (vector<Tweet*>::iterator it = firstHash.begin(); it != firstHash.end(); ++it)
    {
      matchedTweets.insert(*it);
    }

    // check if every tweet in the list have all other required hash tags
    for (set<Tweet*>::iterator it = matchedTweets.begin(); it != matchedTweets.end();)
    {
      bool missingHashtag = false;
      set<string> hashnames = (*it)->hashTags();
      // from 1st to last hash tags in term
      for (vector<string>::iterator str_it = ++terms.begin(); str_it != terms.end(); ++str_it)
      {  
        // missing hashtags
        if(hashnames.find(*str_it) == hashnames.end())
        {
          set<Tweet*>::iterator toDelete = it;
          ++it;
          matchedTweets.erase(*toDelete);
          missingHashtag = true;
          break;
        }
      }
      if (!missingHashtag)
      {
        ++it;
      }
    }
  }

  // output
  for (set<Tweet*>::iterator it = matchedTweets.begin(); it != matchedTweets.end(); ++it)
  {
    outputTweets.push_back(*it);
  }
  return outputTweets;
}

void TwitEng::dumpFeeds()
{
  for (map<string,User*>::iterator it = usermap.begin(); it != usermap.end(); ++it)
  {
    string filename = "";
    filename += it->first;
    filename += ".feed";
    ofstream ofile(filename);
    ofile << it->first << endl;
    vector<Tweet*> tweet_vector = it->second->getFeed();
    for (vector<Tweet*>::iterator tweet_it = tweet_vector.begin();
        tweet_it != tweet_vector.end(); ++tweet_it)
    {
      ofile << **tweet_it << endl;
    }
    ofile.close();
  }
}

bool TwitEng::validUser(const std::string& name) const
{
  if (usermap.find(name) != usermap.end())
  {
    return true;
  }
  else
  {
    return false;
  }
}

