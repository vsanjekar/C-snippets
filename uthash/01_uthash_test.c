/*
 * uthash example
 * https://troydhanson.github.io/uthash/userguide.html
 */

#include <stdio.h>
#include "uthash/src/uthash.h"

// In uthash, a hash table is comprised of structures. Each structure represents
// a key-value association. One or more of the structure fields constitute the key.
// The structure pointer itself is the value.
struct my_struct_hash {
    int key;
    char value[16];
    UT_hash_handle hh; // Adding
};

// important! initialize to NULL
struct my_struct_hash *users_hash = NULL;

void add_user(int user_id, char *name) {
    struct my_struct_hash *user;

    user = malloc(sizeof(struct my_struct_hash));
    user->key = user_id;
    strncpy(user->value, name, 16);
    HASH_ADD_INT(users_hash, key, user);
}

void print_users() {
    struct my_struct_hash *user;
    for (user=users_hash; user!=NULL; user=user->hh.next) {
        printf("id:%d name:%s\n", user->key, user->value);
    }
}

int main() {
    add_user(1, "Vinay");
    add_user(2, "Sanjekar");
    print_users();
}
