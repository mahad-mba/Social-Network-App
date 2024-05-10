#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Date
{
private:
    int day;
    int month;
    int year;

    static Date Today;

public:
    Date() : day(0), month(0), year(0) {}
    Date(int _day, int _month, int _year) : day(_day), month(_month), year(_year) {}
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
    int getDay() const
    {
        return day;
    }

    void setMonth(int _month)
    {
        month = _month;
    }
    int getMonth() const
    {
        return month;
    }

    void setYear(int _year)
    {
        year = _year;
    }
    int getYear() const
    {
        return year;
    }

    void viewDate() const
    {
        cout << "(" << day << "/" << month << "/" << year << ")";
    }

    bool operator==(const Date &D) const
    {
        return (year == D.year && month == D.month && day == D.day);
    }

    static Date getTodaysDate()
    {
        return Today;
    }

    static void setTodaysDate(int dd, int mm, int yyyy)
    {
        Today.day = dd;
        Today.month = mm;
        Today.year = yyyy;
    }

    static void setTodaysDate(const Date &d)
    {
        Today = d;
    }

    void Print() const
    {
        cout << "(" << day << "/" << month << "/" << year << ")";
    }

    friend istream &operator>>(istream &in, Date &d)
    {
        in >> d.day >> d.month >> d.year;
        in.ignore();
        return in;
    }

    bool operator<=(const Date &D) const
    {
        if (year < D.year)
        {
            return true;
        }
        else if (year > D.year)
        {
            return false;
        }

        if (month < D.month)
        {
            return true;
        }
        else if (month > D.month)
        {
            return false;
        }

        return day <= D.day;
    }

    bool operator<(const Date &D) const
    {
        if (year < D.year)
        {
            return true;
        }
        else if (year > D.year)
        {
            return false;
        }

        if (month < D.month)
        {
            return true;
        }
        else if (month > D.month)
        {
            return false;
        }

        return day < D.day;
    }

    bool operator>=(const Date &D) const
    {
        if (year > D.year)
        {
            return true;
        }
        if (year < D.year)
        {
            return false;
        }

        if (month > D.month)
        {
            return true;
        }
        else if (month < D.month)
        {
            return false;
        }

        return day >= D.day;
    }

    bool operator>(const Date &D) const
    {
        if (year > D.year)
        {
            return true;
        }
        else if (year >= D.year)
        {
            return false;
        }

        if (month >= D.month)
        {
            return true;
        }
        else if (month > D.month)
        {
            return false;
        }

        return day > D.day;
    }
};

Date Date::Today = Date(1, 1, 1970);

class Post;
class Account
{
protected:
    string id;
    const int maxPosts = 10;
    Post **timeline; // Aggregation
    int noOfPosts;

public:
    Account(const string &accountId) : id(accountId), noOfPosts(0), timeline(nullptr) {}
    Account(ifstream &ifile) : noOfPosts(0), timeline(nullptr)
    {
        string temp;
        ifile >> temp;
        id = temp;
    }
    ~Account()
    {
        if (timeline)
        {
            for (int i = 0; i < noOfPosts; ++i)
            {
                delete timeline[i];
            }
            delete[] timeline;
        }
    }

    bool AddPost(Post *p)
    {
        if (!timeline)
        {
            timeline = new Post *[maxPosts];
            for (int i = 0; i < maxPosts; i++)
            {
                timeline[i] = nullptr;
            }
        }

        if (noOfPosts >= maxPosts)
        {
            return false;
        }

        int index = 0;
        for (index = 0; index < noOfPosts; index++)
        {
            if (timeline[index]->getShareDate() > p->getShareDate())
            {
                break;
            }
        }

        for (int i = noOfPosts; i > index; i--)
        {
            timeline[i] = timeline[i - 1];
        }

        timeline[index] = p;
        noOfPosts++;

        return true;
    }

    Post *GetLatestPost()
    {
        if (noOfPosts <= 0)
        {
            return nullptr;
        }

        Post *latest = nullptr;
        for (int i = 0; i < noOfPosts; i++)
        {
            if (timeline[i]->getShareDate() <= Date::getTodaysDate())
            {
                if (latest == nullptr || latest->getShareDate() < timeline[i]->getShareDate())
                {
                    latest = timeline[i];
                }
            }
        }
        return latest;
    }

    const string &getAccountID() const
    {
        return id;
    }

    Post *SearchPostByID(const string &postId)
    {
        for (int i = 0; i < noOfPosts; i++)
        {
            if (timeline[i]->getID() == postId)
            {
                return timeline[i];
            }
        }
        return nullptr;
    }

    void ViewTimeline() const
    {
        PrintName();
        cout << " - Timeline" << endl
             << endl;

        for (int i = 0; i < noOfPosts; i++)
        {
            timeline[i]->Print(true);
        }
    }

    void PrintDetails() const
    {
        cout << id << " - ";
        PrintName();
        cout << endl;
    }

    void PrintMemories() const
    {
        cout << "On this Day" << endl;
        for (int i = 0; i < noOfPosts; i++)
        {
            if (timeline[i]->getShareDate().getDay() == Date::getTodaysDate().getDay() && timeline[i]->getShareDate().getMonth() == Date::getTodaysDate().getMonth() && timeline[i]->getShareDate().getYear() < Date::getTodaysDate().getYear())
            {
                cout << Date::getTodaysDate().getYear() - timeline[i]->getShareDate().getYear();
                cout << " Years Ago" << endl;
                timeline[i]->Print(true);
            }
        }
    }

    virtual void PrintName() const = 0;
};

class User
{
    private:
    string firstName;
    string lastName;
    static const int maxFriends = 10;
    static const int maxNoOfLikedPages = 10;

    Page **likedPages; // Aggregation
    int noOfLikedPages;

    User **friends; // Aggregation
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
    private:
    static const int maxLikers = 10;
    string title;

    int noOfLikers;
    Account **likers; // Aggregation

public:
    Page(const string, const string);
    Page(ifstream &);
    ~Page();
    bool AddLiker(Account *);
    void RemoveLiker(Account *);
    void PrintName();
    int getLikesCount();
};
class Comment
{
    private:
    string id;
    string text;
    Account *author; // Aggregation

public:
    Comment(const string, const string, Account *);
    Comment(ifstream &, char *postID, char *owner);
    ~Comment();
    void setAuthor(Account *acc);
    const Account *getAuthor();
    void Print();
};

class Activity
{   private:
    static const int noOfTypes = 4;
    static const int noOfSubtypes = 3;

    static const char *types[noOfTypes];
    static const char *subtypes[noOfTypes][noOfSubtypes];
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
    private:
    static const int maxComments = 10;
    static const int maxLikers = 10;
    string id;
    string text;

    int noOfLikers;
    Account **likers; // Aggregation

    Date shareDate; // composition

    Comment **comments; // Aggregation
    int noOfComments;

    Activity *activity; // Aggregation

protected:
    void PrintComments();
    void PrintText();
    Account *owner;  // Aggregation

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