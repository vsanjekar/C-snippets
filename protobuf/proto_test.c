#include <stdio.h>
#include <stdlib.h>

#include "message.pb-c.h"

Repository rep = REPOSITORY__INIT;

void serialize_repository() {
    // Serialization
    rep.n_persons = 8;
    rep.persons = malloc(sizeof(Person *)*rep.n_persons);

    Person **persons = rep.persons;
    for (size_t i=0; i<rep.n_persons; i++) {
        persons[i] = malloc(sizeof(Person));
        person__init(persons[i]);
        persons[i]->id = i+1;
        asprintf(&persons[i]->name, "Vinay");
    }
    for (size_t i=0; i<rep.n_persons; i++)
        printf("%d %s\n", persons[i]->id, persons[i]->name);

    int32_t len = repository__get_packed_size(&rep);
    void * buf = malloc(len);

    fprintf(stderr,"Writing %d serialized bytes\n",len);
    // Write to stdout to allow direct command line piping
    fwrite(buf,len,1,stdout);

    free(buf);
}

int main()
{
    Person p1 = PERSON__INIT;
    if (p1.name)
        printf("%s\n",p1.name);

    serialize_repository();
    return 0;
}
