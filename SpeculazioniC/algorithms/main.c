#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("  \033[31m[FAIL]\033[0m %s\n", msg); \
        tests_failed++; \
        return; \
    } \
} while (0)

static void print_int(const void *data)
{
    if (data)
        printf("%d ", *(int *)data);
}

static void noop_fn(const void *data)
{
    (void)data;
}

static void free_int(void *data)
{
    free(data);
}

static int *new_int(int val)
{
    int *p = malloc(sizeof(int));
    if (p)
        *p = val;
    return p;
}

static void dump(const char *label, Vector *v)
{
    if (!v)
    {
        printf("  %-22s NULL\n", label);
        return;
    }
    printf("  %-22s size=%d  capacity=%d  [", label,
           vector_size(v), vector_capacity(v));
    for (int i = 0; i < vector_size(v); i++)
    {
        int *p = vector_get(v, i);
        if (p)
            printf("%d", *p);
        else
            printf("NULL");
        if (i < vector_size(v) - 1)
            printf(", ");
    }
    printf("]\n");
}

static void test_create(const char *test_name)
{
    printf(">>> %s\n", test_name);
    printf("  vector_create(5, free_int)\n");
    Vector *v = vector_create(5, free_int);
    ASSERT(v, "v non deve essere NULL");
    dump("dopo create", v);
    printf("  size=%d (atteso 0), capacity=%d (atteso 5)\n",
           vector_size(v), vector_capacity(v));
    ASSERT(vector_capacity(v) == 5, "capacity deve essere 5");
    ASSERT(vector_size(v) == 0, "size deve essere 0");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_create_empty(const char *test_name)
{
    printf(">>> %s\n", test_name);
    printf("  vector_create_empty(free_int)\n");
    Vector *v = vector_create_empty(free_int);
    ASSERT(v, "v non deve essere NULL");
    dump("dopo create_empty", v);
    printf("  size=%d (atteso 0), capacity=%d (atteso 0)\n",
           vector_size(v), vector_capacity(v));
    ASSERT(vector_capacity(v) == 0, "capacity deve essere 0");
    ASSERT(vector_size(v) == 0, "size deve essere 0");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_init(const char *test_name)
{
    printf(">>> %s\n", test_name);
    void *ptrs[] = {new_int(10), new_int(20), new_int(30)};
    printf("  array C: [10, 20, 30] (heap-allocati)\n");
    printf("  vector_init(ptrs, 3, free_int)\n");
    Vector *v = vector_init(ptrs, 3, free_int);
    ASSERT(v, "v non deve essere NULL");
    dump("dopo init", v);
    printf("  get(0)=%d (atteso 10)\n", *(int *)vector_get(v, 0));
    printf("  get(1)=%d (atteso 20)\n", *(int *)vector_get(v, 1));
    printf("  get(2)=%d (atteso 30)\n", *(int *)vector_get(v, 2));
    ASSERT(*(int *)vector_get(v, 0) == 10, "get(0) deve essere 10");
    ASSERT(*(int *)vector_get(v, 1) == 20, "get(1) deve essere 20");
    ASSERT(*(int *)vector_get(v, 2) == 30, "get(2) deve essere 30");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_get_set(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int a = 1, b = 2, c = 3;
    Vector *v = vector_create_empty(NULL);
    vector_push(v, &a);
    vector_push(v, &b);
    vector_push(v, &c);
    dump("vettore iniziale", v);
    printf("  valori iniziali: [1, 2, 3]\n");

    printf("  vector_set(1, 3)\n");
    vector_set(v, 1, &c);
    dump("dopo set(1,3)", v);
    printf("  get(0)=%d (atteso 1)\n", *(int *)vector_get(v, 0));
    printf("  get(1)=%d (atteso 3, era 2)\n", *(int *)vector_get(v, 1));
    ASSERT(*(int *)vector_get(v, 0) == 1, "get(0) deve essere 1");
    ASSERT(*(int *)vector_get(v, 1) == 3, "get(1) deve essere 3");

    printf("  get(-1)=%s (atteso NULL)\n",
           vector_get(v, -1) ? "non-NULL" : "NULL");
    printf("  get(5)=%s (atteso NULL)\n",
           vector_get(v, 5) ? "non-NULL" : "NULL");
    ASSERT(vector_get(v, -1) == NULL, "get(-1) deve restituire NULL");
    ASSERT(vector_get(v,  5) == NULL, "get(5) deve restituire NULL");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_push(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int a = 1, b = 2, c = 3;
    Vector *v = vector_create_empty(NULL);
    dump("vuoto", v);

    printf("  push(1) -> %d\n", vector_push(v, &a));
    dump("dopo push 1", v);
    printf("  size=%d (atteso 1), get(0)=%d (atteso 1)\n",
           vector_size(v), *(int *)vector_get(v, 0));
    ASSERT(vector_size(v) == 1, "size deve essere 1 dopo primo push");
    ASSERT(*(int *)vector_get(v, 0) == 1, "get(0) deve essere 1");

    printf("  push(2) -> %d\n", vector_push(v, &b));
    dump("dopo push 2", v);
    printf("  push(3) -> %d\n", vector_push(v, &c));
    dump("dopo push 3", v);

    printf("  size=%d (atteso 3)\n", vector_size(v));
    printf("  get(0)=%d, get(1)=%d, get(2)=%d  (attesi 1, 2, 3)\n",
           *(int *)vector_get(v, 0), *(int *)vector_get(v, 1),
           *(int *)vector_get(v, 2));
    ASSERT(vector_size(v) == 3, "size deve essere 3");
    ASSERT(*(int *)vector_get(v, 0) == 1, "get(0) deve essere 1");
    ASSERT(*(int *)vector_get(v, 1) == 2, "get(1) deve essere 2");
    ASSERT(*(int *)vector_get(v, 2) == 3, "get(2) deve essere 3");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_push_growth(const char *test_name)
{
    printf(">>> %s\n", test_name);
    Vector *v = vector_create_empty(free_int);
    dump("vuoto (capacity=0)", v);

    printf("  push 18 elementi (0..17):\n");
    int prev_cap = 0;
    for (int i = 0; i < 18; i++)
    {
        int *p = new_int(i);
        int ok = vector_push(v, p);
        int cap = vector_capacity(v);
        if (cap != prev_cap)
        {
            printf("    dopo push(%2d): capacity %d -> %d\n", i, prev_cap, cap);
            prev_cap = cap;
        }
        ASSERT(ok, "push deve riuscire");
    }
    dump("finale", v);
    int cap = vector_capacity(v);
    printf("  capacity=%d, size=%d\n", cap, vector_size(v));
    ASSERT(vector_size(v) == 18, "size deve essere 18");
    ASSERT(cap >= 18, "capacity deve essere >= 18");
    for (int i = 0; i < 18; i++)
        ASSERT(*(int *)vector_get(v, i) == i, "elemento deve corrispondere all'indice");
    printf("  tutti i 18 elementi verificati con successo\n");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_pop(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int a = 1, b = 2, c = 3;
    Vector *v = vector_create_empty(NULL);
    vector_push(v, &a);
    vector_push(v, &b);
    vector_push(v, &c);
    dump("vettore iniziale [1, 2, 3]", v);

    int *val = vector_pop(v);
    printf("  pop() -> %d, size=%d (atteso 3, size=2)\n",
           val ? *val : -1, vector_size(v));
    ASSERT(val && *val == 3, "primo pop deve essere 3");
    ASSERT(vector_size(v) == 2, "size deve essere 2 dopo primo pop");

    val = vector_pop(v);
    printf("  pop() -> %d, size=%d (atteso 2, size=1)\n",
           val ? *val : -1, vector_size(v));
    ASSERT(val && *val == 2, "secondo pop deve essere 2");

    val = vector_pop(v);
    printf("  pop() -> %d, size=%d (atteso 1, size=0)\n",
           val ? *val : -1, vector_size(v));
    ASSERT(val && *val == 1, "terzo pop deve essere 1");

    val = vector_pop(v);
    printf("  pop() -> NULL, size=%d (atteso NULL, size=0)\n",
           vector_size(v));
    ASSERT(val == NULL, "pop su vuoto deve restituire NULL");

    dump("dopo tutti i pop", v);
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_for_each(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int a = 1, b = 2, c = 3;
    Vector *v = vector_create_empty(NULL);
    vector_push(v, &a);
    vector_push(v, &b);
    vector_push(v, &c);
    dump("vettore", v);

    printf("  output for_each: ");
    vector_for_each(v, print_int);
    printf("\n");
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_resize(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int d[] = {1, 2, 3, 4};
    Vector *v = vector_create(2, NULL);
    printf("  create(2): capacity=2, size=0\n");
    vector_push(v, &d[0]);
    vector_push(v, &d[1]);
    dump("dopo 2 push [1, 2]", v);

    printf("  vector_resize(4) -- espando\n");
    vector_resize(v, 4);
    dump("dopo resize(4)", v);
    printf("  capacity=%d (atteso 4), size=%d (atteso 2)\n",
           vector_capacity(v), vector_size(v));
    ASSERT(vector_capacity(v) == 4, "capacity deve essere 4");
    ASSERT(vector_size(v) == 2, "size deve rimanere 2");

    vector_push(v, &d[2]);
    vector_push(v, &d[3]);
    dump("dopo push 3, 4", v);

    printf("  vector_resize(1) -- tronco\n");
    vector_resize(v, 1);
    dump("dopo resize(1)", v);
    printf("  capacity=%d (atteso 1), size=%d (atteso 1)\n",
           vector_capacity(v), vector_size(v));
    ASSERT(vector_capacity(v) == 1, "capacity deve essere 1");
    ASSERT(vector_size(v) == 1, "size deve essere 1 (troncato)");
    ASSERT(*(int *)vector_get(v, 0) == 1, "l'unico elemento rimasto deve essere 1");
    printf("  unico elemento superstite: %d (atteso 1)\n",
           *(int *)vector_get(v, 0));
    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void *sum_int(void *acc, const void *elem)
{
    int old = *(int *)acc;
    *(int *)acc += *(int *)elem;
    printf("    passo: acc %d + %d = %d\n", old, *(int *)elem, *(int *)acc);
    return acc;
}

static void test_reduce(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int dummy[] = {1, 2, 3, 4, 5};
    Vector *v = vector_create_empty(NULL);
    for (int i = 0; i < 5; i++)
        vector_push(v, &dummy[i]);
    dump("vettore [1,2,3,4,5]", v);

    int acc = 0;
    printf("  reduce(acc=0, acc += elem):\n");
    void *res = vector_reduce(v, sum_int, &acc);
    printf("  risultato: %d (atteso 15), puntatore restituito: %s\n",
           acc, res == &acc ? "&acc (corretto)" : "altro");
    ASSERT(acc == 15, "somma deve essere 15");
    ASSERT(res == &acc, "reduce deve restituire lo stesso puntatore dell'accumulatore");

    printf("  reduce con v=NULL:\n");
    void *null_res = vector_reduce(NULL, sum_int, &acc);
    printf("  restituito %s (atteso &acc=%p)\n",
           null_res == &acc ? "&acc" : "altro", (void *)&acc);
    ASSERT(null_res == &acc, "reduce con v=NULL restituisce l'accumulatore invariato");

    vector_free(v);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void *double_int(const void *elem)
{
    int *p = malloc(sizeof(int));
    if (p)
        *p = *(int *)elem * 2;
    return p;
}

static void test_map(const char *test_name)
{
    printf(">>> %s\n", test_name);
    int dummy[] = {1, 2, 3};
    Vector *v = vector_create_empty(NULL);
    for (int i = 0; i < 3; i++)
        vector_push(v, &dummy[i]);
    dump("originale [1, 2, 3]", v);

    printf("  map(x -> x * 2, free_int):\n");
    Vector *mapped = vector_map(v, double_int, free_int);
    ASSERT(mapped, "mapped non deve essere NULL");
    dump("mappato", mapped);
    printf("  mapped[0] = %d (atteso 2)\n", *(int *)vector_get(mapped, 0));
    printf("  mapped[1] = %d (atteso 4)\n", *(int *)vector_get(mapped, 1));
    printf("  mapped[2] = %d (atteso 6)\n", *(int *)vector_get(mapped, 2));
    ASSERT(*(int *)vector_get(mapped, 0) == 2, "mapped[0] deve essere 2");
    ASSERT(*(int *)vector_get(mapped, 1) == 4, "mapped[1] deve essere 4");
    ASSERT(*(int *)vector_get(mapped, 2) == 6, "mapped[2] deve essere 6");

    printf("  map(NULL, ...) = %s (atteso NULL)\n",
           vector_map(NULL, double_int, free_int) ? "non-NULL" : "NULL");
    ASSERT(vector_map(NULL, double_int, free_int) == NULL,
           "map con v=NULL restituisce NULL");

    vector_free(v);
    vector_free(mapped);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

static void test_null_safety(const char *test_name)
{
    printf(">>> %s\n", test_name);
    printf("  vector_free(NULL) -> no-op\n");
    vector_free(NULL);
    printf("  vector_for_each(NULL, ...) -> no-op\n");
    vector_for_each(NULL, noop_fn);
    printf("  vector_resize(NULL, 10) -> no-op\n");
    vector_resize(NULL, 10);
    printf("  vector_get(NULL, 0) = %s (atteso NULL)\n",
           vector_get(NULL, 0) ? "non-NULL" : "NULL");
    ASSERT(vector_get(NULL, 0) == NULL, "get(NULL,0) restituisce NULL");
    printf("  vector_pop(NULL) = %s (atteso NULL)\n",
           vector_pop(NULL) ? "non-NULL" : "NULL");
    ASSERT(vector_pop(NULL) == NULL, "pop(NULL) restituisce NULL");
    printf("  vector_push(NULL, NULL) = %d (atteso 0)\n",
           vector_push(NULL, NULL));
    ASSERT(vector_push(NULL, NULL) == 0, "push(NULL,NULL) restituisce 0");
    printf("  vector_set(NULL, 0, NULL) -> no-op\n");
    vector_set(NULL, 0, NULL);
    printf("  \033[32m[PASS]\033[0m\n\n");
    tests_passed++;
}

int main(void)
{
#define RUN(fn) fn(#fn)
    RUN(test_create);
    RUN(test_create_empty);
    RUN(test_init);
    RUN(test_get_set);
    RUN(test_push);
    RUN(test_push_growth);
    RUN(test_pop);
    RUN(test_for_each);
    RUN(test_resize);
    RUN(test_reduce);
    RUN(test_map);
    RUN(test_null_safety);

    int total = tests_passed + tests_failed;
    printf("=== %d / %d tests passed", tests_passed, total);
    if (tests_failed > 0)
        printf(", %d FAILED", tests_failed);
    printf(" ===\n");
    return tests_failed;
}
