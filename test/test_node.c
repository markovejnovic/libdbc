#include <check.h>
#include "libdbc_node.h"

START_TEST(tc_name_simple)
{
    const dbc_node_t node = dbc_node_new("MYNODE");
    ck_assert_str_eq(dbc_node_get_name(node), "MYNODE");
    dbc_node_free(node);
}
END_TEST

START_TEST(tc_name_supports_utf8)
{
    const dbc_node_t node = dbc_node_new("MYNODE和平");
    ck_assert_str_eq(dbc_node_get_name(node), "MYNODE和平");
    dbc_node_free(node);
}

int main(void)
{
    Suite* const s = suite_create("Node");

    {
        TCase* const tc = tcase_create("With Name");
        tcase_add_test(tc, tc_name_simple);
        tcase_add_test(tc, tc_name_supports_utf8);

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
