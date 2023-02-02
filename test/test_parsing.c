#include <check.h>
#include "libdbc.h"
#include "parser.c"

START_TEST(version_simple)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"v0.1.0\"";
    __dbc_parse_version(dbc, str);
    dbc_free(dbc);
}
END_TEST

START_TEST(version_no_space)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version\"v0.1.0\"";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_str_eq(dbc_get_version(dbc), "v0.1.0");
    dbc_free(dbc);
}
END_TEST

START_TEST(version_trailing_space)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"v0.1.0\"    ";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_str_eq(dbc_get_version(dbc), "v0.1.0");
    dbc_free(dbc);
}
END_TEST

START_TEST(version_double_quote)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"v0.1.0\"    \"";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_str_eq(dbc_get_version(dbc), "v0.1.0");
    dbc_free(dbc);
}

START_TEST(version_utf8)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"v0.1.0和平\"";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_str_eq(dbc_get_version(dbc), "v0.1.0和平");
    dbc_free(dbc);
}

START_TEST(version_long)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"v0.1.0和平3030,corporateinstitutionalbankoftime\"";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_str_eq(dbc_get_version(dbc),
                     "v0.1.0和平3030,corporateinstitutionalbankoftime");
    dbc_free(dbc);
}

START_TEST(version_empty)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"\"";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_str_eq(dbc_get_version(dbc), "");
    dbc_free(dbc);
}

START_TEST(version_malformed_simple)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_MALFORMED);
    ck_assert_str_eq(dbc_get_version(dbc), "");
    dbc_free(dbc);
}

START_TEST(version_malformed_whitespace)
{
    const dbc_t dbc = dbc_new();
    char str[] = "Version \"                 ";
    ck_assert_uint_eq(__dbc_parse_version(dbc, str), PARSE_ERR_MALFORMED);
    ck_assert_str_eq(dbc_get_version(dbc), "");
    dbc_free(dbc);
}

START_TEST(nodes_simple)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_ NODE1";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 1);
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 0)), "NODE1");
    dbc_free(dbc);
}

START_TEST(nodes_multiple)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_ NODE1 NODE2";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 2);
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 0)), "NODE1");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 1)), "NODE2");
    dbc_free(dbc);
}

START_TEST(nodes_utf8)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_ NODE1 NODE2和平";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_MALFORMED);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 2);
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 0)), "NODE1");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 1)), "NODE2和平");
    dbc_free(dbc);
}

START_TEST(nodes_many)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_ a b c d e f g h";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 8);
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 0)), "a");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 1)), "b");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 2)), "c");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 3)), "d");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 4)), "e");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 5)), "f");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 6)), "g");
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 7)), "h");
    dbc_free(dbc);
}

START_TEST(nodes_whitespace_head)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_       a";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 1);
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 0)), "a");
    dbc_free(dbc);
}

START_TEST(nodes_whitespace_tail)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_ a        ";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 1);
    ck_assert_str_eq(dbc_node_get_name(dbc_get_node(dbc, 0)), "a");
    dbc_free(dbc);
}

START_TEST(nodes_none)
{
    const dbc_t dbc = dbc_new();
    char str[] = "BU_         ";
    ck_assert_uint_eq(__dbc_parse_nodes(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_nodes(dbc), 0);
    dbc_free(dbc);
}

START_TEST(vtables_empty)
{
    const dbc_t dbc = dbc_new();
    char str[] = "VAL_TABLE_ m_table ;";
    ck_assert_uint_eq(__dbc_parse_value_table(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_value_tables(dbc), 1);

    const dbc_value_table_t val_tbl = dbc_get_value_table(dbc, "m_table");
    ck_assert_str_eq(dbc_value_table_get_name(val_tbl), "m_table");


    dbc_free(dbc);
}

START_TEST(vtables_simple)
{
    const dbc_t dbc = dbc_new();
    char str[] = "VAL_TABLE_ m_table 0 \"Zero\" ;";
    ck_assert_uint_eq(__dbc_parse_value_table(dbc, str), PARSE_ERR_SUCCESS);
    ck_assert_uint_eq(dbc_get_num_value_tables(dbc), 1);

    const dbc_value_table_t val_tbl = dbc_get_value_table(dbc, "m_table");
    ck_assert_str_eq(dbc_value_table_get_name(val_tbl), "m_table");
    ck_assert_uint_eq(dbc_value_table_get_num_values(val_tbl), 1);
    ck_assert_str_eq(dbc_value_table_get_value(val_tbl, 0), "Zero");

    dbc_free(dbc);
}

int main(void)
{
    Suite* const s = suite_create("Parsing");

    {
        TCase* const tc = tcase_create("Version");
        tcase_add_test(tc, version_simple);
        tcase_add_test(tc, version_no_space);
        tcase_add_test(tc, version_trailing_space);
        tcase_add_test(tc, version_double_quote);
        tcase_add_test(tc, version_utf8);
        tcase_add_test(tc, version_long);
        tcase_add_test(tc, version_empty);
        tcase_add_test(tc, version_malformed_simple);
        tcase_add_test(tc, version_malformed_whitespace);
        suite_add_tcase(s, tc);
    }

    {
        TCase* const tc = tcase_create("Nodes");
        tcase_add_test(tc, nodes_simple);
        tcase_add_test(tc, nodes_multiple);
        tcase_add_test(tc, nodes_utf8);
        tcase_add_test(tc, nodes_many);
        tcase_add_test(tc, nodes_whitespace_head);
        tcase_add_test(tc, nodes_whitespace_tail);
        tcase_add_test(tc, nodes_none);
        suite_add_tcase(s, tc);
    }

    {
        TCase* const tc = tcase_create("Value Tables");
        tcase_add_test(tc, vtables_empty);
        tcase_add_test(tc, vtables_simple);
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
