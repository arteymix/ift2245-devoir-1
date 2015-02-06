/* File system */
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <sys/prctl.h>

/* Streams */
#include <iostream>
#include <fstream>   /* File stream */

/* Data structures */
#include <list>
#include <map>
#include <string>

using namespace std;

typedef struct pid_node {
    long       ppid; /* méta-donnée */
    long       pid;  /* méta-donnée */
    char*      name;
    list<long> children;
} pid_node;

map<long, pid_node> pids;

/**
 * Imprime un noeud récursivement.
 *
 * @param node   noeud à imprimer
 * @param indent indentation pour l'impression
 */
void pid_node_pretty_print (pid_node node, int indent = 0)
{
    int remaining = indent;
    while (remaining--)
        cout << "    "; /* 4 espaces */

    cout << node.pid << " " << node.name << endl;

    /* boucle dans les enfants */
    for (list<long>::iterator child = node.children.begin (); child != node.children.end (); child++)
        pid_node_pretty_print (pids[*child], indent + 1);
}

int main(int argc, char* argv[])
{
    struct dirent *entry;

    pids[0] = pid_node { .ppid = 0, .pid = 0, .name = "[SCHED]" };

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

        /*
         * Prépare un buffer pour lire le nom du processus.
         *
         * La taille maximale du nom d'un processus est définit par la constante
         * PS_GET_NAME.
         */
        node.name     = new char[PR_GET_NAME];
        node.children = pids[node.pid].children; /* reprends les child si ils sont déjà défini */

        // skip "<pid> (" dans stat
        f.ignore(strlen(entry->d_name) + 2);

        // lit le nom du pid
        f.getline (node.name, PR_GET_NAME, ')');

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

        // ajoute le noeud au map
        pids[node.pid] = node;
    }

    pid_node_pretty_print (pids[0]);
}
