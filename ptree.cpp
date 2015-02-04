/* File system */
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/* Streams */
#include <fstream>   /* File stream */

/* Data structures */
#include <list>
#include <map>
#include <string>

using namespace std;

typedef struct pid_node {
    long       ppid;
    long       pid;
    char*      name;
    list<long> children;
} pid_node;

int main(int argc, char* argv[])
{
    struct dirent *entry;
    map<long, pid_node> pids;

    DIR *directory = opendir("/proc");

    if (directory == NULL)
        return EXIT_FAILURE;

    while (entry = readdir(directory))
    {
        // considère seulement les dossiers
        if (entry->d_type != DT_DIR)
            continue;

        // stocke l'information du pid
        pid_node node;

        /* test si le nom du dossier est un long */
        char *err = NULL;
        node.pid = strtol(entry->d_name, &err, 10);
        if (*err)
            continue;

        /* construit le chemin vers stat */
        string filename ("/proc/");
        filename.append (entry->d_name);
        filename.append ("/stat");

        ifstream f;
        f.open (filename);

        if (!f.is_open())
            return EXIT_FAILURE;

        node.name     = new char[256];
        node.children = pids[node.pid].children; /* reprends les child si ils sont déjà défini */

        // skip "<pid> (" dans stat
        f.ignore(strlen(entry->d_name) + 2);

        // lit le nom du pid
        f.getline (node.name, 256, ')');

        // skip le status " S "
        f.ignore (3);

        // lit le pid du parent
        char *ppid = new char[16];
        f.getline (ppid, 16, ' ');
        node.ppid = strtol (ppid, NULL, 10);

        // ajout du ppid dans le map si il est manquant
        if (pids.find (node.ppid) == pids.end ())
        {
            pid_node parent_node;
            /* tout ce qu'on sait c'est le pid du parent */
            parent_node.pid = strtol(ppid, NULL, 10);

            pids[node.ppid] = parent_node;
        }

        // ajoute de l'enfant au parent
        pids[node.ppid].children.push_back (node.pid);
    }
}
