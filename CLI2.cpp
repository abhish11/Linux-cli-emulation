    #include <iostream>
    #include <string>
    #include <set>
    #include <map>
    #include <vector>
    #include <stack>
    using namespace std;
     
    class playment
    {
     
    private:
        const string root = "/";
        vector<string> path; // current path
        set<string> folders; // list of absolute paths
     
        std::vector<string> parsePath(string path)
        {
            std::vector<string> S;
            string tmp = "";
            if (path[0] == '/')
                S.push_back(root);
            for (char x : path)
            {
                if (x == ' ' || x == '/')
                {
                    if (tmp != "")
                        S.push_back(tmp);
                    tmp = "";
                    continue;
                }
                tmp += x;
            }
            if (tmp != "")
                S.push_back(tmp);
            return S;
        }
     
        std::vector<string> parseCommand(string path)
        {
            vector<string> seq_of_paths;
            string tmp = "";
            string path2 = "";
            for (int i = 0; i < path.length(); ++i)
            {
                char x = path[i];
                if (x == ' ')
                {
                    path2 = path.substr(i + 1);
                    if (tmp != "")
                    {
                        seq_of_paths.push_back(tmp);
                        tmp = "";
                        break;
                    }
                    else
                    {
                        tmp = "";
                        continue;
                    }
                }
                tmp += x;
            }
            if (tmp != "")
                seq_of_paths.push_back(tmp);
     
            std::vector<string> tmpp = parsePath(path2);
            for (string x : tmpp)
                seq_of_paths.push_back(x);
            return seq_of_paths;
        }
     
        string pwd()
        {
     
            string ans = "";
            for (string x : path)
            {
                if (x == "/")
                    continue;
                ans += '/';
                ans += x;
            }
            if (ans == "")
                ans = "/";
     
            //ans+='\n';
            return ans;
        }
     
        int match(string path1, string path2)
        {
            if (path2.length() < path1.length())
                return -1;
            for (int i = 0; i < path1.length(); ++i)
            {
                if (path1[i] != path2[i])
                    return -1;
                ;
            }
            return path1.length();
        }
     
        string ls()
        {
     
            // std::vector<string> S = parseCommand(command);
            string curr = pwd();
            if (curr[curr.length() - 1] != '/')
                curr += '/';
            string ans = "";
            set<string> unique;
            for (string p : folders)
            {
                int pos = match(curr, p);
                if (pos == -1 || pos >= p.length())
                    continue;
                vector<string> s = parsePath(p.substr(pos));
                unique.insert(s[0]);
            }
            cout << "DIRS: ";
            for (string s : unique)
                ans += s + ' ';
            ans += '\n';
            return ans;
        }
     
        string session_clear(string command)
        {
            std::vector<string> S = parseCommand(command);
            if ((int)S.size() == 2 && S[1] == "clear")
            {
                folders.clear();
                folders.insert(root);
                path.clear();
                path.push_back(root);
                return "SUCC: CLEARED: RESET TO ROOT\n";
            }
            return "ERR: CANNOT RECOGNIZE INPUT.\n";
        }
     
        string mkdir(string command)
        {
            std::vector<string> S = parseCommand(command);
            int n = S.size();
            if(n == 1) return "ERR: INVALID PATH\n\n";
     
            if (S[1] == "/")
            {
                // start from root
                string fullpath = "/";
                for (int i = 2; i < n - 1; ++i)
                {
                    fullpath += S[i];
                    if (i < n - 2)
                        fullpath += '/';
                }
                if (folders.find(fullpath) != folders.end())
                {
                    if (fullpath[fullpath.length() - 1] != '/')
                        fullpath += '/';
                    fullpath += S[n - 1];
                    if (folders.find(fullpath) == folders.end())
                    {
                        folders.insert(fullpath);
                        return "SUCC: CREATED\n";
                    }
                    else
                        return "ERR: DIRECTORY ALREADY EXISTS\n";
                }
            }
            else
            {
                // start from cureent
                string fullpath = pwd();
                if (fullpath[fullpath.length() - 1] != '/')
                    fullpath += '/';
                for (int i = 1; i < n - 1; ++i)
                {
                    if (S[i] == ".")
                        continue;
                    fullpath += S[i];
                    if (i < n - 2)
                        fullpath += '/';
                }
                string tmpp = "";
                if (fullpath[fullpath.length() - 1] == '/')
                {
                    for (int i = 0; i < fullpath.length() - 1; ++i)
                        tmpp += fullpath[i];
                }
                if (tmpp != "")
                    fullpath = tmpp;
                // cout << "full " << fullpath << endl;
                if (folders.find(fullpath) != folders.end())
                {
                    if (fullpath[fullpath.length() - 1] != '/')
                        fullpath += '/';
                    fullpath += S[n - 1];
                    if (folders.find(fullpath) == folders.end())
                    {
                        folders.insert(fullpath);
                        return "SUCC: CREATED\n";
                    }
                    else
                        return "ERR: DIRECTORY ALREADY EXISTS\n";
                }
            }
            return "ERR : INVALID PATH\n";
        }
     
        string rmdir(string command)
        {
     
            std::vector<string> S = parseCommand(command);
            int n = S.size();
            if(n == 1) return "ERR : NO SUCH FILE OR DIRECTORY\n\n";
     
            if (S[1] == "/")
            {
                // start from root
                string fullpath = "/";
                for (int i = 2; i < n - 1; ++i)
                {
                    fullpath += S[i];
                    if (i < n - 2)
                        fullpath += '/';
                }
                if (folders.find(fullpath) != folders.end())
                {
                    if (fullpath[fullpath.length() - 1] != '/')
                        fullpath += '/';
                    fullpath += S[n - 1];
                    int x = match(fullpath, pwd());
                    if (folders.find(fullpath) == folders.end())
                        return "ERR: NO SUCH FILE OR DIRECTORY\n\n";
                    if (x == -1)
                    {
                    	std::vector<string> toerase;
                    	for (string s : folders) {
                    		if (match(fullpath, s) != -1)
                    			toerase.push_back(s);
                    	}
                    	for (string s : toerase)
                        	folders.erase(s);
                        // folders.erase(fullpath);
                        return "SUCC: DELETED\n";
                    }
                    else
                        return "ERR: CANNOT DELETE BASE PATH\n";
                }
            }
            else
            {
                // start from cureent
                string fullpath = pwd();
                if (fullpath[fullpath.length() - 1] != '/')
                    fullpath += '/';
                for (int i = 1; i < n - 1; ++i)
                {
                    if (S[i] == ".")
                        continue;
                    fullpath += S[i];
                    if (i < n - 2)
                        fullpath += '/';
                }
                string tmpp = "";
                if (fullpath[fullpath.length() - 1] == '/')
                {
                    for (int i = 0; i < fullpath.length() - 1; ++i)
                        tmpp += fullpath[i];
                }
                if (tmpp != "")
                    fullpath = tmpp;
                if (folders.find(fullpath) != folders.end())
                {
                    if (fullpath[fullpath.length() - 1] != '/')
                        fullpath += '/';
                    fullpath += S[n - 1];
     
                    int x = match(fullpath, pwd());
                    if (folders.find(fullpath) == folders.end())
                        return "ERR : NO SUCH FILE OR DIRECTORY\n\n";
                    if (x == -1)
                    {
                    	std::vector<string> toerase;
                    	for (string s : folders) {
                    		if (match(fullpath, s) != -1) {
                    			toerase.push_back(s);
                    			// cout << "s " << s << endl;
                    		}
                    	}
                    	for (string s : toerase)
                        	folders.erase(s);
                        return "SUCC: DELETED\n";
                    }
                    else
                        return "ERR: CANNOT DELETE BASE PATH\n";
                }
            }
            return "ERR: Failed to delete\n";
        }
     
        string cd(string command)
        {
            std::vector<string> S = parseCommand(command);
            int n = S.size();
            if(n == 1) return "ERR: INVALID PATH\n";
     
            std::vector<string> tmp;
     
            if (S[1] == "/")
                tmp.push_back(root);
            else
                tmp = path;
     
            std::vector<string> Sp;
            for (int i = 1; i < S.size(); ++i)
                Sp.push_back(S[i]);
            S = Sp;
     
            for (string x : S)
            {
                if (x == "/" || x == ".")
                    continue;
                if (x == "..")
                {
                    if ((int)tmp.size() > 1)
                        tmp.pop_back();
                    else
                        return "ERR: ALREADY AT ROOT\n";
                }
                else
                    tmp.push_back(x);
            }
            string tmppath = "";
            for (string s : tmp)
            {
                if (s == "/")
                    continue;
                tmppath += "/";
                tmppath += s;
            }
            if (tmppath == "")
                tmppath += '/';
            if (folders.find(tmppath) != folders.end())
            {
                path = tmp;
                return "SUCC: REACHED\n";
            }
            return "ERR: INVALID PATH\n";
        }

        
     
    public:
        playment()
        {
            folders.insert(root);
            path.push_back(root);
        }


        //
        void listall() {
        	cout << "----\n";
        	for (string s : folders) cout << s << endl;
        		cout << "---\n";
        }
     
        string exec(string command)
        {
            vector<string> S = parseCommand(command);
            string action = S[0];
            if (action == "pwd")
                return "PATH:  " + pwd();
            else if (action == "ls")
                return ls();
            else if (action == "mkdir")
                return mkdir(command);
            else if (action == "session")
                return session_clear(command);
            else if (action == "cd")
                return cd(command);
            else if (action == "rm")
                return rmdir(command);
     
            return "ERR: CANNOT RECOGNIZE INPUT\n";
        }
     
        ~playment()
        {
        }
    };
    int main(int argc, char const *argv[])
    {
     
        cout << "$ <Starting your application...>" << endl;
        playment p;
        while (1)
        {
            cout<<"$ ";
            string s;
            getline(cin, s);
     
            if (s == "\n" || s.length() == 0)
                continue;
            cout << p.exec(s) << endl;
            // p.listall();
     
        }
     
        return 0;
    }