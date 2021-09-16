#include "user.h"
#include "tweet.h"

Tweet::Tweet()
{}

Tweet::Tweet(User* user, const DateTime& time, const std::string& text) : 
tweetUser(user), tweetTime(time), tweetText(text)
{
    // handle hashtags if exist
    std::stringstream tweetWords;
    std::string word;
    tweetWords << text;
    while (tweetWords >> word)
    {
        if (word[0] == '#')
        {
            word.erase(0,1);
            tweetHash.insert(word);
        }
    }
}

DateTime const & Tweet::time() const
{
    return tweetTime;
}

std::string const & Tweet::text() const
{
    return tweetText;
}

std::set<std::string> Tweet::hashTags() const
{
    return tweetHash;
}

bool Tweet::operator<(const Tweet& other) const
{
    if (this->tweetTime < other.tweetTime)
    {
        return true;
    }
    else
    {
        return false;
    }
}

User* Tweet::user() const
{
    return tweetUser;
}

std::ostream& operator<<(std::ostream& os, const Tweet& t)
{
    os << t.tweetTime << " " << t.user()->name() << " " << t.text();
    return os;
}