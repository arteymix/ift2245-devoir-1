/* File system */
#include <dirent.h>

#include <stdlib.h>
#include <string.h>

/* Streams */
#include <iostream>  /* IO stream */
#include <fstream>   /* File stream */
#include <sstream>   /* String stream */

/* Data structures */
#include <string>
#include <list>
#include <map>

using namespace std;

typedef struct pid_node {
    long  pid;
    char* name;
    long* children;
} pid_node;

int main(int argc, char* argv[])
{
    DIR *directory;
    struct dirent *entry;
    map<long, pid_node> pids;

    directory = opendir("/proc");

    if (directory == NULL)
        return EXIT_FAILURE;

    while (entry = readdir(directory))
    {
        if (entry->d_type != DT_DIR)
            continue;

        // stocke l'information du pid
        pid_node node;

        /* test si le nom du dossier est un long */
        char *err = NULL;
        node.pid = strtol(entry->d_name, &err, 10);
        if (*err)
            continue;

        string filename ("/proc/");
        filename.append (entry->d_name);
        filename.append ("/stat");

        ifstream f;
        f.open (filename);

        if (!f.is_open())
            return EXIT_FAILURE;

        node.name     = new char[256];
        node.children = pids[node.pid].children;

        // skip "<pid> (" dans stat
        f.ignore(strlen(entry->d_name) + 2);

        // lit le nom du pid
        char *name_b = node.name;
        while (f.peek() != ')') {
            f.read (node.name++, 1);
        }
        *node.name = '\0';
        node.name = name_b;

        // skip ") S "
        f.ignore (4);

        // lit le pid du parent
        char *ppid = new char[16];
        char *ppid_b = ppid;
        while (f.peek() != ' ') {
            f.read (ppid++, 1);
        }
        *ppid = '\0';
        ppid = ppid_b;

        cout << "pid: " << node.pid << endl;
        cout << "name: " << node.name << endl;
        cout << "ppid: " << ppid << endl << endl;
    }
}
