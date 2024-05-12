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

    virtual void PrintName() const;
};
class User : public Account
{
protected:
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

    User(ifstream &ifile, const string &_firstName, int &friendsCount, string **friends, int &likedPagesCount, string **likedPages)
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

    static int getMaxFriends()
    {
        return maxFriends;
    }

    // Static method to get the maximum number of liked pages
    static int getMaxNoOfLikedPages()
    {
        return maxNoOfLikedPages;
    }

    void ReadDataFromFile(ifstream &ifile, string **friends, int &friendsCount, string **likedPages, int &likedPagesCount)
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
                friends[friendsCount] = new string(temp);
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
                likedPages[likedPagesCount] = new string(temp);
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
        ifile >> id;
        // Consume newline character after reading the id
        ifile.ignore(numeric_limits<streamsize>::max(), '\n');
        // Read the remaining text into the comment's text
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
private:
    Post *origPost;

public:
    Memory(const string &id, const string &text, int dd, int mm, int yyyy, Account *author, Post *orig)
        : Post(id, text, dd, mm, yyyy, author), origPost(orig) {}

    Memory(const string &id, const string &text, const Date &currDate, Account *author, Post *orig)
        : Post(id, text, currDate, author), origPost(orig) {}

    void Print(bool printDate = true, bool withComments = true)
    {
        cout << "~~~ ";
        owner->PrintName();
        cout << " shared a memory ~~~ ...";

        if (printDate)
        {
            getShareDate().Print();
        }
        cout << endl;

        PrintText();
        cout << endl;

        cout << '\t' << '\t' << '(' << Date::getTodaysDate().getYear() - origPost->getShareDate().getYear() << " Years Ago)" << '\n';
        origPost->Print(true, false);

        if (withComments)
        {
            PrintComments();
        }
    }
};

class SocialMediaApp
{
private:
    static SocialMediaApp *instance;
    const int maxFriends = 10;
    const int maxLikedPages = 10;
    const int maxIDLength = 10;
    string userFile;
    string pageFile;
    string postFile;
    string commentFile;

    User *currentUser; // Aggregation
    int noOfUsers;
    int noOfPages;
    int noOfPosts;
    int totalNoOfComments;

    User **user; // Aggregation
    Page **page; // Aggregation
    Post **post; // Aggregation

    void ReadUsersFromFile(ifstream &, string ***, int *, string ***, int *);
    void ReadPagesFromFile(ifstream &);
    void ReadPostsFromFile(ifstream &);
    void ReadCommentsFromFile(ifstream &);

    void SetupUsersFriends(string ***, int *);
    void SetupUsersLikedPages(string ***, int *);
    User *SearchUserByID(const string &);
    Page *SearchPageByID(const string &);
    Post *SearchPostByID(const string &);

    void DeleteUser(User *&);
    void DeletePage(Page *&);
    void DeletePostsOfAuthor(Account *);

    SocialMediaApp();

public:
    SocialMediaApp(const SocialMediaApp &) = delete;

    SocialMediaApp()
    {
        currentUser = nullptr;

        userFile = "Users.txt";
        pageFile = "Pages.txt";
        postFile = "Posts.txt";
        commentFile = "Comments.txt";

        user = nullptr;
        page = nullptr;
        post = nullptr;

        noOfUsers = 0;
        noOfPages = 0;
        noOfPosts = 0;
        totalNoOfComments = 0;
    }
    ~SocialMediaApp()
    {
        if (user)
        {
            for (int i = 0; i < noOfUsers; i++)
            {
                if (user[i])
                {
                    DeleteUser(user[i]);
                }
            }
            delete[] user;
            user = nullptr;
        }
        if (page)
        {
            for (int i = 0; i < noOfPages; i++)
            {
                if (page[i])
                {
                    DeletePage(page[i]);
                }
            }
            delete[] page;
            page = nullptr;
        }
        if (post)
        {
            for (int i = 0; i < noOfPosts; i++)
            {
                if (post[i])
                {
                    delete post[i];
                }
            }
            delete[] post;
            post = nullptr;
        }
    }

    static SocialMediaApp *getInstance()
    {
        if (!instance)
        {
            instance = new SocialMediaApp;
        }
        return instance;
    }

    void ReadDataFromFile()
    {
        ifstream ifile;
        ifile.open(userFile);
        if (!ifile.is_open())
        {
            return;
        }

        ifile >> noOfUsers;
        user = new User *[noOfUsers];

        string ***userFriends = new string **[noOfUsers];
        int *noOfFriends = new int[noOfUsers];

        string ***userLikedPages = new string **[noOfUsers];
        int *noOfLikedPages = new int[noOfUsers];

        for (int i = 0; i < noOfUsers; i++)
        {
            userFriends[i] = new string *[maxFriends];
            for (int j = 0; j < maxFriends; j++)
            {
                userFriends[i][j] = new string[maxIDLength];
            }

            userLikedPages[i] = new string *[maxLikedPages];
            for (int j = 0; j < maxLikedPages; j++)
            {
                userLikedPages[i][j] = new string[maxIDLength];
            }
        }

        ReadUsersFromFile(ifile, userFriends, noOfFriends, userLikedPages, noOfLikedPages);
        ifile.close();

        ifile.open(pageFile);
        if (ifile.is_open())
        {
            ifile >> noOfPages;
            page = new Page *[noOfPages];
            ReadPagesFromFile(ifile);
            ifile.close();
        }
        SetupUsersFriends(userFriends, noOfFriends);
        SetupUsersLikedPages(userLikedPages, noOfLikedPages);

        bool deleteUserFriends = false, deleteUserLikedPages = false;

        if (userFriends)
            deleteUserFriends = true;

        if (userLikedPages)
            deleteUserLikedPages = true;

        for (int i = 0; i < noOfUsers; i++)
        {
            if (userFriends[i] && deleteUserFriends)
            {
                for (int j = 0; j < maxFriends; j++)
                {
                    if (userFriends[i][j])
                    {
                        delete[] userFriends[i][j];
                    }
                    delete[] userFriends[i];
                }
            }
            if (userLikedPages[i] && deleteUserLikedPages)
            {
                for (int j = 0; j < maxLikedPages; j++)
                {
                    if (userLikedPages[i][j])
                    {
                    }
                    delete[] userLikedPages[i][j];
                }

                delete[] userLikedPages[i];
            }
        }

        if (deleteUserFriends)
            delete[] userFriends;
        delete[] noOfFriends;
        userFriends = nullptr;
        noOfFriends = nullptr;

        if (deleteUserLikedPages)
            delete[] userLikedPages;
        delete[] noOfLikedPages;
        userLikedPages = nullptr;
        noOfLikedPages = nullptr;

        ifile.open(postFile);

        if (ifile.is_open())
        {
            ifile >> noOfPosts;

            post = new Post *[noOfPosts];

            ReadPostsFromFile(ifile);

            ifile.close();
        }

        ifile.open(commentFile);

        if (ifile.is_open())
        {
            ifile >> totalNoOfComments;

            ReadCommentsFromFile(ifile);
        }
    }
    void ReadUsersFromFile(ifstream &ifile, string ***userFriends, int *friendsCount, string ***userLikedPages, int *likedPagesCount)
    {
        for (int i = 0; i < noOfUsers; i++)
        {
            // Initialize the arrays inside the function
            userFriends[i] = new string *[User::getMaxFriends()];
            userLikedPages[i] = new string *[User::getMaxNoOfLikedPages()];

            // Initialize friendsCount and likedPagesCount
            friendsCount[i] = 0;
            likedPagesCount[i] = 0;

            // Read data into the arrays
            string firstName;
            user[i] = new User(ifile, firstName, friendsCount[i], userFriends[i], likedPagesCount[i], userLikedPages[i]);
        }
    }
    void ReadPagesFromFile(ifstream &ifile)
    {
        for (int i = 0; i < noOfPages; i++)
        {
            page[i] = new Page(ifile);
        }
    }
    void ReadPostsFromFile(ifstream &ifile)
    {
        ifile.ignore();
        ifile.ignore(100, '\n');

        char ***likersList = new char **[noOfPosts]; // Changed noOfUsers to noOfPosts
        for (int i = 0; i < noOfPosts; i++)          // Changed noOfUsers to noOfPosts
        {
            likersList[i] = new char *[User::getMaxNoOfLikedPages()]; // Allocate memory for likersList[i]
            for (int j = 0; j < User::getMaxNoOfLikedPages(); j++)
            {
                likersList[i][j] = new char[maxIDLength]; // Allocate memory for likersList[i][j]
            }
        }

        for (int i = 0; i < noOfPosts; i++) // Changed noOfUsers to noOfPosts
        {
            int noOfLikes = 0;
            string ownerId;

            post[i] = new Post(ifile, ownerId, likersList[i], noOfLikes);

            Account *owner = (ownerId[0] == 'u') ? (Account *)SearchUserByID(ownerId) : (Account *)SearchPageByID(ownerId);

            if (owner)
            {
                owner->AddPost(post[i]);
                post[i]->SetOwner(owner);

                for (int j = 0; j < noOfLikes; j++)
                {
                    Account *liker = (likersList[i][j][0] == 'u') ? (Account *)SearchUserByID(likersList[i][j]) : (Account *)SearchPageByID(likersList[i][j]);
                    if (liker)
                        post[i]->AddLiker(liker);
                }
            }

            ifile.ignore();
            ifile.ignore(100, '\n');
        }

        // Deallocate memory for likersList
        if (likersList)
        {
            for (int i = 0; i < noOfPosts; i++) // Changed noOfUsers to noOfPosts
            {
                if (likersList[i])
                {
                    for (int j = 0; j < User::getMaxNoOfLikedPages(); j++)
                    {
                        delete[] likersList[i][j];
                    }
                    delete[] likersList[i];
                }
            }
            delete[] likersList;
        }
    }
    void ReadCommentsFromFile(ifstream &ifile)
    {
        for (int i = 0; i < totalNoOfComments; i++)
        {
            string ownerID, postID;

            Account *owner = nullptr;
            Post *post = nullptr;
            Comment *temp = new Comment(ifile, postID, ownerID);
            if (!ownerID.empty())
            {
                owner = (ownerID[0] == 'p') ? (Account *)SearchPageByID(ownerID) : SearchUserByID(ownerID);

                post = SearchPostByID(postID);
                if (post)
                {
                    post->AddComment(temp);
                    if (owner)
                        temp->setAuthor(owner);
                    break;
                }
                else
                {
                    delete temp;
                    temp = nullptr;
                }
            }
        }
    }
    void SetupUsersFriends(string ***userFriends, int *friendsCount)
    {
        for (int i = 0; i < noOfUsers; i++)
        {
            for (int j = 0; j < friendsCount[i]; j++)
            {
                User *newFriend = SearchUserByID(*userFriends[i][j]);
                if (newFriend)
                {
                    user[i]->AddFriend(newFriend);
                }
            }
        }
    }
    void SetupUsersLikedPages(string ***userLikedPages, int *likedPagesCount)
    {
        for (int i = 0; i < noOfUsers; i++)
        {
            for (int j = 0; j < likedPagesCount[i]; j++)
            {
                Page *likedPage = SearchPageByID(*userLikedPages[i][j]);
                if (likedPage)
                {
                    user[i]->LikePage(likedPage);
                    likedPage->AddLiker(user[i]);
                }
            }
        }
    }
    User *SearchUserByID(const string &userID)
    {
        string idPrefix = "u";

        if (userID.compare(0, idPrefix.size(), idPrefix) != 0)
        {
            return nullptr;
        }

        for (int i = 0; i < noOfUsers; i++)
        {
            if (user[i] && user[i]->getAccountID() == userID)
            {
                return user[i];
            }
        }

        return nullptr;
    }
    Page *SearchPageByID(const string &pageID)
    {
        string idPrefix = "page";
        if (pageID.compare(0, idPrefix.size(), idPrefix) != 0)
        {
            return nullptr;
        }

        for (int i = 0; i < noOfPages; i++)
        {
            if (page[i] && page[i]->getAccountID() == pageID)
            {
                return page[i];
            }
        }

        return nullptr;
    }
    Post *SearchPostByID(const string &postID)
    {
        string idPrefix = "post";

        if (postID.compare(0, idPrefix.size(), idPrefix) != 0)
        {
            return nullptr;
        }

        for (int i = 0; i < noOfPosts; i++)
        {
            if (post[i] && post[i]->getID() == postID)
            {
                return post[i];
            }
        }
        return nullptr;
    }

    void DeleteUser(User *&userPtr)
    {
        if (!userPtr)
            return;
        DeletePostsOfAuthor(userPtr);

        if (user)
        {
            for (int i = 0; i < noOfUsers; i++)
                if (user[i])
                    user[i]->RemoveFriend(userPtr);
        }

        if (page)
        {
            for (int i = 0; i < noOfPages; i++)
                if (page[i])
                    page[i]->RemoveLiker(userPtr);
        }

        if (post)
        {
            for (int i = 0; i < noOfPosts; i++)
                if (post[i])
                    post[i]->RemoveCommentsOfAccount(userPtr);
        }

        delete userPtr;
        userPtr = nullptr;
    }

    void DeletePage(Page *&pagePtr)
    {
        if (!pagePtr)
            return;
        DeletePostsOfAuthor(pagePtr);

        if (user)
        {
            for (int i = 0; i < noOfPages; i++)
            {
                if (user[i])
                {
                    user[i]->UnlikePage(pagePtr);
                }
            }
        }

        if (page)
        {
            for (int i = 0; i < noOfPages; i++)
            {
                if (page[i])
                {
                    page[i]->RemoveLiker(pagePtr);
                }
            }
        }

        if (post)
        {
            for (int i = 0; i < noOfPosts; i++)
            {
                if (post[i])
                {
                    post[i]->RemoveCommentsOfAccount(pagePtr);
                }
            }
        }

        delete pagePtr;
        pagePtr = nullptr;
    }

    void DeletePostsOfAuthor(Account *accPtr)
    {
        if (post)
        {
            for (int i = 0; i < noOfPosts; i++)
            {
                if (post[i] && post[i]->getOwner() == accPtr)
                {
                    delete post[i];
                    post[i] = nullptr;

                    for (i = i + 1; i < noOfPosts; i++)
                    {
                        post[i - 1] = post[i];
                    }

                    noOfPosts--;
                    post[noOfPosts] = nullptr;
                }
            }
        }
    }

    void SetCurrentUser(const string &userID)
    {
        string idPrefix = "u";

        if (userID.compare(0, idPrefix.size(), idPrefix) != 0)
        {
            return;
        }

        currentUser = SearchUserByID(userID);
        if (currentUser)
        {
            currentUser->PrintName();
            cout << " successfully set as Current User." << endl;
        }
    }

    void ViewHome()
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return;
        }
        currentUser->ViewHome();
    }

    void ViewTimeline()
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return;
        }
        currentUser->ViewTimeline();
    }

    void ViewPostLikedList(const string &postID)
    {
        Post *post = SearchPostByID(postID);
        if (!post)
        {
            cout << "Post not found!" << endl;
            return;
        }
        post->PrintLikedList();
    }

    bool LikePost(const string &postID)
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return false;
        }

        Post *post = SearchPostByID(postID);
        if (post)
        {
            return post->AddLiker(currentUser);
        }
        return false;
    }

    bool PostComment(const string &postID, const string &text)
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return false;
        }

        Post *post = SearchPostByID(postID);
        if (post)
        {
            Comment *newComment = new Comment(('c' + to_string(totalNoOfComments++)).c_str(), text, currentUser);
            bool status = post->AddComment(newComment);

            if (status)
                return true;

            delete newComment;
            return false;
        }
        return false;
    }

    void ViewPost(const string &postID)
    {
        Post *post = SearchPostByID(postID);
        if (post)
        {
            post->Print(true);
        }
    }

    void ViewFriendList()
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return;
        }
        currentUser->PrintFriendList();
    }

    void ViewLikedPagesList()
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return;
        }
        currentUser->PrintLikedPagesList();
    }

    void ViewPage(const string &pageID)
    {
        Page *page = SearchPageByID(pageID);
        if (page)
        {
            page->ViewTimeline();
        }
    }

    void PrintMemories()
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return;
        }

        cout << '\n'
             << "We hope you enjoy looking back and sharing your memories on our app,";
        cout << "from the most recent to those long ago." << '\n'
             << '\n';

        currentUser->PrintMemories();
    }

    bool ShareMemory(const string &postID, const string &body)
    {
        if (!currentUser)
        {
            cout << "Please set the current user first" << endl;
            return false;
        }

        Post *post = SearchPostByID(postID);

        if (post && post->getOwner() == currentUser)
        {
            Memory *newPost = new Memory(("post" + to_string(noOfPosts + 1)).c_str(), body, Date::getTodaysDate(), currentUser, post);
            bool status = currentUser->AddPost(newPost);

            if (!status)
            {
                delete newPost;
                return false;
            }

            Post **newPostlist = new Post *[noOfPosts + 1];

            for (int i = 0; i < noOfPosts; i++)
            {
                newPostlist[i] = this->post[i];
            }
            newPostlist[noOfPosts++] = newPost;

            delete[] this->post;
            this->post = newPostlist;
            return true;
        }

        return false;
    }

    void run()
    {
        Date newDate;

        SocialMediaApp::ReadDataFromFile();

        const int noOfCommands = 14;
        // List of valid Commands - indexes synced with switch case below
        string validCommands[noOfCommands] = {
            "Set current system date",
            "Set current user",
            "View Home",
            "Like Post",
            "View Liked List",
            "Post Comment",
            "View Post",
            "View Memories",
            "Share Memory",
            "View Timeline",
            "View Friend List",
            "View Liked Pages",
            "View Page",
            "Exit"};

        // Commands to execute
        string commands[] = {
            "Set current system date", "Set current user", "View Friend List", "View Liked Pages", "View Home",
            "View Timeline", "View Liked List", "Like Post", "View Liked List", "Post Comment",
            "View Post", "Post Comment", "View Post", "View Memories", "Share Memory",
            "View Timeline", "View Page", "Set current user", "View Home", "View Timeline",
            "Exit"};

        // First arg
        string args[] = {
            "15 11 2017", "u7", "", "", "",
            "", "post5", "post5", "post5", "post4",
            "post4", "post8", "post8", "", "post10",
            "", "p1", "u11", "", "",
            ""};

        // Second arg
        string texts[] = {
            "", "", "", "", "",
            "", "", "", "", "Good luck for your results",
            "", "Thanks for the wishes", "", "", "Never thought I'd be a specialist in this field",
            "", "", "", "", "",
            ""};

        bool exit = false;

        int commandNo = 0;

        while (!exit && commandNo < noOfCommands)
        {
            cout << "---------------------------------------------------------------------------------------------------------------" << endl;
            cout << "Command:" << '\t' << commands[commandNo] << '\n';
            cout << "---------------------------------------------------------------------------------------------------------------" << endl;

            string inputID = args[commandNo];
            string body = texts[commandNo];

            int commandNumber = -1;
            for (int i = 0; i < noOfCommands; i++)
            {
                if (commands[commandNo] == validCommands[i])
                {
                    commandNumber = i;
                    break;
                }
            }

            switch (commandNumber)
            {
            case 0: // Set current system date
            {
                cout << "Enter new system date (dd mm yyyy) : ";
                cin >> newDate;
                cout << inputID << '\n';
                newDate = Date(15, 11, 2017);

                Date::setTodaysDate(newDate);

                cout << "System Date:" << '\t';
                Date::getTodaysDate().Print();
                cout << '\n';
                break;
            }

            case 1: // Set current user
            {
                cout << "Enter user id (u<id>) : ";
                cin >> inputID;
                cin.ignore();

                SocialMediaApp::SetCurrentUser(inputID);
                break;
            }

            case 2: // View Home
            {
                SocialMediaApp::ViewHome();
                break;
            }

            case 3: // Like Post
            {
                cout << "Enter post id (post<id>) : ";
                cin >> inputID;
                cin.ignore();

                SocialMediaApp::LikePost(inputID);
                break;
            }

            case 4: // View Liked List
            {
                cout << "Enter post id (post<id>) : ";
                cin >> inputID;
                cin.ignore();

                SocialMediaApp::ViewPostLikedList(inputID);
                break;
            }

            case 5: // Post Comment
            {
                cout << "Enter post id (post<id>): ";
                cin >> inputID;

                cout << "Enter Comment text: ";
                cin.ignore();
                getline(cin, body);

                SocialMediaApp::PostComment(inputID, body);
                break;
            }

            case 6: // View Post
            {
                cout << "Enter post id (post<id>) : ";
                cin.ignore();
                cin >> inputID;
                cout << endl;

                SocialMediaApp::ViewPost(inputID);
                break;
            }

            case 7: // View Memories
            {
                SocialMediaApp::PrintMemories();
                break;
            }

            case 8: // Share Memory
            {
                cout << "Enter post id (post<id>): ";
                cin.ignore();
                cin >> inputID;

                cout << "Enter Post body: ";
                cin.ignore();
                getline(cin, body);

                SocialMediaApp::ShareMemory(inputID, body);
                break;
            }

            case 9: // View Timeline
            {
                SocialMediaApp::ViewTimeline();
                break;
            }

            case 10: // View Friend List
            {
                SocialMediaApp::ViewFriendList();
                break;
            }

            case 11: // View Liked Pages
            {
                SocialMediaApp::ViewLikedPagesList();
                break;
            }

            case 12: // View Page
            {
                cout << "Enter page id (p<id>) : ";
                cin.ignore();
                cin >> inputID;

                SocialMediaApp::ViewPage(inputID);
                break;
            }

            case 13: // Exit
            {
                cout << "Exiting..." << endl;
                exit = true;
                break;
            }

            default:

                cout << "Invalid Command" << endl;
                break;

                commandNo++;
            }
        }
    }
};

SocialMediaApp *SocialMediaApp::instance = nullptr;

int main()
{
    SocialMediaApp *app = SocialMediaApp::getInstance();
    app->run();

    delete app;
    return 0;
}