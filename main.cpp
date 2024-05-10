#include <iostream>
#include<string>
using namespace std;

class Date
{
private:
    int day;
    int month;
    int year;

    static Date Today;

public:
    Date()
    {
        day = 0;
        month = 0;
        year = 0;
    }
    Date(int _day, int _month, int _year)
    {
        day = _day;
        month = _month;
        year = _year;
    }
    ~Date() {}

    void setDate(int _day, int _month, int _year)
    {
        day = _day;
        month = _month;
        year = _year;
    }

    void setDay(int _day)
    {
        day = _day;
    }
    int getDay()
    {
        return day;
    }

    void setMonth(int _month)
    {
        month = _month;
    }
    int getMonth()
    {
        return month;
    }

    void setYear(int _year)
    {
        year = _year;
    }
    int getYear()
    {
        return year;
    }

    void viewDate()
    {
        cout << "(" << day << "/" << month << "/" << year << ")";
    }

    bool operator==(const Date &D)
    {
        if (year == D.year && month == D.month && day == D.day)
            return true;
        else
            return false;
    }
};

class Post;

class Account
{
    string id;
    const int maxPosts = 10;
    Post **timeline;

    int noOfPosts;

public:
    Account(const string);
    Account(ifstream &);
    virtual ~Account();
    bool AddPost(Post *);
    Post *GetLatestPost();
    const char *getAccountID();
    Post *SearchPostByID(char *);
    void ViewTimeline();
    void PrintMemories();
    virtual void PrintName() = 0;
    virtual void PrintDetails();
};

class User
{
    string firstName;
    string lastName;
    static const int maxFriends = 10;
    static const int maxNoOfLikedPages = 10;

    Page **likedPages;
    int noOfLikedPages;

    User **friends;
    int noOfFriends;

public:
    User(string, string, string);
    User(ifstream &, string, int &, char **, int &);
    ~User();
    void ReadDataFromFile(ifstream &, char **, int &, char **, int &);
    bool AddFriend(User *);
    void RemoveFriend(User *);
    bool LikePage(Page *);
    void UnlikePage(Page *);
    void PrintName();
    int getFriendCount();
    int getLikedPagesCount();
    void ViewHome();
    void PrintFriendList();
    void PrintLikedPagesList();
};

class Page : public Account
{
    static const int maxLikers = 10;
    string title;

    int noOfLikers;
    Account **likers;

public:
    Page(const string, const string);
    Page(ifstream &);
    ~Page();
    bool AddLiker(Account *);
    void RemoveLiker(Account *);
    void PrintName();
    // int getLikesCount();
};
class Comment
{
    string id;
    string text;
    Account *author;

public:
    Comment(const string, const string, Account *);
    Comment(ifstream &, char *postID, char *owner);
    ~Comment();
    void setAuthor(Account *acc);
    const Account *getAuthor();
    void Print();
};

class Activity
{
    static const int noOfTypes = 4;
    static const int noOfSubtypes = 3;

    static const char *types[noOfTypes];
    // static const char* subtypes[noOfTypes][noOfSubtypes];
    int typeNo;
    char *subtype;

public:
    Activity(int, const string);
    Activity(ifstream &);
    ~Activity();
    void Print();
};

class Post
{
    static const int maxComments = 10;
    static const int maxLikers = 10;
    string id;
    string text;

    int noOfLikers;
    Account **likers;

    Date shareDate;

    Comment **comments;
    int noOfComments;

    Activity *activity;

protected:
    void PrintComments();
    void PrintText();
    Account *owner;

public:
    Post(const string, const string, int, int, int, Account *, int = -1, string = nullptr);
    Post(const string, const string, const Date &, Account *, int = -1, string = nullptr);
    Post(ifstream &, string, char **, int &);
    void ReadDataFromFile(ifstream &, string, char **, int &);
    virtual ~Post();
    Date getShareDate();
    bool AddLiker(Account *);
    void RemoveLiker(Account *);
    bool AddComment(Comment *);
    void RemoveCommentsOfAccount(Account *);
    virtual void Print(bool = false, bool = true);
    void SetOwner(Account *);
    const Account *getOwner();
    const string getID();
    void PrintLikedList();
};

class Memory : public Post
{
    Post *origPost;

public:
    Memory(const string id, const string text, int dd, int mm, int yyyy, Account *author, Post *orig);
    Memory(const string id, const string text, const Date &currDate, Account *author, Post *orig);
    void Print(bool, bool);
};

int main()
{
}