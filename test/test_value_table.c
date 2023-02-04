#include <check.h>
#include "libdbc_value_table.h"
#include <stdio.h>

START_TEST(tc_empty)
{
    const dbc_value_table_t vt = dbc_value_table_new("m_table");
    ck_assert_str_eq(dbc_value_table_get_name(vt), "m_table");
    ck_assert_uint_eq(dbc_value_table_get_size(vt), 0);
    dbc_value_table_free(vt);
}
END_TEST

START_TEST(tc_insert)
{
    const dbc_value_table_t vt = dbc_value_table_new("m_table");
    ck_assert_str_eq(dbc_value_table_get_name(vt), "m_table");
    dbc_value_table_insert(vt, 0.0, "SOMETHING");
    ck_assert_uint_eq(dbc_value_table_get_size(vt), 1);
    ck_assert_str_eq(dbc_value_table_get_desc(vt, 0.0), "SOMETHING");
    dbc_value_table_free(vt);
}
END_TEST

START_TEST(tc_insert_many)
{
    const size_t num_values = 100000;

    const dbc_value_table_t vt = dbc_value_table_new("m_table");
    ck_assert_str_eq(dbc_value_table_get_name(vt), "m_table");

    for (size_t i = 0; i < num_values; i++) {
        char str_buf[20];
        snprintf(str_buf, 20, "%f", (double)i);
        dbc_value_table_insert(vt, (double)i, str_buf);
    }
    ck_assert_uint_eq(dbc_value_table_get_size(vt), num_values);
    for (size_t i = 0; i < num_values; i++) {
        char str_buf[20];
        snprintf(str_buf, 20, "%f", (double)i);
        ck_assert_str_eq(dbc_value_table_get_desc(vt, (double)i), str_buf);
    }

    dbc_value_table_free(vt);
}
END_TEST

int main(void)
{
    Suite* const s = suite_create("Value Table");

    {
        TCase* const tc = tcase_create("Empty");
        tcase_add_test(tc, tc_empty);
        suite_add_tcase(s, tc);
    }

    {
        TCase* const tc = tcase_create("Single Insert");
        tcase_add_test(tc, tc_insert);
        suite_add_tcase(s, tc);
    }

    {
        TCase* const tc = tcase_create("Many Insert");
        tcase_add_test(tc, tc_insert_many);
        suite_add_tcase(s, tc);
    }

    {
        SRunner* const sr = srunner_create(s);
        srunner_run_all(sr, CK_NORMAL);
        const size_t number_failed = srunner_ntests_failed(sr);
        srunner_free(sr);

        return number_failed;
    }

}
