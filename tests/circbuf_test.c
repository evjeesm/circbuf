#include <check.h>
#include <stdlib.h>
#include "../src/circbuf.h"

#define DATA_SIZE 10
static char data[DATA_SIZE] = {'a', 'b', 'c', 'd', 'e', 'f', 'g' , 'h' , 'i', 'j'};
static circbuf_t *circbuf;

static void setup_empty(void)
{
    circbuf_create(circbuf, .initial_cap = 10);
}

static void teardown(void)
{
    circbuf_destroy(circbuf);
}


START_TEST (test_circbuf_create)
{
    ck_assert_ptr_nonnull(circbuf);
}
END_TEST


START_TEST (test_circbuf_write)
{
    ck_assert_uint_eq(DATA_SIZE, circbuf_write(circbuf, DATA_SIZE, data));

    ck_assert_uint_eq(circbuf_avail_to_write(circbuf), 0);
    ck_assert_uint_eq(circbuf_avail_to_read(circbuf), DATA_SIZE);

    char out[DATA_SIZE] = {0};
    ck_assert_uint_eq(DATA_SIZE, circbuf_read(circbuf, DATA_SIZE, out));

    ck_assert_uint_eq(circbuf_avail_to_write(circbuf), DATA_SIZE);
    ck_assert_uint_eq(circbuf_avail_to_read(circbuf), 0);
    ck_assert_mem_eq(data, out, DATA_SIZE);
}
END_TEST


#define SEQLEN 5
static size_t wr_rd_seq[SEQLEN][3] = { /* write, read, avail_to_read */
    { 5, 2, 3 },
    { 5, 1, 7 },
    { 1, 6, 2 },
    { 8, 4, 6 },
    { 1, 7, 0 }
};
START_TEST (test_circbuf_seq)
{
    for (size_t i = 0; i < SEQLEN; ++i)
    {
        const size_t write = wr_rd_seq[i][0];
        const size_t read = wr_rd_seq[i][1];
        const size_t exp_read_avail = wr_rd_seq[i][2];

        char out[DATA_SIZE] = {0};

        ck_assert_uint_eq(write, circbuf_write(circbuf, write, data));
        ck_assert_uint_eq(read, circbuf_read(circbuf, read, out));
        ck_assert_uint_eq(exp_read_avail, circbuf_avail_to_read(circbuf));
        ck_assert_uint_eq(DATA_SIZE - exp_read_avail, circbuf_avail_to_write(circbuf));

    }
}
END_TEST


START_TEST (test_circbuf_longrun)
{
    srand(911);
    for (size_t w = 0, r = 0; r < 10000;)
    {
        size_t w_rand = (rand() % (DATA_SIZE - 1)) + 1;
        size_t r_rand = (rand() % w_rand) + 1;
        for (size_t wi = 0; wi < w_rand; ++wi, ++w)
        {
            if (0 == circbuf_avail_to_write(circbuf)) break;
            circbuf_write(circbuf, 1, &w);
        }
        for (size_t ri = 0; ri < r_rand; ++ri, ++r)
        {
            char out;
            if (0 == circbuf_avail_to_read(circbuf)) break;
            circbuf_read(circbuf, 1, &out);
            ck_assert_uint_eq(out, (char)r);
        }
    }
}

START_TEST (test_circbuf_clear)
{
    circbuf_write(circbuf, DATA_SIZE, data);
    circbuf_clear(circbuf);

    ck_assert_uint_eq(DATA_SIZE, circbuf_avail_to_write(circbuf));
    ck_assert_uint_eq(0, circbuf_avail_to_read(circbuf));
}
END_TEST


Suite *circbuf_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Circbuf");
    
    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup_empty, teardown);
    tcase_add_test(tc_core, test_circbuf_create);
    tcase_add_test(tc_core, test_circbuf_write);
    tcase_add_test(tc_core, test_circbuf_clear);
    tcase_add_test(tc_core, test_circbuf_seq);
    tcase_add_test(tc_core, test_circbuf_longrun);
    suite_add_tcase(s, tc_core);

    return s;
}


int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = circbuf_suite();
    sr = srunner_create(s);

    /* srunner_set_fork_status(sr, CK_NOFORK); */
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

