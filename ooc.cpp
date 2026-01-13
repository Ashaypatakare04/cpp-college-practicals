#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
using namespace std;

// ====================== COMMENT CLASS ======================
class Comment {
private:
    string user;
    string text;

public:
    Comment() {}
    Comment(const string &u, const string &t) : user(u), text(t) {}

    void display() const {
        cout << user << ": " << text << "\n";
    }

    string serialize(int postId) const {
        return to_string(postId) + "|" + user + "|" + text + "\n";
    }

    static Comment deserialize(const string &line, int &postId) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);

        postId = stoi(line.substr(0, p1));
        string u = line.substr(p1 + 1, p2 - p1 - 1);
        string t = line.substr(p2 + 1);

        return Comment(u, t);
    }
};

// ====================== POST CLASS ======================
class Post {
private:
    int id;
    string title, content, category, author;
    int agree = 0, disagree = 0, neutral = 0;
    vector<Comment> comments;

public:
    Post() {}
    Post(int pid, string t, string c, string cat, string a)
        : id(pid), title(t), content(c), category(cat), author(a) {}

    int getId() const { return id; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    const string& getCategory() const { return category; }

    void vote(int choice) {
        if (choice == 1) agree++;
        else if (choice == 2) disagree++;
        else if (choice == 3) neutral++;
    }

    void addComment(const Comment &c) {
        comments.push_back(c);
    }

    void displayShort() const {
        cout << "\n[" << id << "] " << title
             << "\nBy: " << author
             << "\nCategory: " << category
             << "\nVotes: 👍" << agree << " | 👎" << disagree << " | 😐" << neutral
             << "\n-------------------------\n";
    }

    void displayFull() const {
        displayShort();
        cout << "Content: " << content << "\n\n--- Comments ---\n";
        if (comments.empty()) cout << "No comments yet.\n";
        else for (auto &c : comments) c.display();
    }

    string serialize() const {
        return to_string(id) + "|" + title + "|" + content + "|" + category + "|" +
               author + "|" + to_string(agree) + "|" + to_string(disagree) + "|" +
               to_string(neutral) + "\n";
    }

    static Post deserialize(const string &line) {
        vector<string> parts;
        string temp;
        for (char ch : line) {
            if (ch == '|') { parts.push_back(temp); temp = ""; }
            else temp += ch;
        }
        parts.push_back(temp);

        Post p(stoi(parts[0]), parts[1], parts[2], parts[3], parts[4]);
        p.agree = stoi(parts[5]);
        p.disagree = stoi(parts[6]);
        p.neutral = stoi(parts[7]);

        return p;
    }
};

// ====================== POST MANAGER CLASS ======================
class PostManager {
private:
    vector<Post> posts;
    int counter = 1;

    void clearInput() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Post* findPostById(int id) {
        for (auto &p : posts)
            if (p.getId() == id)
                return &p;
        return nullptr;
    }

public:
    PostManager() {
        loadData();
    }

    // ----------- SAVE SYSTEM -----------
    void savePost(const Post &p) {
        ofstream f("posts.txt", ios::app);
        f << p.serialize();
        f.close();
    }

    void saveComment(int postId, const Comment &c) {
        ofstream f("comments.txt", ios::app);
        f << c.serialize(postId);
        f.close();
    }

    void saveVote(int postId, int choice) {
        ofstream f("votes.txt", ios::app);
        f << postId << "|" << choice << "\n";
        f.close();
    }

    // ----------- LOAD SYSTEM -----------
    void loadData() {
        // Load posts
        ifstream f("posts.txt");
        string line;
        while (getline(f, line)) {
            Post p = Post::deserialize(line);
            posts.push_back(p);
            counter = max(counter, p.getId() + 1);
        }
        f.close();

        // Load comments
        ifstream fc("comments.txt");
        while (getline(fc, line)) {
            int pid;
            Comment c = Comment::deserialize(line, pid);
            Post *p = findPostById(pid);
            if (p) p->addComment(c);
        }
        fc.close();

        // Load votes
        ifstream fv("votes.txt");
        while (getline(fv, line)) {
            size_t pos = line.find('|');
            int pid = stoi(line.substr(0, pos));
            int choice = stoi(line.substr(pos + 1));

            Post *p = findPostById(pid);
            if (p) p->vote(choice);
        }
        fv.close();
    }

    // ====================== FEATURES ======================

    void createPost() {
        clearInput();

        string t, c, cat, a;
        cout << "Enter title: ";
        getline(cin, t);
        cout << "Enter content: ";
        getline(cin, c);
        cout << "Enter category: ";
        getline(cin, cat);
        cout << "Enter your name: ";
        getline(cin, a);

        Post p(counter++, t, c, cat, a);
        posts.push_back(p);
        savePost(p);

        cout << "Post created!\n";
    }

    void viewPosts() {
        if (posts.empty()) { cout << "No posts available.\n"; return; }
        for (auto &p : posts) p.displayShort();
    }

    void viewPostDetails() {
        if (posts.empty()) { cout << "No posts available.\n"; return; }

        int id;
        cout << "Enter Post ID: ";
        cin >> id;

        Post *p = findPostById(id);
        if (p) p->displayFull();
        else cout << "Post not found.\n";
    }

    void votePost() {
        if (posts.empty()) { cout << "No posts available.\n"; return; }

        int id, choice;
        cout << "Enter Post ID: ";
        cin >> id;

        Post *p = findPostById(id);
        if (!p) { cout << "Post not found.\n"; return; }

        cout << "1. Agree\n2. Disagree\n3. Neutral\nChoose: ";
        cin >> choice;

        if (choice < 1 || choice > 3) cout << "Invalid choice.\n";
        else {
            p->vote(choice);
            saveVote(id, choice);
            cout << "Vote recorded.\n";
        }
    }

    void addComment() {
        if (posts.empty()) { cout << "No posts to comment on.\n"; return; }

        int id;
        cout << "Enter Post ID to comment: ";
        cin >> id;
        clearInput();

        Post *p = findPostById(id);
        if (!p) { cout << "Post not found.\n"; return; }

        string u, text;
        cout << "Your name: ";
        getline(cin, u);
        cout << "Comment: ";
        getline(cin, text);

        Comment c(u, text);
        p->addComment(c);
        saveComment(id, c);

        cout << "Comment added!\n";
    }
};

// ====================== MAIN ======================
int main() {
    PostManager pm;
    int choice;

    do {
        cout << "\n====== PulseVoice ======\n";
        cout << "1. Create Post\n";
        cout << "2. View All Posts\n";
        cout << "3. View Post Details\n";
        cout << "4. Vote on Post\n";
        cout << "5. Add Comment\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: pm.createPost(); break;
            case 2: pm.viewPosts(); break;
            case 3: pm.viewPostDetails(); break;
            case 4: pm.votePost(); break;
            case 5: pm.addComment(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}