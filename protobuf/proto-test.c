#include <stdio.h>
#include <stdlib.h>

#include "message.pb-c.h"

int main()
{
    Person p1 = PERSON__INIT;

    if (p1.name)
        printf("%s\n",p1.name);

    Person *persons;
    persons = malloc(sizeof(Person)*8);

    for (size_t i=0; i<8; i++) {
        person__init(&persons[i]);
        asprintf(&persons[i].name, "Vinay");
    }

    for (size_t i=0; i<8; i++)
        printf("%s\n",persons[i].name);

    return 0;
}
