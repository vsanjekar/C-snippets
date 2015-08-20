/*
 * uthash example
 * https://troydhanson.github.io/uthash/userguide.html
 */

#include <stdio.h>
#include "uthash/src/uthash.h"

// In uthash, a hash table is comprised of structures. Each structure represents
// a key-value association. One or more of the structure fields constitute the key.
// The structure pointer itself is the value.
struct simple_struct_hash {
    int key;
    char value[16];
    UT_hash_handle hh; // makes this structure hashable
};

// important! initialize to NULL
struct simple_struct_hash *users_hash = NULL;

// uthash from existing struct
struct my_struct {
    int key;
    char value[16];
};

struct my_struct_hash {
    struct my_struct *s;
    UT_hash_handle hh; // makes this structure hashable
};

struct my_struct_hash *products_hash = NULL;


void add_product(int product_id, char *name) {
    struct my_struct_hash *product_entry;

    product_entry = malloc(sizeof(struct my_struct_hash));
    product_entry->s = malloc(sizeof(struct my_struct));
    product_entry->s->key = product_id;
    strncpy(product_entry->s->value, name, 16);
    HASH_ADD_INT(products_hash, s->key, product_entry);
}

void print_products() {
    struct my_struct_hash *product_entry;
    for (product_entry=products_hash; product_entry!=NULL; product_entry=product_entry->hh.next) {
        printf("id:%d name:%s\n", product_entry->s->key, product_entry->s->value);
    }
}

void add_user(int user_id, char *name) {
    struct simple_struct_hash *user;

    user = malloc(sizeof(struct simple_struct_hash));
    user->key = user_id;
    strncpy(user->value, name, 16);
    HASH_ADD_INT(users_hash, key, user);
}

void print_users() {
    struct simple_struct_hash *user;
    for (user=users_hash; user!=NULL; user=user->hh.next) {
        printf("id:%d name:%s\n", user->key, user->value);
    }
}

int main() {
    add_user(1, "Vinay");
    add_user(2, "Sanjekar");
    print_users();
    add_product(1, "Macbook");
    add_product(2, "iPad");
    print_products();
}
