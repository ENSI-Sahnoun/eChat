#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

typedef struct Relation {
    int id;                       
    struct Relation *next;        
} Relation;

typedef struct Publication {
    char message[200];
    char date[20];                 
    struct Publication *next;      
} Publication;

typedef struct User {
    int id;
    char name[30];
    char password[30];
    Relation *amis;                
    Relation *abonnements;         
    Publication *publications;     
    struct User *left;
    struct User *right;
} User;

typedef struct {
    int id;
    char name[30];
    char password[30];
} UserSave;

typedef struct TimelinePost {
    int id;
    char name[30];
    char message[200];
    char date[20];
} TimelinePost;

// ============= FONCTIONS UTILITAIRES =============

void clear_screen(void) {
    system("cls");
}

void pause_screen(void) {
    printf("\n");
    printf("\n");
    printf("Appuyez sur Entrée pour continuer...\n");
    printf("\n");
    while (getchar() != '\n');
}

int read_line(char *buffer, size_t size) {
    if (!fgets(buffer, size, stdin)) {
        buffer[0] = '\0';
        return 0;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

int read_int(int *value) {
    char buffer[50];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }
    if (sscanf(buffer, "%d", value) != 1) {
        return 0;
    }
    return 1;
}

void print_header(const char *title) {
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║  %s  ║\n", title);
    printf("╚════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_subheader(const char *title) {
    printf("\n");
    printf("┌────────────────────────────────────────────────┐\n");
    printf("│  %s  │\n", title);
    printf("└────────────────────────────────────────────────┘\n");
}

void show_success(const char *message) {
    printf("\n✅ %s\n", message);
}

void show_error(const char *message) {
    printf("\n❌ %s\n", message);
}

void show_info(const char *message) {
    printf("\nℹ️ %s\n", message);
}

void show_warning(const char *message) {
    printf("\n⚠️ %s\n", message);
}

void prompt(const char *text) {
    printf("▶ %s: ", text);
}

void loading_animation(const char *text, int duration_ms) {
    printf("\n⏳ %s", text);
    fflush(stdout);
    printf("\n");
}

void print_separator(void) {
    printf("────────────────────────────────────────────────\n");
}

void print_double_separator(void) {
    printf("════════════════════════════════════════════════\n");
}

// ============= DÉCLARATIONS AVANT =============

void titlescreen(void);
void credits(void);
int main_menu(void);
int admin_menu(void);
int user_menu(User *u);

User* create_user(int id, const char *name, const char *password);
User* insert_user(User *root, int id, char *name, char *password);
User* add_user(User *root);
User* search_user(User *root, int id);
User* find_min(User *root);
User* delete_user(User *root, int id);
User* login(User *root);
void inorder(User *root);
void show_user_profile(User *root, User *u);

void save_users(User *root);
User* load_users(void);
void save_all_relations(User *root);
void load_relations(User *root);
void save_all_publications(User *root);
void load_publications(User *root);

void save_users_rec(User *root, FILE *file);
void save_relations(User *root, FILE *file);
void save_publications(User *root, FILE *file);

int relation_exists(Relation *head, int id);
void add_relation_front(Relation **head, int id);
void add_friend(User *root, User *u, int friend_id);
void follow(User *root, User *u, int target_id);
void remove_relation(Relation **head, int id);
void remove_friend(User *root, User *u, int friend_id);
void unfollow(User *root, User *u, int target_id);
void manage_relations_user(User *root, User *u);
void show_my_connections(User *root, User *u);

void push_publication(User *u, const char *message, const char *date);
void show_timeline(User *root, User *u);
void show_my_posts(User *u);

int count_relations(Relation *r);
int count_posts(Publication *p);
int count_followers(User *root, int target_id);
void compute_statistics(User *root, int *total_users,
                        User **most_active, int *max_posts,
                        User **most_followed, int *max_followers,
                        User **most_friends, int *max_friends);
void show_statistics(User *root);

// ============= Interface =============

void titlescreen() {
    printf("\n");
    print_double_separator();
    printf("         ,-----.,--.               ,--.   \n"
           "  ,---. '  .--./|  ,---.  ,--,--.,-'  '-. \n"
           " | .-. :|  |    |  .-.  |' ,-.  |'-.  .-' \n"
           " \\   --.'  '--'\\|  | |  |\\ '-'  |  |  |   \n"
           "  `----' `-----'`--' `--' `--`--'  `--'   \n");
    print_double_separator();
}

void credits() {
    printf("    Realise par:\n");
    printf("       - Youssef Sahnoun\n");
    printf("       - Ameur Masmoudi\n");
    printf("       - Mohamed Amine Walha\n");
    print_separator();
}

int main_menu() {
    clear_screen();
    print_header("MENU PRINCIPAL");
    
    printf("  1. Créer un compte\n");
    printf("  2. Connexion Admin\n");
    printf("  3. Connexion Utilisateur\n");
    printf("  4. Quitter\n");
    printf("\n");
    print_separator();
    
    int choice;
    prompt("Votre choix");
    if (!read_int(&choice)) {
        show_error("Choix invalide!");
        pause_screen();
        return -1;
    }
    return choice;
}

int admin_menu() {
    clear_screen();
    print_header("PANNEAU ADMINISTRATEUR");
    
    printf("  1. Créer un utilisateur\n");
    printf("  2. Supprimer un utilisateur\n");
    printf("  3. Rechercher un utilisateur\n");
    printf("  4. Liste complète des utilisateurs\n");
    printf("  5. Statistiques du réseau\n");
    printf("  6. Déconnexion\n");
    printf("\n");
    print_separator();
    
    int choice;
    prompt("Votre choix");
    if (!read_int(&choice)) {
        show_error("Choix invalide!");
        pause_screen();
        return -1;
    }
    return choice;
}

int user_menu(User *u) {
    clear_screen();
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║  👤 %s  ║\n", u->name);
    printf("╚════════════════════════════════════════════════╝\n");
    printf("\n");
    
    printf("  1.  Publier un message\n");
    printf("  2.  Gérer mes relations\n");
    printf("  3.  Voir mon fil d'actualité\n");
    printf("  4.  Mes publications\n");
    printf("  5.  Mon profil\n");
    printf("  6.  Statistiques du réseau\n");
    printf("  7.  Déconnexion\n");
    printf("\n");
    print_separator();
    
    int choice;
    prompt("Votre choix");
    if (!read_int(&choice)) {
        show_error("Choix invalide!");
        pause_screen();
        return -1;
    }
    return choice;
}

// ============= Utilisateurs =============

User *create_user(int id, const char *name, const char *password) {
    User *u = (User*)malloc(sizeof(User));
    if (!u) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    u->id = id;
    strncpy(u->name, name, sizeof(u->name)-1);
    u->name[sizeof(u->name)-1] = '\0';
    strncpy(u->password, password, sizeof(u->password)-1);
    u->password[sizeof(u->password)-1] = '\0';
    u->amis = NULL;
    u->abonnements = NULL;
    u->publications = NULL;
    u->left = u->right = NULL;
    return u;
}

User* insert_user(User *root, int id, char *name, char *password) {
    if (root == NULL)
        return create_user(id, name, password);

    if (id < root->id)
        root->left = insert_user(root->left, id, name, password);
    else if (id > root->id)
        root->right = insert_user(root->right, id, name, password);
    else {
        show_warning("Un utilisateur avec cet ID existe deja! ⚠️");
        return root;
    }

    return root;
}

User* add_user(User *root) {
    int id;
    char name[30], password[30];
    
    print_subheader("Création d'un nouveau compte");
    
    prompt("ID utilisateur");
    if (!read_int(&id)) {
        show_error("ID invalide - Utilisez uniquement des chiffres!");
        pause_screen();
        return root;
    }
    
    if (search_user(root, id)) {
        show_error("Cet ID est déjà utilisé!");
        pause_screen();
        return root;
    }
    
    prompt("Nom d'utilisateur");
    if (!read_line(name, sizeof(name)) || strlen(name) == 0) {
        show_error("Nom invalide!");
        pause_screen();
        return root;
    }
    
    prompt("Mot de passe");
    if (!read_line(password, sizeof(password)) || strlen(password) < 3) {
        show_error("Mot de passe trop court (minimum 3 caractères)!");
        pause_screen();
        return root;
    }
    
    root = insert_user(root, id, name, password);
    show_success("Compte créé avec succès!");
    printf("\n   ID: %d\n   Nom: %s\n", id, name);
    pause_screen();
    return root;
}

User* search_user(User *root, int id) {
    if (root == NULL || root->id == id)
        return root;

    if (id < root->id)
        return search_user(root->left, id);
    else
        return search_user(root->right, id);
}

User* find_min(User *root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

User* delete_user(User *root, int id) {
    if (root == NULL) {
        return NULL;
    }

    if (id < root->id)
        root->left = delete_user(root->left, id);
    else if (id > root->id)
        root->right = delete_user(root->right, id);
    else {
        printf("\n⚠️ Suppression de: %s (ID: %d)\n", root->name, root->id);
        
        if (root->left == NULL && root->right == NULL) {
            Relation *r, *rn;
            for (r = root->amis; r; r = rn) { rn = r->next; free(r); }
            for (r = root->abonnements; r; r = rn) { rn = r->next; free(r); }
            Publication *p, *pn;
            for (p = root->publications; p; p = pn) { pn = p->next; free(p); }
            free(root);
            return NULL;
        }
        if (root->left == NULL) {
            User *tmp = root->right;
            Relation *r, *rn;
            for (r = root->amis; r; r = rn) { rn = r->next; free(r); }
            for (r = root->abonnements; r; r = rn) { rn = r->next; free(r); }
            Publication *p, *pn;
            for (p = root->publications; p; p = pn) { pn = p->next; free(p); }
            free(root);
            return tmp;
        }
        if (root->right == NULL) {
            User *tmp = root->left;
            Relation *r, *rn;
            for (r = root->amis; r; r = rn) { rn = r->next; free(r); }
            for (r = root->abonnements; r; r = rn) { rn = r->next; free(r); }
            Publication *p, *pn;
            for (p = root->publications; p; p = pn) { pn = p->next; free(p); }
            free(root);
            return tmp;
        }

        User *succ = find_min(root->right);
        root->id = succ->id;
        strcpy(root->name, succ->name);
        strcpy(root->password, succ->password);
        root->right = delete_user(root->right, succ->id);
    }

    return root;
}

User* login(User *root) {
    int id;
    char password[30];
    
    print_subheader("Connexion");
    
    prompt("ID utilisateur");
    if (!read_int(&id)) {
        show_error("ID invalide");
        pause_screen();
        return NULL;
    }
    
    prompt("Mot de passe");
    if (!read_line(password, sizeof(password))) {
        show_error("Mot de passe invalide!");
        pause_screen();
        return NULL;
    }
    
    User *u = search_user(root, id);
    if (!u) {
        show_error("Utilisateur introuvable!");
        pause_screen();
        return NULL;
    }
    if (strcmp(u->password, password) != 0) {
        show_error("Mot de passe incorrect!");
        pause_screen();
        return NULL;
    }
    
    loading_animation("Connexion en cours", 500);
    show_success("Connexion réussie!");
    printf("\n🎉 Bon retour, %s!\n", u->name);
    pause_screen();
    return u;
}

void inorder(User *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("  - ID: %d | %s\n", root->id, root->name);
    inorder(root->right);
}

void show_user_profile(User *root, User *u) {
    if (!u) return;
    
    clear_screen();
    print_header("MON PROFIL");
    
    printf("👤 Nom d'utilisateur: %s\n", u->name);
    printf("🆔 ID: %d\n\n", u->id);
    
    int friends = count_relations(u->amis);
    int following = count_relations(u->abonnements);
    int followers = count_followers(root, u->id);
    int posts = count_posts(u->publications);
    
    print_subheader("Statistiques");
    printf("👥 Amis: %d\n", friends);
    printf("➕ Abonnements: %d\n", following);
    printf("⭐ Abonnés: %d\n", followers);
    printf("📝 Publications: %d\n", posts);
    
    pause_screen();
}

// ============= Fichiers =============

void save_users_rec(User *root, FILE *file) {
    if (!root) return;

    save_users_rec(root->left, file);
    save_users_rec(root->right, file);
    UserSave tmp;
    tmp.id = root->id;
    strncpy(tmp.name, root->name, sizeof(tmp.name)-1);
    tmp.name[sizeof(tmp.name)-1] = '\0';
    strncpy(tmp.password, root->password, sizeof(tmp.password)-1);
    tmp.password[sizeof(tmp.password)-1] = '\0';
    fwrite(&tmp, sizeof(UserSave), 1, file);
}

void save_users(User *root) {
    FILE *file = fopen("utilisateurs.bin", "wb");
    if (!file) {
        show_error("Impossible de sauvegarder les utilisateurs!");
        return;
    }
    save_users_rec(root, file);
    fclose(file);
}

User* load_users() {
    FILE *file = fopen("utilisateurs.bin", "rb");
    if (!file) return NULL;
    
    User *root = NULL;
    UserSave tmp;

    while (fread(&tmp, sizeof(UserSave), 1, file) == 1) {
        root = insert_user(root, tmp.id, tmp.name, tmp.password);
    }

    fclose(file);
    return root;
}

void save_relations(User *root, FILE *file) {
    if (!root) return;

    save_relations(root->left, file);
    save_relations(root->right, file);
    fprintf(file, "ID: %d\n", root->id);

    fprintf(file, "Amis:");
    Relation *f = root->amis;
    while (f) {
        fprintf(file, " %d", f->id);
        f = f->next;
    }
    fprintf(file, "\n");

    fprintf(file, "Abonnements:");
    Relation *fl = root->abonnements;
    while (fl) {
        fprintf(file, " %d", fl->id);
        fl = fl->next;
    }
    fprintf(file, "\n\n");
}

void save_all_relations(User *root) {
    FILE *file = fopen("relations.txt", "w");
    if (!file) {
        show_error("Impossible de sauvegarder les relations!");
        return;
    }
    save_relations(root, file);
    fclose(file);
}

void load_relations(User *root) {
    FILE *file = fopen("relations.txt", "r");
    if (!file) return;
    
    char line[512];
    int current_id = -1;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "ID:", 3) == 0) {
            sscanf(line, "ID: %d", &current_id);
        }
        else if (strncmp(line, "Amis:", 5) == 0) {
            User *u = search_user(root, current_id);
            if (!u) continue;

            char *p = line + 5;
            char *token = strtok(p, " \t\n");
            while (token) {
                int fid;
                if (sscanf(token, "%d", &fid) == 1) {
                    if (!relation_exists(u->amis, fid))
                        add_relation_front(&u->amis, fid);
                }
                token = strtok(NULL, " \t\n");
            }
        }
        else if (strncmp(line, "Abonnements:", 12) == 0) {
            User *u = search_user(root, current_id);
            if (!u) continue;

            char *p = line + 12;
            char *token = strtok(p, " \t\n");
            while (token) {
                int fid;
                if (sscanf(token, "%d", &fid) == 1) {
                    if (!relation_exists(u->abonnements, fid))
                        add_relation_front(&u->abonnements, fid);
                }
                token = strtok(NULL, " \t\n");
            }
        }
    }

    fclose(file);
}

void save_publications(User *root, FILE *file) {
    if (!root) return;

    save_publications(root->left, file);
    save_publications(root->right, file);

    Publication *p = root->publications;
    while (p) {
        fprintf(file,"%d;%s;%s\n", root->id, p->date, p->message);
        p = p->next;
    }
}

void save_all_publications(User *root) {
    FILE *file = fopen("publications.txt", "w");
    if (!file) {
        show_error("Impossible de sauvegarder les publications!");
        return;
    }
    save_publications(root, file);
    fclose(file);
}

void load_publications(User *root) {
    FILE *file = fopen("publications.txt", "r");
    if (!file) return;
    
    int id;
    char date[20];
    char message[200];

    while (fscanf(file, "%d;%19[^;];%199[^\n]", &id, date, message) == 3) {
        User *u = search_user(root, id);
        if (u) push_publication(u, message, date);
        fscanf(file, "\n");
    }

    fclose(file);
}

// ============= RELATIONS =============

int relation_exists(Relation *head, int id) {
    Relation *r = head;
    while (r) {
        if (r->id == id) return 1;
        r = r->next;
    }
    return 0;
}

void add_relation_front(Relation **head, int id) {
    if (relation_exists(*head, id)) return;
    Relation *n = (Relation*)malloc(sizeof(Relation));
    n->id = id;
    n->next = *head;
    *head = n;
}

void add_friend(User *root, User *u, int friend_id) {
    if (!u) return;
    if (u->id == friend_id) {
        show_warning("Vous ne pouvez pas etre ami avec vous-meme!");
        return;
    }
    User *f = search_user(root, friend_id);
    if (!f) {
        show_error("Utilisateur introuvable!");
        return;
    }
    if (relation_exists(u->amis, friend_id)) {
        show_info("Vous etes deja amis avec cet utilisateur!");
        printf("👤 %s\n", f->name);
        return;
    }
    add_relation_front(&u->amis, friend_id);
    add_relation_front(&f->amis, u->id);
    show_success("Nouveau ami ajoute! 🤝");
    printf("\n%s et %s sont maintenant amis! ✅\n", u->name, f->name);
}

void follow(User *root, User *u, int target_id) {
    if (!u) return;
    if (u->id == target_id) {
        show_warning("Vous ne pouvez pas vous abonner a vous-meme!");
        return;
    }
    User *t = search_user(root, target_id);
    if (!t) {
        show_error("Utilisateur introuvable!");
        return;
    }
    if (relation_exists(u->abonnements, target_id)) {
        show_info("Vous suivez deja cet utilisateur!");
        printf("👤 %s\n", t->name);
        return;
    }
    add_relation_front(&u->abonnements, target_id);
    show_success("Nouvel abonnement!");
    printf("\nVous suivez maintenant %s! ✅\n", t->name);
}

void remove_relation(Relation **head, int id) {
    Relation *prev = NULL;
    Relation *curr = *head;
    while (curr) {
        if (curr->id == id) {
            if (prev) prev->next = curr->next;
            else *head = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void remove_friend(User *root, User *u, int friend_id) {
    if (!u) return;
    User *f = search_user(root, friend_id);
    if (!f) {
        show_error("Utilisateur introuvable!");
        return;
    }
    if (!relation_exists(u->amis, friend_id)) {
        show_warning("Vous n'etes pas amis!");
        return;
    }
    remove_relation(&u->amis, friend_id);
    remove_relation(&f->amis, u->id);
    show_success("Ami retire!");
    printf("\n%s et %s ne sont plus amis!\n", u->name, f->name);
}

void unfollow(User *root, User *u, int target_id) {
    if (!u) return;
    User *t = search_user(root, target_id);
    if (!t) {
        show_error("Utilisateur introuvable!");
        return;
    }
    if (!relation_exists(u->abonnements, target_id)) {
        show_warning("Vous ne suivez pas cet utilisateur!");
        return;
    }
    remove_relation(&u->abonnements, target_id);
    show_success("Désabonnement effectué!");
    printf("\nVous ne suivez plus %s! ✅\n", t->name);
}

void show_my_connections(User *root, User *u) {
    if (!u) return;
    
    clear_screen();
    print_header("MES CONNEXIONS");

    print_subheader("👥 Mes Amis");
    Relation *r = u->amis;
    int count = 0;
    if (!r) {
        printf("Aucun ami pour le moment! ⚠️\n");
    } else {
        while (r) {
            User *friend = search_user(root, r->id);
            if (friend) {
                printf("   - [%d] %s\n", friend->id, friend->name);
                count++;
            }
            r = r->next;
        }
        printf("\nTotal: %d ami(s)\n", count);
    }
    
    printf("\n");
    
    print_subheader("➕ Mes Abonnements");
    r = u->abonnements;
    count = 0;
    if (!r) {
        printf("Aucun abonnement! ⚠️\n");
    } else {
        while (r) {
            User *followed = search_user(root, r->id);
            if (followed) {
                printf("   - [%d] %s\n", followed->id, followed->name);
                count++;
            }
            r = r->next;
        }
        printf("\nTotal: %d abonnement(s)\n", count);
    }
    
    printf("\n");
    
    // Followers
    print_subheader("⭐ Mes Abonnes");
    int followers = count_followers(root, u->id);
    printf("%d personne(s) vous suit/suivent\n", followers);
    
    pause_screen();
}

void manage_relations_user(User *root, User *u) {
    clear_screen();
    print_header("GESTION DES RELATIONS");
    
    printf("  1.  Ajouter un ami\n");
    printf("  2.  S'abonner à un utilisateur\n");
    printf("  3.  Retirer un ami\n");
    printf("  4.  Se désabonner\n");
    printf("  5.  Voir mes connexions\n");
    print_separator();
    
    int op;
    prompt("Votre choix");
    if (!read_int(&op)) {
        show_error("Choix invalide!");
        pause_screen();
        return;
    }
    
    if (op == 5) {
        show_my_connections(root, u);
        return;
    }
    
    int id2;
    prompt("ID de l'utilisateur");
    if (!read_int(&id2)) {
        show_error("ID invalide!");
        pause_screen();
        return;
    }
    
    User *c = search_user(root, id2);
    if (!c) {
        show_error("Utilisateur introuvable!");
        pause_screen();
        return;
    }
    
    switch (op) {
        case 1:
            add_friend(root, u, id2);
            break;
        case 2:
            follow(root, u, id2);
            break;
        case 3:
            remove_friend(root, u, id2);
            break;
        case 4:
            unfollow(root, u, id2);
            break;
        default:
            show_error("Choix invalide!");
            break;
    }
    pause_screen();
}

// ============= PUBLICATIONS =============

void push_publication(User *u, const char *message, const char *date) {
    if (!u) return;
    Publication *p = (Publication*)malloc(sizeof(Publication));
    strncpy(p->message, message, sizeof(p->message)-1);
    p->message[sizeof(p->message)-1] = '\0';
    strncpy(p->date, date, sizeof(p->date)-1);
    p->date[sizeof(p->date)-1] = '\0';
    p->next = u->publications;
    u->publications = p;
}

void show_my_posts(User *u) {
    if (!u) return;
    
    clear_screen();
    print_header("MES PUBLICATIONS");
    
    Publication *p = u->publications;
    int count = 0;
    
    if (!p) {
        show_info("Vous n'avez pas encore publié de message!");
        pause_screen();
        return;
    }
    
    while (p) {
        count++;
        printf("┌─────────────────────────────────────────────────┐\n");
        printf("│ 📅 %s\n", p->date);
        printf("├─────────────────────────────────────────────────┤\n");
        printf("│ %s\n", p->message);
        printf("└─────────────────────────────────────────────────┘\n\n");
        p = p->next;
    }
    
    printf("📊 Total: %d publication(s)\n", count);
    pause_screen();
}

void show_timeline(User *root, User *u) {
    if (!u) {
        show_error("Utilisateur introuvable!");
        return;
    }
    
    int count = 0;
    Relation *rf;

    for (rf = u->amis; rf; rf = rf->next) {
        User *friend = search_user(root, rf->id);
        if (!friend) continue;
        Publication *p;
        for (p = friend->publications; p; p = p->next)
            count++;
    }

    if (count == 0) {
        clear_screen();
        print_header("FIL D'ACTUALITE");
        show_info("Aucune publication de vos amis!");
        printf("\n💡 Astuce: Ajoutez des amis pour voir leurs publications!\n");
        pause_screen();
        return;
    }

    TimelinePost *posts = malloc(count * sizeof(TimelinePost));
    int index = 0;

    for (rf = u->amis; rf; rf = rf->next) {
        User *friend = search_user(root, rf->id);
        if (!friend) continue;

        Publication *p;
        for (p = friend->publications; p; p = p->next) {
            posts[index].id = friend->id;
            strcpy(posts[index].name, friend->name);
            strcpy(posts[index].message, p->message);
            strcpy(posts[index].date, p->date);
            index++;
        }
    }


    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(posts[j].date, posts[i].date) > 0) {
                TimelinePost tmp = posts[i];
                posts[i] = posts[j];
                posts[j] = tmp;
            }
        }
    }

    clear_screen();
    print_header("🏠 FIL D'ACTUALITE");
    printf("📰 Affichage de %d publication(s)\n\n", count);
    
    for (int i = 0; i < count; i++) {
        printf("╔═════════════════════════════════════════════════╗\n");
        printf("║ 👤 %s ║\n", posts[i].name);
        printf("║ 🆔 ID: %d ║\n", posts[i].id);   
        printf("║ 📅 %s                          ║\n", posts[i].date);
        printf("╠═════════════════════════════════════════════════╣\n");
        printf("║                                                 ║\n");
        printf("║ %s\n", posts[i].message);
        printf("║                                                 ║\n");
        printf("╚═════════════════════════════════════════════════╝\n\n");
    }
    
    free(posts);
    pause_screen();
}

// ============= STATISTICS =============

int count_relations(Relation *r) {
    int c = 0;
    while (r) { c++; r = r->next; }
    return c;
}

int count_posts(Publication *p) {
    int c = 0;
    while (p) { c++; p = p->next; }
    return c;
}

int count_followers(User *root, int target_id) {
    if (!root) return 0;

    int c = 0;
    Relation *r = root->abonnements;
    while (r) {
        if (r->id == target_id) c++;
        r = r->next;
    }

    return c + count_followers(root->left, target_id) + count_followers(root->right, target_id);
}

void compute_statistics(User *root, int *total_users,
                        User **most_active, int *max_posts,
                        User **most_followed, int *max_followers,
                        User **most_friends, int *max_friends)
{
    if (!root) return;

    compute_statistics(root->left, total_users,
                       most_active, max_posts,
                       most_followed, max_followers,
                       most_friends, max_friends);

    (*total_users)++;

    int posts = count_posts(root->publications);
    if (posts > *max_posts) {
        *max_posts = posts;
        *most_active = root;
    }

    int friends = count_relations(root->amis);
    if (friends > *max_friends) {
        *max_friends = friends;
        *most_friends = root;
    }

    int followers = count_followers(root, root->id);
    if (followers > *max_followers) {
        *max_followers = followers;
        *most_followed = root;
    }

    compute_statistics(root->right, total_users,
                       most_active, max_posts,
                       most_followed, max_followers,
                       most_friends, max_friends);
}

void show_statistics(User *root) {
    if (!root) {
        show_error("Aucune statistique disponible!");
        pause_screen();
        return;
    }

    int total_users = 0;
    User *most_active = NULL;
    User *most_followed = NULL;
    User *most_friends_user = NULL;
    int max_posts = -1;
    int max_followers = -1;
    int max_friends = -1;

    loading_animation("Calcul des statistiques", 300);
    
    compute_statistics(root, &total_users,
                       &most_active, &max_posts,
                       &most_followed, &max_followers,
                       &most_friends_user, &max_friends);

    clear_screen();
    print_header("📊 STATISTIQUES DU RESEAU");
    
    printf("╔═════════════════════════════════════════════════╗\n");
    printf("║  APERÇU GÉNÉRAL                                 ║\n");
    printf("╚═════════════════════════════════════════════════╝\n\n");
    printf("  Utilisateurs inscrits: %d\n\n", total_users);
    
    printf("\n");
    
    printf("╔═════════════════════════════════════════════════╗\n");
    printf("║  🏆 CLASSEMENTS                                 ║\n");
    printf("╚═════════════════════════════════════════════════╝\n\n");

    if (most_active) {
            printf("  🔥 Utilisateur le plus actif:\n");
        printf("     • %s\n", most_active->name);
        printf("     • %d publication(s)\n\n", max_posts);
    }

    if (most_followed) {
        printf("  ⭐ Utilisateur le plus suivi:\n");
        printf("     • %s\n", most_followed->name);
        printf("     • %d abonné(s)\n\n", max_followers);
    }

    if (most_friends_user) {
        printf("  👥 Utilisateur le plus sociable:\n");
        printf("     • %s\n", most_friends_user->name);
        printf("     • %d ami(s)\n\n", max_friends);
    }

    pause_screen();
}

// ============= MAIN =============

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    User *root = NULL;
    User *current_user = NULL;
    int is_admin = 0;
    int choice;
    clear_screen();
    titlescreen();
    credits();
    printf("\n");

    loading_animation("Chargement des utilisateurs", 300);
    root = load_users();

    loading_animation("Chargement des relations", 300);
    load_relations(root);

    loading_animation("Chargement des publications", 300);
    load_publications(root);

    show_success("Système prêt! 🚀");
    pause_screen();

    while (1) {
        if (!current_user && !is_admin) {
            choice = main_menu();

            switch (choice) {
                case 1: {
                    clear_screen();
                    print_header("CRÉER UN COMPTE");
                    root = add_user(root);
                    break;
                }

                case 2: {
                    clear_screen();
                    print_header("CONNEXION ADMINISTRATEUR");
                    char admin_pass[30];
                    prompt("Mot de passe admin");
                    if (!read_line(admin_pass, sizeof(admin_pass))) {
                        show_error("Mot de passe invalide!");
                        pause_screen();
                        break;
                    }
                    if (strcmp(admin_pass, "admin") == 0) {
                        loading_animation("Vérification", 400);
                        is_admin = 1;
                        show_success("Accès administrateur accordé!");
                        pause_screen();
                    } else {
                        show_error("Mot de passe administrateur incorrect!");
                        pause_screen();
                    }
                    break;
                }

                case 3: {
                    clear_screen();
                    print_header("CONNEXION UTILISATEUR");
                    current_user = login(root);
                    break;
                }

                case 4: {
                    clear_screen();
                    print_header("FERMETURE DU SYSTÈME");
                    loading_animation("Sauvegarde des utilisateurs", 300);
                    save_users(root);
                    loading_animation("Sauvegarde des relations", 300);
                    save_all_relations(root);
                    loading_animation("Sauvegarde des publications", 300);
                    save_all_publications(root);
                    show_success("Toutes les données ont été sauvegardées!");
                    printf("\n");
                    printf("╔════════════════════════════════════════════════╗\n");
                    printf("║         Merci d'avoir utilisé eChat! 👋        ║\n");
                    printf("║              À bientôt! 🎉                     ║\n");
                    printf("╚════════════════════════════════════════════════╝\n");
                    printf("\n");
                    pause_screen();
                    exit(0);
                }

                default:
                    show_error("Choix invalide (1-4)");
                    pause_screen();
                    break;
            }
        } 
        else if (is_admin) {
            choice = admin_menu();

            switch (choice) {
                case 1: {
                    clear_screen();
                    print_header("CREER UTILISATEUR");
                    root = add_user(root);
                    break;
                }

                case 2: {
                    clear_screen();
                    print_header("SUPPRIMER UTILISATEUR");
                    int id;
                    prompt("ID a supprimer");
                    if (!read_int(&id)) {
                        show_error("ID invalide!");
                        pause_screen();
                        break;
                    }
                    User *target = search_user(root, id);
                    if (!target) {
                        show_error("Utilisateur introuvable!");
                        pause_screen();
                        break;
                    }
                    root = delete_user(root, id);
                    show_success("Utilisateur supprimé avec succès!");
                    pause_screen();
                    break;
                }

                case 3: {
                    clear_screen();
                    print_header("RECHERCHER UTILISATEUR");
                    int id;
                    prompt("ID a rechercher");
                    if (!read_int(&id)) {
                        show_error("ID invalide!");
                        pause_screen();
                        break;
                    }
                    loading_animation("Recherche", 200);
                    User *u = search_user(root, id);
                    if (u) {
                        show_success("Utilisateur trouvé!");
                        printf("\n");
                        print_subheader("Informations");
                        printf("  🆔 ID: %d\n", u->id);
                        printf("  👤 Nom: %s\n", u->name);
                        printf("  👥 Amis: %d\n", count_relations(u->amis));
                        printf("  ➕ Abonnements: %d\n", count_relations(u->abonnements));
                        printf("  📝 Publications: %d\n", count_posts(u->publications));
                    } else {
                        show_error("Utilisateur introuvable!");
                        break;
                    }
                    pause_screen();
                    break;
                }

                case 4: {
                    clear_screen();
                    print_header("LISTE DES UTILISATEURS");
                    if (root == NULL) {
                        show_info("Aucun utilisateur inscrit!");
                    } else {
                        printf("\n");
                        inorder(root);
                    }
                    pause_screen();
                    break;
                }

                case 5: {
                    show_statistics(root);
                    break;
                }

                case 6: {
                    is_admin = 0;
                    show_success("Déconnexion administrateur réussie!");
                    pause_screen();
                    break;
                }

                default:
                    show_error("Choix invalide (1-6)");
                    pause_screen();
                    break;
            }
        } 
        else {
            choice = user_menu(current_user);

            switch (choice) {
                case 1: {
                    clear_screen();
                    print_header("NOUVELLE PUBLICATION");
                    char msg[200];
                    char date[20];

                    printf("Partagez vos pensées avec vos amis...\n\n");
                    prompt("Votre message");
                    if (!read_line(msg, sizeof(msg)) || strlen(msg) == 0) {
                        show_error("Message vide!");
                        pause_screen();
                        break;
                    }

                    time_t now = time(NULL);
                    struct tm *tm_info = localtime(&now);
                    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);
                    
                    push_publication(current_user, msg, date);
                    loading_animation("Publication en cours", 300);
                    show_success("Message publié avec succès!");
                    pause_screen();
                    break;
                }

                case 2: {
                    manage_relations_user(root, current_user);
                    break;
                }

                case 3: {
                    show_timeline(root, current_user);
                    break;
                }

                case 4: {
                    show_my_posts(current_user);
                    break;
                }

                case 5: {
                    show_user_profile(root, current_user);
                    break;
                }

                case 6: {
                    show_statistics(root);
                    break;
                }

                case 7: {
                    loading_animation("Déconnexion", 300);
                    current_user = NULL;
                    show_success("À bientôt! 👋");
                    pause_screen();
                    break;
                }

                default:
                    show_error("Choix invalide (1-7)");
                    pause_screen();
                    break;
            }
        }
    }
    return 0;
}