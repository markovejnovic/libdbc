#include <check.h>
#include "libdbc.h"

START_TEST(tc_newfree_1)
{
    const dbc_t dbc = dbc_new();
    dbc_free(dbc);
}
END_TEST

START_TEST(tc_version_getset)
{
    const char* const expected_str = "v0.1.0";

    const dbc_t dbc = dbc_new();
    dbc_set_version(dbc, expected_str);
    ck_assert_str_eq(dbc_get_version(dbc), expected_str);

    dbc_free(dbc);
}

START_TEST(tc_version_copied)
{
    char* str = "v0.1.0";

    const dbc_t dbc = dbc_new();
    dbc_set_version(dbc, str);
    str = "v0.2.0";

    ck_assert_str_eq(dbc_get_version(dbc), "v0.1.0");

    dbc_free(dbc);
}

int main(void)
{
    Suite* const s = suite_create("CRUD");

    {
        TCase* const tc = tcase_create("New/Free");
        tcase_add_test(tc, tc_newfree_1);

        suite_add_tcase(s, tc);
    }

    {
        TCase* const tc = tcase_create("Version");
        tcase_add_test(tc, tc_version_getset);
        tcase_add_test(tc, tc_version_copied);

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
