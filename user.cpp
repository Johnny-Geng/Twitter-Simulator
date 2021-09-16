#include "user.h"
#include "tweet.h"
User::User(std::string name) : myUsername(name)
{}

User::~User()
{

}

std::string User::name() const
{
    return myUsername;
}

std::set<User*> User::followers() const
{
    return myFollowers;
}

std::set<User*> User::following() const
{
    return myFollowing;
}

std::list<Tweet*> User::tweets() const
{
    return myTweets;
}

void User::addFollower(User* u)
{
    if (myFollowers.find(u) == myFollowers.end())
    {myFollowers.insert(u);}    
}

void User::addFollowing(User* u)
{
    if (myFollowing.find(u) == myFollowing.end())
    {myFollowing.insert(u);}  
}

void User::addTweet(Tweet* t)
{
    myTweets.push_back(t);
}

std::vector<Tweet*> User::getFeed()
{
    std::vector<Tweet*> vecTweets;

    // add my tweets to vecTweets
    for (std::list<Tweet*>::iterator it = myTweets.begin(); 
        it != myTweets.end(); ++it)
    {
        vecTweets.push_back(*it);
    }

    // add my followings' tweets to myFollowTweets
    for (std::set<User*>::iterator it = myFollowing.begin();
        it != myFollowing.end(); ++it)
    {
        std::list<Tweet*> tempFollowTweets = (*it)->tweets();
        for (std::list<Tweet*>::iterator tweet_it = tempFollowTweets.begin();
            tweet_it != tempFollowTweets.end(); ++tweet_it)
        {
            vecTweets.push_back(*tweet_it);
        } 
    }

    //sort
    std::sort(vecTweets.begin(),vecTweets.end(), TweetComp());
    return vecTweets;
}