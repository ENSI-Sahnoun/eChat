# 🌐 SNS eChat - Système de Réseau Social

<p align="center">
  <img src="https://img.shields.io/badge/Langage-C-blue?style=for-the-badge&logo=c" alt="Langage C">
  <img src="https://img.shields.io/badge/Plateforme-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge" alt="Plateforme">
</p>

Une application console de **Réseau Social** développée en C. Cette application gère les utilisateurs (avec mot de passe), les amitiés, les abonnements et les publications. Conçue comme projet pédagogique pour le cours de Programmation C à l'ENSI, elle propose une persistance simple des données et des fonctions d'administration et d'utilisateur.

---

## 📋 Table des Matières

- [Fonctionnalités](#-fonctionnalités)
- [Structures de Données](#-structures-de-données)
- [Structure du Projet](#-structure-du-projet)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Options du Menu](#-options-du-menu)
- [Formats de Fichiers](#-formats-de-fichiers)
- [Détails Techniques](#-détails-techniques)
- [Améliorations Futures](#-améliorations-futures)
- [Auteurs](#-auteurs)
- [Licence](#-licence)

---

## ✨ Fonctionnalités

| Fonctionnalité | Description |
|----------------|-------------|
| 👤 **Gestion des Utilisateurs** | Ajouter, rechercher et supprimer des profils avec IDs uniques et mot de passe |
| 🔐 **Authentification** | Login utilisateur par ID + mot de passe ; login admin via mot de passe `admin` |
| 🤝 **Amitiés** | Créer des amitiés bidirectionnelles entre utilisateurs |
| 📢 **Système d'Abonnement** | S'abonner à d'autres utilisateurs (unidirectionnel) |
| 📝 **Publications** | Publier des messages horodatés automatiquement |
| 📰 **Fil d'Actualité (Timeline)** | Voir les publications des amis triées par date (récentes d'abord) |
| 💾 **Persistance des Données** | Sauvegarde/chargement des utilisateurs, relations et publications au démarrage/quitte |
| 📊 **Statistiques** | Comptage d'utilisateurs, utilisateur le plus actif, le plus suivi et le plus d'amis |

---

## 🏗️ Structures de Données

L'application utilise plusieurs structures de données optimisées pour différentes opérations.

### Arbre Binaire de Recherche (ABR) - Utilisateurs

Les utilisateurs sont stockés dans un Arbre Binaire de Recherche pour des opérations efficaces en moyenne O(log n) pour la recherche, l'insertion et la suppression.

Exemple visuel :
```
        [User 50]
        /       \
   [User 25]  [User 75]
    /    \      /    \
 [10]   [30]  [60]  [90]
```

### Listes Chaînées - Relations & Publications

Chaque utilisateur maintient trois listes chaînées :
- Amis (`amis`) : Amitiés bidirectionnelles
- Abonnements (`abonnements`) : Utilisateurs suivis (unidirectionnel)
- Publications (`publications`) : Messages publiés

Exemple :
```
User -> Amis: [ID:5] -> [ID:12] -> NULL
     -> Abonnements: [ID:69] -> NULL
     -> Publications: [Post1] -> [Post2] -> NULL
```

### Définitions importantes (résumé)

- User : id, name, password, listes relations/publications, pointeurs left/right pour ABR.
- Relation : id (de l'autre utilisateur), pointeur next.
- Publication : message, date, pointeur next.

---

## 📁 Structure du Projet

```
Projet-C/
├── eChat.c  # Code source principal (login + toutes les fonctionnalités)
└── README.md              # Ce fichier

# Fichiers générés à l'exécution :
# ├── utilisateurs.bin     # Fichier binaire des utilisateurs (id, name, password)
# ├── relations.txt        # Fichier texte des amitiés et abonnements
# └── publications.txt     # Stockage des publications
```

---

## 🚀 Installation

### Prérequis

- Compilateur GCC (ou tout compilateur C compatible)
- Terminal / Invite de commandes

### Compilation sur Linux/macOS

```bash
# Cloner le dépôt
git clone https://github.com/ENSI-Sahnoun/Projet-C.git
cd Projet-C

# Compiler
gcc -o sns_echat eChat.c

# Exécuter
./sns_echat
```

### Compilation sur Windows (MinGW)

```cmd
gcc -o sns_echat.exe eChat.c
sns_echat.exe
```

### Utilisation d'un IDE

1. Ouvrir votre IDE préféré (Code::Blocks, Visual Studio, etc.)
2. Créer un projet C
3. Ajouter `eChat.c` au projet
4. Compiler et exécuter

---

## 💻 Utilisation

### Lancement de l'Application

À l'exécution, des messages indiquent le chargement des données :

- Chargement des utilisateurs...
- Chargement des relations...
- Chargement des publications...
- Chargement terminé.

L'interface propose un menu principal (cf. ci-dessous).

---

## 📖 Options du Menu

Menu principal :
- 1 : Créer utilisateur (demande d'ID, nom et mot de passe)
- 2 : Login Admin (mot de passe admin par défaut : "admin")
- 3 : Login Utilisateur (ID + mot de passe)
- 4 : Quitter (sauvegarde automatique avant fermeture)

Menu Admin :
- 1 : Créer utilisateur
- 2 : Supprimer utilisateur (par ID)
- 3 : Rechercher utilisateur (par ID)
- 4 : Afficher tous les utilisateurs (par parcours infixe)
- 5 : Afficher statistiques
- 6 : Déconnexion admin

Menu Utilisateur (après login) :
- 1 : Publier un message (horodatage automatique)
- 2 : Gérer les relations (ajouter/supprimer ami, suivre/arrêter de suivre)
- 3 : Afficher timeline (publications des amis triées par date décroissante)
- 4 : Afficher statistiques
- 5 : Déconnexion

Gestion des relations (option interne) :
```
1. Ajouter ami
2. Suivre utilisateur
3. Supprimer ami
4. Arrêter de suivre
```

---

## 📄 Formats de Fichiers

### utilisateurs.bin (Binaire)
Stocke les utilisateurs en binaire pour une lecture/écriture efficace. Chaque entrée contient :
- int id;
- char name[30];
- char password[30];

Le code lit/écrit cette structure via des enregistrements fixes.

### relations.txt (Texte)
Fichier lisible contenant les relations par utilisateur :
```
ID: 5
Amis: 12 88 123
Abonnements: 69 56

ID: 12
Amis: 5
Abonnements:
```

### publications.txt (Texte)
Une ligne par publication avec champ séparé par ';' :
```
[UserID];[Date];Message
[5];2024-01-15 14:30:00;Bonjour le monde !
```

---

## 🔧 Détails Techniques

### Algorithmes Utilisés

| Opération | Algorithme | Complexité |
|-----------|------------|------------|
| Recherche utilisateur | Recherche ABR | O(log n) |
| Insertion utilisateur | Insertion ABR | O(log n) |
| Suppression utilisateur | Suppression ABR | O(log n) |
| Vérification ami | Recherche linéaire | O(k) |
| Tri du fil | Tri à bulles | O(p²) |

*Où n = nombre d'utilisateurs, k = nombre d'amis, p = nombre de publications*

### Gestion de la Mémoire

- Toute la mémoire dynamique est allouée avec `malloc()`
- La mémoire est libérée lors de la suppression des utilisateurs (free)
- Les listes chaînées sont parcourues et libérées à la suppression

---

## 🔮 Améliorations Futures

- ☑️ Protection par mot de passe (implémenté)
- ☑️ Implémentation des statistiques (utilisateurs actifs, nombre d'amis)
- ☑️ Authentification admin et utilisateur
- ☑️ Affichage des listes d'amis et d'abonnés
- [ ] Édition et suppression des publications
- [ ] Recherche avancée (par nom)
- [ ] Réactions/likes sur les messages

---

## 👥 Auteurs

<table>
  <tr>
    <td align="center">
      <b>Youssef Sahnoun</b><br>
      <sub>Étudiant ENSI II1A</sub>
    </td>
    <td align="center">
      <b>Ameur Masmoudi</b><br>
      <sub>Étudiant ENSI II1A</sub>
    </td>
    <td align="center">
      <b>Mohamed Amine Walha</b><br>
      <sub>Étudiant ENSI II1B</sub>
    </td>
  </tr>
</table>

**Institution :** ENSI (École Nationale des Sciences de l'Informatique)

---

## 📜 Licence

Ce projet est développé à des fins éducatives dans le cadre du projet du cours de Programmation C à l'ENSI.

---

<p align="center">
  Fait avec ❤️ | Sahnoun - Masmoudi - Walha<br>
  <b>Version Actuelle :</b> 2.1
</p>
