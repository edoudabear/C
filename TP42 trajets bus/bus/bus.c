#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "heapq.h"

//-----------------------------------
// Structures

struct stop {
    char* name;
    double lat, lng;
    bool transfer;
};

typedef struct stop Stop;

struct time {
    int route;
    int stop;
    int time;
    int n_route;
    int n_stop;
    int n_transfer;
};

typedef struct time Time;
void reachable_aux(int stop,int time,Time times[],int nb_times, Stop stops[],int nb_stops,bool* visited);

//-----------------------------------
// Extract data from files

char* strdup_(char* s) {
    int len = strlen(s);
    char* result = (char*)malloc((len+1)*sizeof(char));
    if (result == NULL) { return NULL; }
    memcpy(result, s, len+1);
    return result;
}

Stop* load_stops(char* filename, int* nb_stops) {
    // Open file
    FILE* f = fopen(filename, "r");
    if (f==NULL) { fprintf(stderr, "Can't open stops file <%s>\n", filename); exit(-1); }

    // First line (number of stops)
    int nb;
    if (fscanf(f, "%d\n", &nb) < 1) { fprintf(stderr, "Error reading stops file <%s> line 1\n", filename); exit(-1); }

    Stop* stops = (Stop*)malloc(nb*sizeof(Stop));
    if (stops==NULL) { fprintf(stderr, "Can't allocate stops array\n"); exit(-1); }

    for (int i=0; i<nb; ++i) {
        int id = -1, transfer;
        Stop s;
        char name[1000];
        if ((fscanf(f, "%d;%lf;%lf;%d;%s\n", &id, &s.lat, &s.lng, &transfer, name)<5) || id<0 || id>=nb) {
            fprintf(stderr, "Error reading stops file <%s> line %d, stop %d (%s)\n", filename, i+2, id, name); exit(-1);
        }
        s.name = strdup_(name);
        s.transfer = transfer != 0;
        stops[i] = s;
    }

    // Close file
    fclose(f);
    *nb_stops = nb;
    return stops;
}

Time* load_times(char* filename, int* nb_times) {
    // Open file
    FILE* f = fopen(filename, "r");
    if (f==NULL) { fprintf(stderr, "Can't open timestamps file <%s>\n", filename); exit(-1); }

    // First line (number of timestamps)
    int nb;
    if (fscanf(f, "%d\n", &nb) < 1) { fprintf(stderr, "Error reading timestamps file <%s> line 1\n", filename); exit(-1); }

    Time* times = (Time*)malloc(nb*sizeof(Time));
    if (times==NULL) { fprintf(stderr, "Can't allocate timestamps array\n"); exit(-1); }

    for (int i=0; i<nb; ++i) {
        if ((fscanf(f, "%d;%d;%d\n", &times[i].route, &times[i].stop, &times[i].time)<3)) {
            fprintf(stderr, "Error reading timestamps file <%s> line %d\n", filename, i+2); exit(-1);
        }
        times[i].n_stop = -1;
        times[i].n_route = -1;
        times[i].n_transfer = -1;
    }

    // Close file
    fclose(f);
    *nb_times = nb;
    return times;
}

//-----------------------------------
// Graph construction

void build_graph(Time times[], int nb_times) {
    // Pour n_route
    for (int i=0;i<nb_times;++i) {

        times[i].n_route=-1;
        for (int j=i+1;j<nb_times;j++) {
            if (times[j].route==times[i].route && times[j].time>times[i].time) {
                times[i].n_route=j;
                break;
            }
        }
    }
    // Pour n_stop
    for (int i=0;i<nb_times;++i) {
        int min_t=times[i].time,min_i=-1;
        for (int j=0;j<nb_times;++j) {
            if ( (times[i].time<=times[j].time && times[i].stop==times[j].stop && i!=j) && (min_i==-1 || min_t>times[j].time)) {
                min_t=times[j].time;
                min_i=j;
            }
        }
        times[i].n_stop=min_i;
    }
}

int terminus(int id_times, Time times[]) {
    int count=id_times;
    while (times[count].n_route!=-1)
        count++;
    return count; // à tester
}

void destination(int id_times, Time times[], Stop stops[]) {
    int count=id_times,t_0=times[id_times].time;
    while (times[count].n_route!=-1)
        count++;
    int t=times[count].time-t_0;
    printf("Passage au terminus %s en %dh%02d.\n",stops[times[count].stop].name,(t/3600)%24,(t/60)%60);// à tester
}

int next_departure(int id_stop, int time, Time times[], int nb_times) {
    int min_t=-1,min_i=-1;
    for (int i=0;i<nb_times;++i) {
        if ((times[i].time>=time && times[i].stop==id_stop) && (min_i==-1 || min_t>times[i].time)) {
            min_t=times[i].time;
            min_i=i;
        }
    }
    return min_i; // à tester
}

void next_five_departures(int id_stop, int time, Time times[], int nb_times, Stop stops[]) {
    printf("Prochain départs depuis l'arrêt %s:\n",stops[id_stop].name);
    int x=next_departure(id_stop,time,times,nb_times);
    if (x==-1) {
        printf("Plus de bus pour aujourd'hui\n");
        return;
    }
    int ptr=times[x].n_stop;
    for (int k=5;k>0 && ptr!=-1;k--) {
        printf("Route %d à %dh%02d (%d), vers %d %s.\n",
        times[ptr].route,
        (times[ptr].time/3600)%24,
        (times[ptr].time/60)%60,
        times[ptr].time,
        terminus(ptr,times),
        stops[times[terminus(ptr,times)].stop].name);
        ptr=times[ptr].n_stop;
    } // à tester
}

void all_destinations(int stop, Time times[], int nb_times, Stop stops[], int nb_stops) {
    bool *visited=malloc(sizeof(int)*nb_stops);
    for (int i=0;i<nb_stops;++i) {
        visited[i]=false;
    }
    printf("Prochain destinations depuis l'arrêt %s:\n",stops[stop].name);
    int x=next_departure(stop,0,times,nb_times);
    if (x==-1) {
        printf("Pas de bus sur cet arrêt..!\n");
        return;
    }
    int ptr=times[x].n_stop;
    while (ptr!=-1) {
        int stop_id=times[terminus(ptr,times)].stop;
        if (!visited[stop_id]) {
            visited[stop_id]=true;
            printf("Route vers %s.\n",
            stops[times[terminus(ptr,times)].stop].name);
        }
        ptr=times[ptr].n_stop;
    }
    free(visited); // à tester
}

void reachable(int stop, int time, Time times[], int nb_times, Stop stops[], int nb_stops) {
    bool *visited=malloc(sizeof(int)*nb_stops);
    visited[stop]=true;
    printf("Stations accessibles au départ de %s :\n",stops[stop].name);
    int nxt=next_departure(stop,time,times,nb_times);
    if (nxt!=-1) {
        reachable_aux(nxt,times[nxt].time,times,nb_times,stops,nb_stops,visited);
    }
    if (times[stop].n_route!=-1) {
        reachable_aux(times[stop].n_route,times[times[stop].n_route].time,times,nb_times,stops,nb_stops,visited);
    }
    free(visited);
    // à tester
}

void reachable_aux(int route,int time,Time times[],int nb_times, Stop stops[],int nb_stops,bool* visited) {
    printf("%s\n",stops[times[route].stop].name);
    visited[times[route].stop]=true;
    int nxt=next_departure(times[route].stop,time+1,times,nb_times);
    if (nxt!=-1 && !visited[times[nxt].stop]) {
        reachable_aux(nxt,times[nxt].time,times,nb_times,stops,nb_stops,visited);
    }
    if (times[route].n_route!=-1 && !visited[times[times[route].n_route].stop]) {
        reachable_aux(times[route].n_route,times[times[times[route].stop].n_route].time,times,nb_times,stops,nb_stops,visited);
    }
}

//-----------------------------------
// Dijkstra exploration

int* Dijkstra(int index_orig, int dest_stop, Time times[], int nb_times) {
    int *res=malloc(sizeof(int)*nb_times);
    bool *visited=malloc(sizeof(int)*nb_times);
    for (int i=0;i<nb_times;++i) {
        visited[i]=-1;
        visited[i]=true;
    }
    Heapq h=new_heapq();
    while (!is_empty(h)) {
        // List.iter (fun v-> ... ) (neighbors pop(h))
    }
    return NULL; // à compléter
}

//-----------------------------------
// Heuristic calculation

//-----------------------------------
// A* exploration

int* Astar(int index_orig, int dst_stop, Time times[]) {
    return NULL; // à compléter
}

//-----------------------------------
// Routes

void disp_route(int src_stop, int time, int dst_stop, Time times[], int nb_times, Stop stops[]) {
     int index_orig = next_departure(src_stop, time, times, nb_times);
     if (index_orig == -1) { printf("Plus aucun départ\n"); return; }
     int* path = Dijkstra(index_orig, dst_stop, times, nb_times);
     if (path == NULL) { printf("Aucun chemin trouvé\n"); return; }
     int i = 0;
     while (path[i] != -1) {
         Time time = times[path[i]];
         printf("Arrêt %s à %dh%02d\n", stops[time.stop].name, (time.time/3600)%24, (time.time/60)%60);
         i++;
     }
}


//-----------------------------------
// Main file

int main(void) {
    int nb_stops, nb_times;
    Stop* stops = load_stops("stops.dat", &nb_stops);
    Time* times = load_times("times.dat", &nb_times);
    build_graph(times, nb_times);
    destination(0,times,stops);
    destination(29,times,stops);
    destination(59,times,stops);
    next_five_departures(57,(12*60+37)*60,times,nb_times,stops);
    all_destinations(19,times,nb_times,stops,nb_stops);
    all_destinations(26,times,nb_times,stops,nb_stops);
    reachable(18,(22*60+40)*60,times,nb_times,stops,nb_stops);
    return 0;
}
