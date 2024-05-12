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

class User : public Account
{
    string firstName;
    string lastName;
    static const int maxFriends = 10;
    static const int maxNoOfLikedPages = 10;

    User **friends; // Aggregation
    int noOfFriends;

    Page **likedPages; // Aggregation
    int noOfLikedPages;

public:
    User(const string &accountId, const string &_firstName, const string &_lastName = "")
        : Account(accountId), firstName(_firstName), lastName(_lastName), noOfFriends(0), noOfLikedPages(0), friends(nullptr), likedPages(nullptr) {}

    User(ifstream &ifile, const string &_firstName, int &friendsCount, char **friends, int &likedPagesCount, char **likedPages)
        : Account(ifile), firstName(_firstName), noOfFriends(0), noOfLikedPages(0), friends(nullptr), likedPages(nullptr)
    {
        ReadDataFromFile(ifile, friends, friendsCount, likedPages, likedPagesCount);
    }

    ~User()
    {
        if (likedPages)
            delete[] likedPages;
        if (friends)
            delete[] friends;
    }

    void ReadDataFromFile(ifstream &ifile, char **friends, int &friendsCount, char **likedPages, int &likedPagesCount)
    {
        string temp;
        while (friendsCount < maxFriends)
        {
            ifile >> temp;
            if (temp == "-1")
            {
                break;
            }
            else
            {
                friends[friendsCount] = new char[temp.length() + 1];
                strcpy(friends[friendsCount], temp.c_str());
                friendsCount++;
            }
        }

        while (likedPagesCount < maxNoOfLikedPages)
        {
            ifile >> temp;
            if (temp == "-1")
            {
                break;
            }
            else
            {
                likedPages[likedPagesCount] = new char[temp.length() + 1];
                strcpy(likedPages[likedPagesCount], temp.c_str());
                likedPagesCount++;
            }
        }
    }

    bool AddFriend(User *secondUser)
    {
        if (!secondUser)
        {
            return false;
        }

        if (!friends)
        {
            friends = new User *[maxFriends];
            for (int i = 0; i < maxFriends; i++)
            {
                friends[i] = nullptr;
            }
        }

        if (noOfFriends < maxFriends)
        {
            friends[noOfFriends++] = secondUser;
            return true;
        }

        return false;
    }

    void RemoveFriend(User *secondUser)
    {
        if (!secondUser || !friends)
        {
            return;
        }

        for (int i = 0; i < noOfFriends; i++)
        {
            if (friends[i] != secondUser)
            {
                continue;
            }

            friends[i] = nullptr;

            for (i = i + 1; i < noOfFriends; i++)
            {
                friends[i - 1] = friends[i];
            }

            noOfFriends--;
            friends[noOfFriends] = nullptr;
        }
    }

    bool LikePage(Page *likedPage)
    {
        if (!likedPage)
        {
            return false;
        }

        if (!likedPages)
        {
            likedPages = new Page *[maxNoOfLikedPages];
            for (int i = 0; i < maxNoOfLikedPages; i++)
            {
                likedPages[i] = nullptr;
            }
        }

        if (noOfLikedPages < maxNoOfLikedPages)
        {
            likedPages[noOfLikedPages++] = likedPage;
            return true;
        }

        return false;
    }

    void UnlikePage(Page *pagePtr)
    {
        if (!pagePtr || !likedPages)
        {
            return;
        }

        for (int i = 0; i < noOfLikedPages; i++)
        {
            if (likedPages[i] != pagePtr)
            {
                continue;
            }

            likedPages[i] = nullptr;

            for (i = i + 1; i < noOfLikedPages; i++)
            {
                likedPages[i - 1] = likedPages[i];
            }

            noOfLikedPages--;
            likedPages[noOfLikedPages] = nullptr;
        }
    }

    void PrintName()
    {
        cout << firstName << ' ' << lastName;
    }

    int getFriendCount() const
    {
        return noOfFriends;
    }

    int getLikedPagesCount() const
    {
        return noOfLikedPages;
    }

    void ViewHome()
    {
        PrintName();
        cout << " - Home Page" << endl
             << endl;

        for (int i = 0; i < noOfFriends; i++)
        {
            Post *latestPost = friends[i]->GetLatestPost();
            if (latestPost)
            {
                latestPost->Print();
            }
        }

        for (int i = 0; i < noOfLikedPages; i++)
        {
            Post *latestPost = likedPages[i]->GetLatestPost();
            if (latestPost)
            {
                latestPost->Print();
            }
        }
    }

    void PrintFriendList()
    {
        PrintName();
        cout << " - " << "Friends" << '\n'
             << '\n';

        for (int i = 0; i < noOfFriends; i++)
        {
            friends[i]->PrintDetails();
        }
    }

    void PrintLikedPagesList()
    {
        PrintName();
        cout << " - " << "Liked Pages" << '\n'
             << '\n';

        for (int i = 0; i < noOfLikedPages; i++)
        {
            likedPages[i]->PrintDetails();
        }
    }
};

class Page : public Account
{
private:
    static const int maxLikers = 10;
    string title;

    int noOfLikers;
    Account **likers; // Aggregation

public:
    Page(const string &accountId, const string &pageTitle) : Account(accountId), title(pageTitle), noOfLikers(0) {}
    Page(ifstream &ifile) : Account(ifile), noOfLikers(0)
    {
        string temp;
        ifile >> temp;
        getline(ifile, title);
    }
    ~Page()
    {
        for (int i = 0; i < noOfLikers; ++i)
        {
            delete likers[i];
        }
    }

    bool AddLiker(Account *accPtr)
    {
        if (!accPtr || noOfLikers >= maxLikers)
        {
            return false;
        }

        likers[noOfLikers++] = accPtr;
        return true;
    }

    void RemoveLiker(Account *accPtr)
    {
        if (!accPtr)
        {
            return;
        }

        for (int i = 0; i < noOfLikers; i++)
        {
            if (likers[i] == accPtr)
            {
                for (int j = i; j < noOfLikers - 1; j++)
                {
                    likers[j] = likers[j + 1];
                }
                likers[--noOfLikers] = nullptr;
                break;
            }
        }
    }

    void PrintName() const override
    {
        cout << title;
    }

    int getLikesCount() const
    {
        return noOfLikers;
    }
};

class Comment
{
private:
    string id;
    string text;
    Account *author; // Aggregation

public:
    Comment(const string &commentId, const string &body, Account *owner) : id(commentId), text(body), author(owner) {}
    Comment(ifstream &ifile, const string &postID, const string &owner)
    {
        ifile >> id >> postID >> owner;
        getline(ifile, text);
    }
    ~Comment()
    {
        if (author)
            delete[] author;
    }

    void setAuthor(Account *acc)
    {
        author = acc;
    }
    const Account *getAuthor() const
    {
        return author;
    }

    void Print() const
    {
        author->PrintName();
        cout << " wrote: ";
        if (!text.empty())
        {
            cout << "\"" << text << "\"" << endl;
        }
    }

    const string &getID() const
    {
        return id;
    }

    const string &getText() const
    {
        return text;
    }
};

class Activity
{
    static const int noOfTypes = 4;
    static const int noOfSubtypes = 3;

    static const string types[noOfTypes];
    static const string subtypes[noOfTypes][noOfSubtypes];

    int typeNo;
    string subtype;

public:
    const string types[noOfTypes] =
        {
            "feeling",
            "thinking about",
            "making",
            "celebrating"};

    Activity(int type, const string &subtype) : typeNo(type - 1)
    {
        if (typeNo < 0 || typeNo >= noOfTypes)
        {
            typeNo = -1;
            return;
        }

        this->subtype = subtype;
    }

    void ReadDataFromFile(ifstream &ifile)
    {
        string temp;

        ifile >> typeNo;
        ifile.ignore();
        typeNo -= 1;

        if (typeNo < 0 || typeNo >= noOfTypes)
        {
            typeNo = -1;
            return;
        }

        getline(ifile, temp);

        if (!temp.empty())
        {
            subtype = temp;
        }
    }

    void Print()
    {
        if (typeNo != -1)
        {
            cout << " is " << types[typeNo];
            if (!subtype.empty())
            {
                cout << ' ' << subtype;
            }
            cout << endl;
        }
    }
};

class Post
{
private:
    static const int maxComments = 10;
    static const int maxLikers = 10;
    string id;
    string text;

    int noOfLikers;
    Account *likers[maxLikers]; // Aggregation

    Date shareDate;

    Comment *comments[maxComments]; // Aggregation
    Activity *activity;             // Aggregation

protected:
    void PrintComments() const
    {
        for (int i = 0; i < maxComments && comments[i] != nullptr; ++i)
        {
            comments[i]->Print();
        }
    }

    void PrintText() const
    {
        cout << "\"" << text << "\"";
    }

    Account *owner; // Aggregation

public:
    Post(const string &_id, const string &_text, int dd, int mm, int yyyy, Account *author, int typeOfActivity = -1, const string &subtypeOfActivity = "") : id(_id), text(_text), shareDate(dd, mm, yyyy), noOfLikers(0), owner(author), activity(nullptr)
    {
        if (typeOfActivity != -1 && !subtypeOfActivity.empty())
        {
            activity = new Activity(typeOfActivity, subtypeOfActivity);
        }
    }

    Post(const string &_id, const string &_text, const Date &_shareDate, Account *author, int typeOfActivity = -1, const string &subtypeOfActivity = "") : id(_id), text(_text), shareDate(_shareDate), noOfLikers(0), owner(author), activity(nullptr)
    {
        if (typeOfActivity != -1 && !subtypeOfActivity.empty())
        {
            activity = new Activity(typeOfActivity, subtypeOfActivity);
        }
    }

    Post(ifstream &ifile, const string &ownerId, char **likersList, int &likesCount) : noOfLikers(0), owner(nullptr), activity(nullptr)
    {
        ReadDataFromFile(ifile, ownerId, likersList, likesCount);
    }

    ~Post()
    {
        delete activity;
        for (int i = 0; i < maxComments; i++)
        {
            delete comments[i];
        }
    }

    Date getShareDate() const
    {
        return shareDate;
    }

    bool AddLiker(Account *accPtr)
    {
        if (!accPtr)
        {
            return false;
        }

        for (int i = 0; i < noOfLikers; i++)
        {
            if (likers[i] == accPtr)
            {
                return true;
            }
        }

        if (noOfLikers < maxLikers)
        {
            likers[noOfLikers++] = accPtr;
            return true;
        }

        return false;
    }

    void RemoveLiker(Account *accPtr)
    {
        for (int i = 0; i < noOfLikers; i++)
        {
            if (likers[i] == accPtr)
            {
                // Shift elements to fill the gap
                for (int j = i; j < noOfLikers - 1; j++)
                {
                    likers[j] = likers[j + 1];
                }
                --noOfLikers;
                break;
            }
        }
    }

    bool AddComment(Comment *cmt)
    {
        if (!cmt)
        {
            return false;
        }

        for (int i = 0; i < maxComments; i++)
        {
            if (comments[i] == nullptr)
            {
                comments[i] = cmt;
                return true;
            }
        }

        return false;
    }

    void RemoveCommentsOfAccount(Account *accPtr)
    {
        for (int i = 0; i < maxComments; i++)
        {
            if (comments[i] && comments[i]->getAuthor() == accPtr)
            {
                delete comments[i];
                comments[i] = nullptr;
            }
        }
    }

    void Print(bool printDate = false, bool withComments = true) const
    {
        cout << "--- ";
        owner->PrintName();
        cout << " shared ";
        PrintText();
        if (printDate)
        {
            cout << "...";
            shareDate.Print();
        }
        cout << endl;

        if (withComments)
        {
            PrintComments();
        }
    }

    const Account *getOwner() const
    {
        return owner;
    }

    const string &getID() const
    {
        return id;
    }

    void SetOwner(Account *acc)
    {
        owner = acc;
    }

    void PrintLikedList() const
    {
        cout << "Post Liked By:" << endl;
        for (int i = 0; i < noOfLikers; ++i)
        {
            likers[i]->PrintDetails();
        }
    }

    void ReadDataFromFile(ifstream &ifile, const string &ownerId, char **likersList, int &likesCount)
    {
        ifile >> id;
        shareDate.setDate(ifile.get(), ifile.get(), ifile.get());
        ifile.ignore();
        getline(ifile, text);

        int type;
        ifile >> type;
        if (type == 2)
        {
            int activityType;
            string activitySubtype;
            ifile >> activityType;
            ifile.ignore();
            getline(ifile, activitySubtype);
            activity = new Activity(activityType, activitySubtype);
        }

        owner = nullptr;
        likesCount = 0;
        while (likesCount < maxLikers && *likersList)
        {
            ifile >> *likersList;
            ++likesCount;
        }
    }
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
    Page page1("page1_id", "Page 1 Title");

    cout << "Page Title: ";
    page1.PrintName();
    cout << endl;

    Account *account = nullptr;
    Comment comment1("comment1_id", "This is a comment", account);

    cout << "Comment ID: " << comment1.getID() << endl;
    cout << "Comment Text: " << comment1.getText() << endl;

    return 0;
}